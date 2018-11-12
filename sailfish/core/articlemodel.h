/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef ARTICLEMODEL_H
#define ARTICLEMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <QModelIndex>

#include "listmodel.h"

class ArticleItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        UidRole = Qt::UserRole,
        TitleRole = Qt::DisplayRole,
        UrlRole
    };

public:
    ArticleItem(QObject *parent = 0): ListItem(parent) {}
    explicit ArticleItem(const QString &uid,
                      const QString &title,
                      const QString &url,
                      QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_uid; }
    inline QString uid() const { return m_uid; }
    inline QString title() const { return m_title; }
    inline QString url() const { return m_url; }

private:
    QString m_uid;
    QString m_title;
    QString m_url;
};

class ArticleModel : public ListModel
{
    Q_OBJECT

public:
    explicit ArticleModel(QObject *parent = 0);
    void clear();
};

#endif // ARTICLEMODEL_H
