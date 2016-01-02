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

/*void ArticleModel::init(const QString &searchString)
{
    this->searchString = searchString;
    init();
}

void ArticleModel::init()
{
    if(rowCount()>0) removeRows(0,rowCount());
    createItems(this->searchString);
}

void ArticleModel::createItems(const QString &searchString)
{
    QList<DatabaseManager::Stream> list;
    Settings *s = Settings::instance();
    int mode = s->getViewMode();
    switch (mode) {
    case 0:
        // View mode: Tabs->Feeds->Entries
        list = _db->readStreamsByTab(search);
        break;
    case 1:
        // View mode: Tabs->Entries
        list = _db->readStreamsByTab(search);
        break;
    case 2:
        // View mode: Feeds->Entries
        list = _db->readStreamsByDashboard(s->getDashboardInUse());
        break;
    case 3:
        // View mode: Entries
    case 4:
        // View mode: Saved
    case 5:
        // View mode: Slow
        qWarning() << "Error: This should never happened";
        return;
    }

    QRegExp re("<[^>]*>");
    QList<DatabaseManager::Stream>::iterator i = list.begin();
    while( i != list.end() ) {
        appendRow(new ArticleItem((*i).id,
                              (*i).title.remove(re),
                              (*i).content,
                              (*i).link,
                              (*i).query,
                              (*i).icon,
                              0,
                              _db->countEntriesUnreadByStream((*i).id),
                              _db->countEntriesReadByStream((*i).id),
                              (*i).saved,
                              _db->countEntriesFreshByStream((*i).id)
                             ));
        ++i;
    }

//#ifdef BB10
    // Dummy row as workaround!
    if (list.count()>0)
        appendRow(new ArticleItem("last","","","","","",0,0,0,0,0));
//#endif
}*/


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
