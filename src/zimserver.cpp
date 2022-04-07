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
#include <cctype>
#include <limits>
#include <string>

#include "articlemodel.h"
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

    if (!m_server.listen(port)) {
        qWarning() << "unable to start HTTP server on port:" << port;
        throw std::runtime_error("unable to start HTTP server");
    }
}

void ZimServer::run() {
    if (!m_archive) {
        qWarning() << "zim file not loaded";
        emit articleReady({});
        return;
    }

    QString url = urlToAsyncGet;

    auto art = article(url);
    if (!art) {
        emit articleReady({});
        return;
    }

    if (art->mime == "text/html") {
        art->mime = "text/html; charset=utf-8";
        auto dd = QString::fromUtf8(art->content);
        filter(&dd);
        emit articleReady(dd);
        return;
    }

    emit articleReady(QString{art->content});
}

void ZimServer::openUrl(const QString &url, const QString &title) {
    auto parts = url.split("/");
    if (parts.length() < 4) {
        qWarning() << "invalid url:" << url;
        return;
    }

    auto uuid = parts.at(3);

    if (!uuid.startsWith("uuid:")) {
        qWarning() << "invalid uuid:" << uuid;
        return;
    }

    uuid = uuid.right(uuid.length() - 5);
    if (!loadZimFileByUuid(uuid)) {
        qWarning() << "unable to load zim file with uuid:" << uuid;
        return;
    }

    emit urlReady(url, title);
}

bool ZimServer::loadZimFileByUuid(const QString &uuid) {
    if (!uuid.isEmpty() && uuid == m_meta.checksum) {
        return true;
    }

    auto fm = FileModel::instance();
    ZimMetaData meta = fm->files.value(uuid);
    if (meta.empty()) {
        qWarning() << "zim file with uuid doesn't exist:" << uuid;
        return false;
    }

    Settings::instance()->setZimFile(meta.path);

    bool ok = loadZimPath(meta.path);
    if (!ok) emit loadedChanged();

    return ok;
}

bool ZimServer::loadZimFile() {
    Settings *s = Settings::instance();
    bool ok = loadZimPath(s->getZimFile());
    emit loadedChanged();
    return ok;
}

void ZimServer::setBusy(bool busy) {
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged();
    }
}

inline static bool tagExists(const QString &tag, const QString &tags) {
    auto tagList = tags.split(';');
    for (const auto &t : tagList) {
        auto fl = t.split(':');
        if (fl.size() == 1) {
            if (tag == fl.first().trimmed()) return true;
        } else if (fl.size() == 2) {
            if (tag == fl.first().trimmed()) return fl.at(1).trimmed() == "yes";
        }
    }
    return false;
}

