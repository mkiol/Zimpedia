/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "zimserver.h"

#include <sailfishapp.h>
#include <zim/archive.h>
#include <zim/blob.h>
#include <zim/entry.h>
#include <zim/error.h>
#include <zim/item.h>
#include <zim/search.h>

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QUrl>
#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include "articlemodel.h"
#include "filemodel.h"
#include "settings.h"

ZimServer *ZimServer::m_instance = nullptr;

ZimServer *ZimServer::instance(QObject *parent) {
    if (ZimServer::m_instance == nullptr) {
        ZimServer::m_instance = new ZimServer(parent);
    }

    return ZimServer::m_instance;
}

ZimServer::ZimServer(QObject *parent) : QThread{parent}, m_server{parent} {
    connect(&m_server, &QHttpServer::newRequest, this,
            &ZimServer::requestHandler);
    connect(FileModel::instance(), &FileModel::busyChanged, this,
            &ZimServer::handleFilemodelChanged, Qt::QueuedConnection);
    connect(
        this, &ZimServer::loadedChanged, this,
        [] { FileModel::instance()->updateModel(); }, Qt::QueuedConnection);
    connect(
        this, &ZimServer::filesChanged, this,
        [] { FileModel::instance()->updateModel(); }, Qt::QueuedConnection);

    if (!m_server.listen(port)) {
        qWarning() << "unable to start HTTP server on port" << port;
        throw std::runtime_error("unable to start HTTP server");
    }
}

void ZimServer::handleFilemodelChanged() {
    if (m_delayedLoadZim && !FileModel::instance()->busy()) {
        m_delayedLoadZim = false;
        qDebug() << "delayed zim load";
        loadZim();
    }
}

void ZimServer::run() {
    if (m_workType == WorkType::article) {
        m_workType = WorkType::none;
        articleAsyncWork();
    } else if (m_workType == WorkType::loadZim) {
        m_workType = WorkType::none;
        loadZimAsyncWork();
    }
}

void ZimServer::openUrl(const QUrl &url, const QString &title) {
    qDebug() << "opening url:" << url;

    const auto &[uuid, path] = parseUrl(url);

    if (uuid.isEmpty() || path.isEmpty()) {
        qWarning() << "invalid url:" << url;
        return;
    }

    if (!loadZimUuidEmit(uuid)) {
        qWarning() << "unable to load zim file with uuid:" << uuid;
        return;
    }

    emit urlReady(url, title);
}

bool ZimServer::loadZimUuidEmit(const QString &uuid) {
    setBusy(true);
    auto ok = loadZimUuid(uuid);
    if (ok) Settings::instance()->addZimFile(uuid);
    setLoaded(ok);
    emit filesChanged();
    setBusy(false);
    return ok;
}

bool ZimServer::unLoadZimUuidEmit(const QString &uuid) {
    setBusy(true);
    auto ok = unLoadZimUuid(uuid);
    if (ok) Settings::instance()->removeZimFile(uuid);
    setLoaded(!m_archive.empty());
    emit filesChanged();
    setBusy(false);
    return ok;
}

void ZimServer::loadZimAsyncWork() {
    setBusy(true);
    setLoaded(loadZimUuids(std::move(m_uuidsToLoad)));
    emit filesChanged();
    setBusy(false);
}

bool ZimServer::loadZim() {
    auto uuids = Settings::instance()->zimFiles();

    if (uuids.empty()) {
        qDebug() << "uuid list is empty";
        m_archive.clear();
        m_meta.clear();
        emit filesChanged();
        setLoaded(false);
        setBusy(false);
        return false;
    }

    if (m_archive.size() == uuids.size() &&
        std::all_of(uuids.cbegin(), uuids.cend(), [this](const auto &uuid) {
            return m_archive.contains(uuid);
        })) {
        qDebug() << "files already loaded";
        setBusy(false);
        return true;
    }

    if (FileModel::instance()->busy()) {
        qDebug() << "file scanning in progress, so delaying zim load";
        m_delayedLoadZim = true;
        setBusy(true);
        return false;
    }

    setBusy(true);
    m_uuidsToLoad = std::move(uuids);
    m_workType = WorkType::loadZim;
    start(QThread::IdlePriority);
    return true;
}

void ZimServer::setBusy(bool busy) {
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged();
    }
}

void ZimServer::setLoaded(bool loaded) {
    if (m_loaded != loaded) {
        m_loaded = loaded;
        emit loadedChanged();
    }
}

