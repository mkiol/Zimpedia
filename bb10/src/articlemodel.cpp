/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "articlemodel.h"

ArticleModel::ArticleModel(QObject *parent) :
    ListModel(new ArticleItem, parent)
{
}

void ArticleModel::clear()
{
    if(rowCount()>0) removeRows(0,rowCount());
}

ArticleItem::ArticleItem(const QString &uid,
                   const QString &title,
                   const QString &url,
                   QObject *parent) :
    ListItem(parent),
    m_uid(uid),
    m_title(title),
    m_url(url)
{}

QHash<int, QByteArray> ArticleItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[UidRole] = "uid";
    names[TitleRole] = "title";
    names[UrlRole] = "url";
    return names;
}

QVariant ArticleItem::data(int role) const
{
    switch(role) {
    case UidRole:
        return uid();
    case TitleRole:
        return title();
    case UrlRole:
        return url();
    default:
        return QVariant();
    }
}
