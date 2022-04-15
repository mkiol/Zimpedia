/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
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
    : SelectableItemModel{new BookmarkItem, false, parent} {
    connect(FileModel::instance(), &FileModel::busyChanged, this,
            [] { BookmarkModel::instance()->updateModel(); });
}

QList<ListItem *> BookmarkModel::makeItems() {
    QList<ListItem *> items;

    if (FileModel::instance()->busy()) return items;

    auto jsonlist = readBookmarks();
    for (const auto &i : jsonlist) {
        auto obj = i.toObject();
        auto url = QUrl{obj["url"].toString()};
        const auto &[uuid, path] = ZimServer::parseUrl(url);
        if (uuid.isEmpty() || path.isEmpty()) {
            qWarning() << "invalid url:" << url;
            continue;
        }

        if (FileModel::instance()->uuidExists(uuid)) {
            const auto &meta = FileModel::instance()->uuidToMeta(uuid);
            items.push_back(new BookmarkItem{
                /*id=*/url.toString(),
                /*title=*/obj["title"].toString(),
                /*url=*/url,
                /*icon=*/meta.icon,
                /*zimtitle=*/meta.language.isEmpty()
                    ? meta.title
                    : QString{"%1 · %2"}.arg(meta.title, meta.language),
                /*zimuuid=*/uuid, /*valid=*/true});
        } else {
            items.push_back(new BookmarkItem{/*id=*/url.toString(),
                                             /*title=*/obj["title"].toString(),
                                             /*url=*/url,
                                             /*icon=*/{},
                                             /*zimtitle=*/{},
                                             /*zimuuid=*/{},
                                             /*valid=*/false});
        }
    }

    std::sort(items.begin(), items.end(), [](ListItem *a, ListItem *b) {
        auto *aa = qobject_cast<BookmarkItem *>(a);
        auto *bb = qobject_cast<BookmarkItem *>(b);
        return aa->title().compare(bb->title(), Qt::CaseInsensitive) < 0;
    });

    return items;
}

QJsonArray BookmarkModel::readBookmarks() {
    QDir dir{QStandardPaths::writableLocation(QStandardPaths::DataLocation)};
    QFile file{dir.absoluteFilePath(bookmarkFilename)};

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "cannot open bookmarks file";
        return {};
    }

    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(file.readAll(), &parseError);
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
    QDir dir{QStandardPaths::writableLocation(QStandardPaths::DataLocation)};
    QFile file{dir.absoluteFilePath(bookmarkFilename)};

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open bookmarks file";
        return false;
    }

    QTextStream outStream{&file};
    QJsonDocument doc{json};
    outStream.setCodec("UTF-8");
    outStream.setGenerateByteOrderMark(true);
    outStream << doc.toJson();

    return true;
}

void BookmarkModel::addBookmark(const QString &title, const QUrl &url) {
    auto array = readBookmarks();

    for (const auto &b : array) {
        auto obj = b.toObject();
        if (obj.contains("url") &&
            obj.value("url").toString() == url.toString()) {
            qWarning() << "bookmark url already exists";
            emit bookmarkExists();
            return;
        }
    }

    QJsonObject obj;
    obj.insert("title", title);
    obj.insert("url", url.toString());
    array.append(obj);

    if (writeBookmarks(array)) emit bookmarkAdded();
    updateModel();
}

void BookmarkModel::updateBookmark(const QUrl &oldUrl, const QString &title,
                                   const QUrl &url) {
    auto array = readBookmarks();

    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") &&
            obj.value("url").toString() == oldUrl.toString()) {
            array.erase(it);
            QJsonObject nobj;
            nobj.insert("title", title);
            nobj.insert("url", url.toString());
            array.insert(it, nobj);

            if (writeBookmarks(array)) emit bookmarkUpdated();
            updateModel();
            return;
        }
        ++it;
    }

    qWarning() << "bookmark url doesn't exist";
}

void BookmarkModel::deleteBookmark(const QUrl &url) {
    auto array = readBookmarks();

    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") &&
            obj.value("url").toString() == url.toString()) {
            array.erase(it);
            if (writeBookmarks(array)) emit bookmarkDeleted();
            updateModel();
            return;
        }
        ++it;
    }
}

bool BookmarkModel::validateUrl(const QUrl &url) {
    const auto &[uuid, path] = ZimServer::parseUrl(url);
    return !uuid.isEmpty() && !path.isEmpty();
}

BookmarkItem::BookmarkItem(const QString &id, const QString &title,
                           const QUrl &url, const QUrl &icon,
                           const QString &zimtitle, const QString &zimuuid,
                           bool valid, QObject *parent)
    : SelectableItem(parent),
      m_id(id),
      m_title(title),
      m_url(url),
      m_icon(icon),
      m_zimtitle(zimtitle),
      m_zimuuid(zimuuid),
      m_valid(valid) {}

QHash<int, QByteArray> BookmarkItem::roleNames() const {
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[TitleRole] = "title";
    names[UrlRole] = "url";
    names[IconRole] = "icon";
    names[ValidRole] = "valid";
    names[ZimTitleRole] = "zimTitle";
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
        case ZimUuidRole:
            return zimuuid();
        default:
            return {};
    }
}