bool ZimServer::unLoadZimUuid(const QString &uuid) {
    if (!m_archive.contains(uuid)) return false;

    qDebug() << "unload zim uuid:" << uuid;

    m_archive.remove(uuid);
    m_meta.remove(uuid);

    qDebug() << "zim successfully unloaded:" << uuid;
    return true;
}

bool ZimServer::loadZimUuid(const QString &uuid) {
    if (m_archive.contains(uuid)) return true;

    qDebug() << "load zim uuid:" << uuid;

    if (!FileModel::instance()->uuidExists(uuid)) {
        qWarning() << "zim file with uuid doesn't exist";
        return false;
    }

    auto &meta = FileModel::instance()->uuidToMeta(uuid);

    if (!QFile::exists(meta.path)) {
        qWarning() << "zim file with path doesn't exist:" << meta.path;
        return false;
    }

    meta.fields = ZimMetaData::Title | ZimMetaData::Uuid |
                  ZimMetaData::Language | ZimMetaData::Icon | ZimMetaData::Tags;

    if (!FileModel::scanZim(&meta)) {
        qWarning() << "scan failed";
        return false;
    }

    try {
        m_archive.insert(
            uuid, std::make_shared<zim::Archive>(meta.path.toStdString()));
    } catch (const std::exception &e) {
        qWarning() << "unable to create archive:" << e.what();
        return false;
    }

    m_meta.insert(uuid, meta);

    qDebug() << "zim successfully loaded:" << uuid;
    return true;
}

bool ZimServer::loadZimUuids(QStringList &&uuids) {
    qDebug() << "loading zim uuids:" << uuids << m_archive.keys();

    ArticleModel::instance()->setFilter({});

    m_archive.clear();
    m_meta.clear();

    for (auto &uuid : uuids) {
        if (!loadZimUuid(uuid)) {
            uuid.clear();
        }
    }

    uuids.removeAll({});

    qDebug() << "successfully loaded uuids:" << uuids;
    Settings::instance()->setZimFiles(uuids);
    return !m_archive.empty();
}

std::optional<QByteArray> ZimServer::resContent(const QString &filename) {
    QFile file{SailfishApp::pathTo("res/" + filename).toLocalFile()};
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "could not open file: " << filename << file.errorString();
        return std::nullopt;
    }
    return file.readAll();
}

QString ZimServer::contentType(const QString &file) {
    auto l = file.split(".");
    QString ext = l.isEmpty() ? "" : l.last();
    return ext == "css"   ? "text/css; charset=utf-8"
           : ext == "gif" ? "image/gif"
           : ext == "png" ? "image/png"
           : ext == "js"  ? "application/x-javascript; charset=utf-8"
                          : "text/html; charset=utf-8";
}

std::optional<ArticleResult> ZimServer::article(const QString &path,
                                                const QString &uuid) {
    if (!uuidLoaded(uuid)) {
        qWarning() << "uuid not loaded:" << uuid;
        return std::nullopt;
    }
    return article(*m_archive[uuid], path);
}

std::optional<ArticleResult> ZimServer::article(const zim::Archive &archive,
                                                const QString &path) {
    std::optional<zim::Item> article;

    // qDebug() << "searching for path:" << path;

    if (path == "A/mainpage") {
        if (archive.hasMainEntry()) {
            article = archive.getMainEntry().getItem(true);
        } else {
            qWarning() << "main page not found";
            return std::nullopt;
        }
    } else {
        auto entries = archive.findByPath(stringQtoStd(path));
        if (entries.size() == 0) {
            qWarning() << "article not found:" << path;
            return std::nullopt;
        }

        article = entries.begin()->getItem(true);
    }

    auto blob = article->getData();

    if (blob.size() == 0) {
        qWarning() << "article is empty";
        return std::nullopt;
    }

    ArticleResult result{QByteArray{blob.data(), static_cast<int>(blob.size())},
                         QString::fromStdString(article->getMimetype())};

    return result;
}

void ZimServer::articleAsyncWork() {
    if (m_archive.isEmpty()) {
        qWarning() << "no zim files loaded";
        emit articleReady({});
        return;
    }

    auto [path, uuid] = m_articleToAsyncGet;
    auto art = article(path, uuid);
    if (!art) {
        emit articleReady({});
        return;
    }

    if (art->mime == "text/html") {
        art->mime = "text/html; charset=utf-8";
        auto dd = QString::fromUtf8(art->content);
        filter(uuid, &dd);
        emit articleReady(dd);
        return;
    }

    emit articleReady(QString{art->content});
}