bool ZimServer::loadZimPath(const QString &path) {
    qDebug() << "loading zim file:" << path;

    if (!path.isEmpty() && path == m_meta.path) {
        return true;
    }

    ArticleModel::instance()->setFilter({});

    m_archive.reset();
    m_meta.clear();

    if (path.isEmpty()) {
        emit archiveChanged();
        return false;
    }

    if (!QFile::exists(path)) {
        qWarning() << "zim file doesn't exist:" << path;
        Settings::instance()->setZimFile({});
        emit archiveChanged();
        return false;
    }

    setBusy(true);

    m_meta.path = path;
    m_meta.fields = ZimMetaData::Title | ZimMetaData::Checksum |
                    ZimMetaData::Language | ZimMetaData::Favicon |
                    ZimMetaData::Tags;

    if (!FileModel::scanZimFile(&m_meta)) {
        m_meta.clear();
        setBusy(false);
        emit archiveChanged();
        return false;
    }

    try {
        m_archive.emplace(m_meta.path.toStdString());
    } catch (const std::exception &e) {
        qWarning() << "unable to open zim file:" << m_meta.path << e.what();
        Settings::instance()->setZimFile({});
        m_meta.clear();
        m_archive.reset();
        setBusy(false);
        emit archiveChanged();
        return false;
    }

    if (!m_archive->hasMainEntry()) {
        m_hasMainPage = false;
    } else {
        // Wikipedia ZIM returns true for hasMainPage but has empty main page
        m_hasMainPage = article("A/mainpage").has_value();
    }

    //    qDebug() << "zim file meta keys:";
    //    for (const auto &key : m_archive->getMetadataKeys()) {
    //        qDebug() << QString::fromStdString(key);
    //    }

    try {
        m_meta.tags =
            QString::fromStdString(m_archive->getMetadata("Tags")).toLower();
        // qDebug() << "archive tags:" << m_meta.tags;
        ftindex = tagExists(QStringLiteral("_ftindex"), m_meta.tags);
    } catch (const zim::EntryNotFound &e) {
        qWarning() << "archive does not have tags:" << e.what();
    }

    setBusy(false);
    emit archiveChanged();

    return true;
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

std::optional<ArticleResult> ZimServer::article(const QString &path) {
    if (m_archive) return ZimServer::article(*m_archive, path);
    return std::nullopt;
}

std::optional<ArticleResult> ZimServer::article(const zim::Archive &archive,
                                                const QString &path) {
    std::optional<zim::Item> article;

    qDebug() << "searching for path:" << path;

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
            qWarning() << "article not found";
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

void ZimServer::articleAsync(const QString &path) {
    auto parts = path.split("/");
    if (parts.length() < 2) {
        qWarning() << "bad path:" << path;
        return;
    }
    urlToAsyncGet = QStringList{parts.mid(parts.length() - 2)}.join("/");
    this->start(QThread::IdlePriority);
}

void ZimServer::requestHandler(QHttpRequest *req, QHttpResponse *resp) {
    qDebug() << "http request:" << req->url().path();

    auto parts =
        QUrl::fromPercentEncoding(req->url().path().toLocal8Bit()).split("/");
    auto uuid = parts.at(1);

    if (parts.length() < 4) {
        qWarning() << "invalid url:" << req->url();
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end();
        return;
    }

    QString path;

    if (uuid.startsWith("uuid:")) {
        uuid = uuid.right(uuid.length() - 5);
        // qDebug() << uuid;
        if (!loadZimFileByUuid(uuid)) {
            qWarning() << "unable to load zim file with uuid:" << uuid;
            resp->setHeader("Content-Length", "0");
            resp->setHeader("Connection", "close");
            resp->writeHead(500);
            resp->end();
            return;
        }
        path = parts.last() == "mainpage" ? "A/mainpage"
                                          : QStringList{parts.mid(2)}.join('/');
    } else {
        qWarning() << "invalid uuid";
        if (!m_archive) {
            qWarning() << "zim file not loaded";
            resp->setHeader("Content-Length", "0");
            resp->setHeader("Connection", "close");
            resp->writeHead(500);
            resp->end();
            return;
        }

        path = QStringList{parts.mid(1)}.join('/');
    }

    auto art = article(path);
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
        filter(&dd);
        art->content = dd.toUtf8();
    }

    resp->setHeader("Content-Length", QString::number(art->content.size()));
    resp->setHeader("Content-Type", art->mime);
    resp->setHeader("Connection", "close");
    resp->writeHead(200);
    resp->end(art->content);
}

QUrl ZimServer::localUrl(const QString &path) const {
    QUrl url{"http://localhost"};
    url.setPort(port);
    url.setPath(QString{"/uuid:%2/%3"}.arg(uuid(), path));
    return url;
}

QString ZimServer::titleFromUrl(const QUrl &url) {
    qDebug() << "searching for title of:" << url.path();

    if (!m_archive) {
        qWarning() << "zim file not loaded";
        return {};
    }

    auto parts = QUrl::fromPercentEncoding(url.path().toLocal8Bit()).split("/");
    if (parts.length() < 4) {
        qWarning() << "url is invalid:" << url;
        return {};
    }

    auto zimuuid = parts.at(1);
    if (zimuuid.startsWith("uuid:")) {
        zimuuid = zimuuid.right(zimuuid.length() - 5);
    } else {
        qWarning() << "url uuid is invalid or missing:" << url;
        return {};
    }

    auto path = QStringList{parts.mid(2)}.join("/");
    if (path == "A/mainpage") {
        return tr("Main page");
    }

    auto entries = m_archive->findByPath(ZimServer::stringQtoStd(path));
    if (entries.size() == 0) {
        qWarning() << "entry for url not found" << url;
        return {};
    }

    //    qDebug() << "entries size:" << entries.size();
    //    for (auto &e : entries) {
    //        qDebug() << ZimServer::stringStdToQ(e.getTitle());
    //    }

    return ZimServer::stringStdToQ(entries.begin()->getTitle());
}

void ZimServer::searchFullText(const QString &phrase, int maxSize,
                               QList<SearchResult> *result) const {
    zim::Searcher searcher{*m_archive};
    auto res = searcher.search(phrase.toStdString()).getResults(0, maxSize);
    for (auto &item : res) {
        result->push_back({stringStdToQ(item.getTitle()),
                           localUrl(stringStdToQ(item.getPath()))});
    }
}

void ZimServer::searchTitle(const QString &phrase, int maxSize,
                            QList<SearchResult> *result) const {
    auto res = m_archive->findByTitle(phrase.toStdString()).offset(0, maxSize);
    for (const auto &item : res) {
        result->push_back({stringStdToQ(item.getTitle()),
                           localUrl(stringStdToQ(item.getPath()))});
    }
}

QList<SearchResult> ZimServer::search(QString phrase) const {
    QList<SearchResult> result;

    if (!m_archive) {
        qWarning() << "zim file not loaded";
        return result;
    }

    phrase = phrase.trimmed();
    if (phrase.isEmpty()) {
        qWarning() << "search text is empty";
        return result;
    }

    if (ftindex &&
        Settings::instance()->getSearchMode() == Settings::FullTextSearch) {
        searchFullText(phrase, maxSearch, &result);
    }

    if (result.isEmpty()) {
        phrase[0] = phrase[0].toUpper();
        searchTitle(phrase, maxSearch / 2, &result);
        phrase[0] = phrase[0].toLower();
        searchTitle(phrase, maxSearch / 2, &result);
    }

    std::sort(result.begin(), result.end(),
              [](const SearchResult &a, const SearchResult &b) {
                  return a.title.compare(b.title, Qt::CaseInsensitive) < 0;
              });

    return result;
}

QUrl ZimServer::serverUrl() const {
    QUrl url{"http://localhost"};
    url.setPort(port);
    url.setPath(QString{"/uuid:%2/"}.arg(uuid()));
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

void ZimServer::filter(QString *data) {
    int pos;

    // <link href=\"../-/s/css_modules/ext.kartographer.link.css\"
    // rel=\"stylesheet\" type=\"text/css\">
    QRegExp rxStyle{"<link\\s[^>]*href\\s*=\\s*\"(\\S*)\"[^>]*>",
                    Qt::CaseInsensitive};
    pos = 0;
    while (pos >= 0) {
        pos = rxStyle.indexIn(*data, pos);
        if (pos >= 0) {
            auto url = fixPath(rxStyle.cap(1));
            if (auto res = article(url)) {
                data->replace(
                    pos, rxStyle.cap(0).length(),
                    "<style type=\"text/css\">" + res->content + "</style>");
            } else {
                qWarning() << "cannot get embeded style content";
            }

            ++pos;
        }
    }

    // <script src=\"../-/j/js_modules/startup.js\"></script>
    QRegExp rxScript{"<script\\s[^>]*src\\s*=\\s*\"(\\S*)\"[^>]*></script>",
                     Qt::CaseInsensitive};
    pos = 0;
    while (pos >= 0) {
        pos = rxScript.indexIn(*data, pos);
        if (pos >= 0) {
            auto url = fixPath(rxScript.cap(1));
            if (auto res = article(url)) {
                data->replace(pos, rxScript.cap(0).length(),
                              "<script type=\"text/javascript\">" +
                                  res->content + "</script>");
            } else {
                qWarning() << "cannot get embeded script content";
            }

            ++pos;
        }
    }

    // <img src=\"../I/m/AloraMountain.JPG\" data-file-width=\"3648\"
    // data-file-height=\"2736\" data-file-type=\"bitmap\" height=\"150\"
    // width=\"200\" id=\"mwBQ\">
    QRegExp rxImg{"<img\\s[^>]*src\\s*=\\s*\"(\\S*)\"[^>]*>",
                  Qt::CaseInsensitive};
    pos = 0;
    while (pos >= 0) {
        pos = rxImg.indexIn(*data, pos);
        if (pos >= 0) {
            auto url = fixPath(rxImg.cap(1));
            if (auto res = article(url)) {
                auto imgTag = rxImg.cap(0).replace(
                    rxImg.cap(1),
                    "data:" + res->mime + ";base64," + res->content.toBase64());
                data->replace(pos, rxImg.cap(0).length(), imgTag);
            } else {
                qWarning() << "cannot get embeded image content";
            }

            ++pos;
        }
    }
}
