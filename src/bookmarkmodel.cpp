/* Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bookmarkmodel.h"

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMap>
#include <QStandardPaths>
#include <QTextStream>
#include <algorithm>

#include "filemodel.h"
#include "settings.h"
#include "zimserver.h"

BookmarkModel *BookmarkModel::m_instance = nullptr;
const QString BookmarkModel::bookmarkFilename = "bookmarks.json";

BookmarkModel *BookmarkModel::instance(QObject *parent) {
    if (BookmarkModel::m_instance == nullptr) {
        BookmarkModel::m_instance = new BookmarkModel{parent};
    }

    return BookmarkModel::m_instance;
}

BookmarkModel::BookmarkModel(QObject *parent)
    : SelectableItemModel{new BookmarkItem, parent} {
    auto fm = FileModel::instance();
    connect(fm, &FileModel::busyChanged, this,
            [] { BookmarkModel::instance()->updateModel(); });
}

QList<ListItem *> BookmarkModel::makeItems() {
    QList<ListItem *> items;

    auto fm = FileModel::instance();

    if (!fm->isBusy()) {
        auto jsonlist = readBookmarks();
        for (const auto &i : jsonlist) {
            auto obj = i.toObject();

            auto url = obj["url"].toString();
            auto parts = url.split("/");
            if (parts.length() < 6) {
                qWarning() << "url is invalid";
                continue;
            }

            auto zimuuid = parts.at(3);
            if (zimuuid.startsWith("uuid:")) {
                zimuuid = zimuuid.right(zimuuid.length() - 5);
            } else {
                qWarning() << "url uuid is invalid or missing";
                continue;
            }

            auto md = fm->files.value(zimuuid);

            if (md.empty()) {
                items << new BookmarkItem{url,                      // id
                                          obj["title"].toString(),  // title
                                          url,                      // url
                                          obj.contains("favicon")
                                              ? obj["favicon"].toString()
                                              : QString{},  // icon
                                          QString(),        // zimtitle
                                          QString(),        // lang
                                          QString(),        // zimuuid
                                          false};
            } else {
                items << new BookmarkItem{url,                      // id
                                          obj["title"].toString(),  // title
                                          url,                      // url
                                          obj.contains("favicon")
                                              ? obj["favicon"].toString()
                                              : QString{},  // icon
                                          md.title,         // zimtitle
                                          md.language,      // lang
                                          zimuuid,          // zimuuid
                                          true};
            }
        }

        // Sorting
        std::sort(items.begin(), items.end(), [](ListItem *a, ListItem *b) {
            auto aa = qobject_cast<BookmarkItem *>(a);
            auto bb = qobject_cast<BookmarkItem *>(b);
            return aa->title().compare(bb->title(), Qt::CaseInsensitive) < 0;
        });
    }

    return items;
}

QJsonArray BookmarkModel::readBookmarks() {
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QFile file(dir.absoluteFilePath(bookmarkFilename));

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "cannot open bookmarks file";
        return {};
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Cannot parse bookmarks file:"
                   << parseError.errorString();
        return {};
    }

    if (!doc.isArray()) {
        qWarning() << "Bookmarks file is not JSON array";
        return {};
    }

    return doc.array();
}

bool BookmarkModel::writeBookmarks(const QJsonArray &json) {
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QFile file(dir.absoluteFilePath(bookmarkFilename));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open bookmarks file";
        return false;
    }

    QTextStream outStream(&file);
    QJsonDocument doc(json);
    outStream.setCodec("UTF-8");
    outStream.setGenerateByteOrderMark(true);
    outStream << doc.toJson();

    file.close();
    return true;
}

void BookmarkModel::addBookmark(const QString &title, const QString &url,
                                const QString &favicon) {
    auto array = readBookmarks();

    for (const auto &b : array) {
        auto obj = b.toObject();
        if (obj.contains("url") && obj.value("url").toString() == url) {
            qWarning() << "bookmark url already exists";
            emit bookmarkExists();
            return;
        }
    }

    QJsonObject obj;
    obj.insert("title", title);
    obj.insert("url", url);
    obj.insert("favicon", favicon);
    array.append(obj);

    if (writeBookmarks(array)) emit bookmarkAdded();
    updateModel();
}

void BookmarkModel::updateBookmark(const QString &oldUrl, const QString &title,
                                   const QString &url, const QString &favicon) {
    auto array = readBookmarks();

    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") && obj.value("url").toString() == oldUrl) {
            array.erase(it);
            QJsonObject nobj;
            nobj.insert("title", title);
            nobj.insert("url", url);
            nobj.insert("favicon", favicon);
            array.insert(it, nobj);

            if (writeBookmarks(array)) emit bookmarkUpdated();
            updateModel();
            return;
        }
        ++it;
    }

    qWarning() << "bookmark url doesn't exist";
}

void BookmarkModel::deleteBookmark(const QString &url) {
    auto array = readBookmarks();

    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") && obj.value("url").toString() == url) {
            array.erase(it);
            if (writeBookmarks(array)) emit bookmarkDeleted();
            updateModel();
            return;
        }
        ++it;
    }
}

QString BookmarkModel::changeUuid(const QString &url,
                                  const QString &newUuid) const {
    return newUuid.isEmpty() ? url
                             : QString("http://localhost:%1/%2/%3")
                                   .arg(ZimServer::port)
                                   .arg("uuid:" + newUuid, articleUrl(url));
}

bool BookmarkModel::validateUrl(const QString &url) {
    auto parts = url.split("/");
    if (parts.length() < 6) {
        qWarning() << "url is invalid";
        return false;
    }

    auto uuid = parts.at(3);
    if (uuid.startsWith("uuid:")) {
        return true;
    }

    return false;
}

QString BookmarkModel::articleUrl(QString url) {
    auto parts = url.split("/");
    if (parts.length() < 2) {
        qWarning() << "url is invalid";
    } else {
        url = QStringList(parts.mid(parts.length() - 2)).join("/");
    }

    return url;
}

BookmarkItem::BookmarkItem(const QString &id, const QString &title,
                           const QString &url, const QString &favicon,
                           const QString &zimtitle, const QString &zimlang,
                           const QString &zimuuid, bool valid, QObject *parent)
    : SelectableItem(parent),
      m_id(id),
      m_title(title),
      m_url(url),
      m_icon(favicon),
      m_zimtitle(zimtitle),
      m_zimlang(zimlang),
      m_zimuuid(zimuuid),
      m_valid(valid) {}

QHash<int, QByteArray> BookmarkItem::roleNames() const {
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[TitleRole] = "title";
    names[UrlRole] = "url";
    names[IconRole] = "favicon";
    names[ValidRole] = "valid";
    names[ZimTitleRole] = "zimTitle";
    names[ZimLangRole] = "zimLanguage";
    names[ZimUuidRole] = "zimUuid";
    return names;
}

QVariant BookmarkItem::data(int role) const {
    switch (role) {
        case IdRole:
            return id();
        case TitleRole:
            return title();
        case UrlRole:
            return url();
        case IconRole:
            return icon();
        case ValidRole:
            return valid();
        case ZimTitleRole:
            return zimtitle();
        case ZimLangRole:
            return zimlang();
        case ZimUuidRole:
            return zimuuid();
        default:
            return QVariant();
    }
}