void ZimServer::articleAsync(const QUrl &url) {
    qDebug() << "getting article async:" << url;

    const auto &[uuid, path] = parseUrl(url);

    if (uuid.isEmpty() || path.isEmpty()) {
        qWarning() << "invalid url:" << url;
        setBusy(false);
        return;
    }

    m_articleToAsyncGet = {path, uuid};
    m_workType = WorkType::article;
    start(QThread::IdlePriority);
}

std::pair<QString, QString> ZimServer::parseUrl(const QUrl &url) {
    // qDebug() << "parsing url:" << url;

    std::pair<QString, QString> result;

    auto parts = QUrl::fromPercentEncoding(url.path().toLocal8Bit()).split("/");
    if (parts.size() < 4) {
        qWarning() << "invalid url:" << url;
        return result;
    }

    if (parts.at(1).startsWith("uuid:")) {
        result.first = parts.at(1).right(parts.at(1).size() - 5);
        result.second = parts.last() == "mainpage"
                            ? "A/mainpage"
                            : QStringList{parts.mid(2)}.join('/');
    } else {
        result.second = QStringList{parts.mid(1)}.join('/');
    }

    return result;
}

void ZimServer::requestHandler(QHttpRequest *req, QHttpResponse *resp) {
    qDebug() << "http request:" << req->url().path();

    auto [uuid, path] = parseUrl(req->url());

    if (path.isEmpty()) {
        qWarning() << "invalid url:" << req->url();
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end();
        return;
    }

    if (uuid.isEmpty()) {
        qWarning() << "invalid uuid";
        if (m_archive.isEmpty() || m_lastUuid.isEmpty()) {
            qWarning() << "zim file not loaded or uuid unknown";
            resp->setHeader("Content-Length", "0");
            resp->setHeader("Connection", "close");
            resp->writeHead(500);
            resp->end();
            return;
        }
        uuid = m_lastUuid;
    } else {
        if (!loadZimUuidEmit(uuid)) {
            qWarning() << "unable to load zim file with uuid:" << uuid;
            resp->setHeader("Content-Length", "0");
            resp->setHeader("Connection", "close");
            resp->writeHead(500);
            resp->end();
            return;
        }
        m_lastUuid = uuid;
    }

    auto art = article(path, uuid);
    if (!art) {
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end();
        return;
    }

    if (art->mime == "text/html") {
        art->mime = "text/html; charset=utf-8";
        auto dd = QString::fromUtf8(art->content);
        filter(uuid, &dd);
        art->content = dd.toUtf8();
    }

    resp->setHeader("Content-Length", QString::number(art->content.size()));
    resp->setHeader("Content-Type", art->mime);
    resp->setHeader("Connection", "close");
    resp->writeHead(200);
    resp->end(art->content);
}

QUrl ZimServer::localUrl(const QString &path, const QString &uuid) {
    QUrl url{"http://localhost"};
    url.setPort(port);
    url.setPath(QString{"/uuid:%2/%3"}.arg(uuid, path));
    return url;
}

QString ZimServer::titleFromUrl(const QUrl &url) const {
    qDebug() << "searching for title of:" << url.path();

    const auto &[uuid, path] = parseUrl(url);

    if (uuid.isEmpty() || path.isEmpty()) {
        qWarning() << "invalid url:" << url;
        return {};
    }

    if (path == "A/mainpage") {
        return tr("Main page");
    }

    if (!uuidLoaded(uuid)) {
        qWarning() << "uuid not loaded:" << uuid;
        return {};
    }

    auto entries = m_archive[uuid]->findByPath(stringQtoStd(path));
    if (entries.size() == 0) {
        qWarning() << "entry for url not found" << url;
        return {};
    }

    //    qDebug() << "entries size:" << entries.size();
    //    for (auto &e : entries) {
    //        qDebug() << stringStdToQ(e.getTitle());
    //    }

    return stringStdToQ(entries.begin()->getTitle());
}

void ZimServer::searchFullText(const QString &phrase, int maxSize,
                               QList<SearchResult> *result) const {
    try {
        std::vector<zim::Archive> archives;
        archives.reserve(m_archive.size());
        for (auto it = m_archive.cbegin(); it != m_archive.cend(); ++it) {
            if (m_meta[it.key()].ftindex) {
                archives.push_back(*it.value());
            }
        }

        zim::Searcher searcher{archives};
        auto res = searcher.search(phrase.toStdString()).getResults(0, maxSize);
        for (auto it = res.begin(); it != res.end(); ++it) {
            if (QThread::currentThread()->isInterruptionRequested()) break;
            auto uuid =
                QString::fromStdString(static_cast<std::string>(it.getZimId()));
            /*qDebug() << "score:" << stringStdToQ(it.getPath()) <<
               it.getScore()
                     << uuid;*/
            result->push_back({stringStdToQ(it.getTitle()),
                               localUrl(stringStdToQ(it.getPath()), uuid), uuid,
                               it.getScore()});
        }
    } catch (const std::runtime_error &e) {
        qWarning() << "error during full search:" << e.what();
    }
}

