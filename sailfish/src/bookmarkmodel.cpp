/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bookmarks.h"
#include "zimmetadata.h"
#include "bookmarkmodel.h"

BookmarkModel::BookmarkModel(QObject *parent) :
    ListModel(new BookmarkItem, parent)
{
    connect(Bookmarks::instance(), &Bookmarks::busyChanged,
            this, &BookmarkModel::bookmarksBusyChangedHandler);
    refreshModel();
}

void BookmarkModel::init()
{
    Bookmarks::instance()->init();
}

void BookmarkModel::bookmarksBusyChangedHandler()
{
    if (!Bookmarks::instance()->getBusy()) {
        refreshModel();
    }
    emit busyChanged();
}

/*bool BookmarkModel::createBookmark(const QByteArray &json)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Can not parse bookmark data!" << parseError.errorString();
        return false;
    }

    if (!doc.isObject()) {
        qWarning() << "Bookmark data is not JSON object!";
        return false;
    }

    QJsonObject obj = doc.object();

    if (!obj["url"].isString() || obj["url"].toString().isEmpty() ||
        !obj["title"].isString() || obj["title"].toString().isEmpty()) {
        qWarning() << "Bookmark data is not valid!";
        return false;
    }

    QJsonObject newObj;
    QString icon;
    if (obj["icon"].isString() &&
        (obj["icon"].toString().startsWith("http://",Qt::CaseInsensitive) ||
        obj["icon"].toString().startsWith("https://",Qt::CaseInsensitive) ||
        obj["icon"].toString().startsWith("data:image/",Qt::CaseInsensitive))) {
        icon = obj["icon"].toString();
    } else {
        icon = "icon-launcher-bookmark";
    }

    newObj.insert("favicon", icon);
    newObj.insert("url", obj["url"]);
    newObj.insert("title", obj["title"]);
    newObj.insert("hasTouchIcon", QJsonValue::fromVariant(false));

    QJsonArray newArray = readBookmarks();
    newArray.append(newObj);

    if (!writeBookmarks(newArray)) {
        qWarning() << "Can not create new bookmark!";
        return false;
    }

    return true;
}*/

void BookmarkModel::refreshModel()
{
    auto list = Bookmarks::instance()->getBookmarks();

    clear();
    for (auto b : list) {
        appendRow(new BookmarkItem(
                      b.url,
                      b.title,
                      b.url,
                      b.favicon,
                      b.zimTitle,
                      b.zimLanguage,
                      b.zimUuid,
                      b.valid
                  ));
        }
}

bool BookmarkModel::getBusy()
{
    return Bookmarks::instance()->getBusy();
}

void BookmarkModel::clear()
{
    if(rowCount()>0) removeRows(0,rowCount());
}

BookmarkItem::BookmarkItem(
                   const QString &id,
                   const QString &title,
                   const QString &url,
                   const QString &favicon,
                   const QString &zimtitle,
                   const QString &zimlang,
                   const QString &zimuuid,
                   bool valid,
                   QObject *parent) :
    ListItem(parent),
    m_id(id),
    m_title(title),
    m_url(url),
    m_icon(favicon),
    m_zimtitle(zimtitle),
    m_zimlang(zimlang),
    m_zimuuid(zimuuid),
    m_valid(valid)
{}

QHash<int, QByteArray> BookmarkItem::roleNames() const
{
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

QVariant BookmarkItem::data(int role) const
{
    switch(role) {
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