void ZimServer::searchTitle(const zim::Archive &archive, const QString &uuid,
                            const QString &phrase, int maxSize,
                            QList<SearchResult> *result) {
    auto res = archive.findByTitle(phrase.toStdString()).offset(0, maxSize);
    for (auto it = res.begin(); it != res.end(); ++it) {
        if (QThread::currentThread()->isInterruptionRequested()) break;
        result->push_back({stringStdToQ(it->getTitle()),
                           localUrl(stringStdToQ(it->getPath()), uuid), uuid});
    }
}

void ZimServer::searchTitle(const QString &phrase, int maxSize,
                            QList<SearchResult> *result) const {
    for (auto it = m_archive.cbegin(); it != m_archive.cend(); ++it) {
        if (QThread::currentThread()->isInterruptionRequested()) break;
        searchTitle(*it.value(), it.key(), phrase, maxSize, result);
    }
}

QStringList static phrasePermuts(const QString &phrase) {
    static const auto maxWords = 6;
    QStringList results;
    auto words = phrase.split(QRegExp{"\\s+"}, QString::SkipEmptyParts);
    auto size = std::min(words.size(), maxWords);
    auto perms = static_cast<int>(std::pow(2, size));
    auto firstLetterIdx = [](const auto &word) {
        for (int i = 0; i < word.size(); ++i) {
            if (word.at(i).isLetter()) return i;
        }
        return -1;
    };
    for (auto per = 0; per < perms; ++per) {
        auto b = std::bitset<maxWords>(per);
        for (int i = 0; i < size; ++i) {
            auto &word = words[i];
            auto idx = firstLetterIdx(word);
            if (idx >= 0) {
                word[idx] =
                    b.test(i) ? word[idx].toUpper() : word[idx].toLower();
            }
        }
        results.push_back(words.join(' '));
    }
    return results;
}

QList<SearchResult> ZimServer::search(QString phrase) const {
    QList<SearchResult> result;

    if (m_archive.isEmpty()) return result;

    phrase = phrase.trimmed();
    if (phrase.isEmpty()) return result;

    if (Settings::instance()->searchMode() == Settings::FullTextSearch) {
        searchFullText(phrase, maxSearch, &result);
    } else {
        auto phrases = phrasePermuts(phrase);
        for (const auto &p : phrases) {
            searchTitle(p, maxSearch, &result);
            if (QThread::currentThread()->isInterruptionRequested())
                return result;
        }
    }

    if (QThread::currentThread()->isInterruptionRequested()) return result;

    std::sort(result.begin(), result.end(),
              [](const SearchResult &a, const SearchResult &b) {
                  if (a.score == b.score)
                      return a.title.compare(b.title, Qt::CaseInsensitive) < 0;
                  return a.score > b.score;
              });

    if (result.size() > maxSearch) {
        result.erase(result.begin() + maxSearch, result.end());
    }

    return result;
}

bool ZimServer::isServerUrl(const QUrl &url) const {
    return url.host() == "localhost" && url.port() == port;
}

QUrl ZimServer::urlToMainPage(const QString &uuid) const {
    QUrl url{"http://localhost"};
    url.setPort(port);
    if (!uuid.isEmpty()) url.setPath(QString{"/uuid:%2/A/mainpage"}.arg(uuid));
    return url;
}

QString ZimServer::fixPath(QString path) {
    if (path.startsWith("..")) {
        path = path.right(path.length() - 2);
    }
    if (path.startsWith("/")) {
        path = path.right(path.length() - 1);
    }
    return path;
}

void ZimServer::filter(const QString &uuid, QString *data) {
    Q_UNUSED(uuid)
    Q_UNUSED(data)
}

QVariantList ZimServer::files() const {
    QVariantList fl;
    for (auto it = m_meta.cbegin(); it != m_meta.cend(); ++it) {
        const auto &meta = it.value();
        fl.push_back(QVariantList{
            meta.uuid, QString{"%1 · %2"}.arg(meta.title, meta.language),
            meta.icon, meta.mainPage});
    }
    std::sort(fl.begin(), fl.end(), [](const QVariant &f1, const QVariant &f2) {
        return f1.toList().at(1).toString().compare(
                   f2.toList().at(1).toString(), Qt::CaseInsensitive) < 0;
    });
    return fl;
}
