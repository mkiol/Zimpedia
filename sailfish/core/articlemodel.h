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

#include <QString>
#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QVariant>
#include <QHash>

#include "itemmodel.h"

class ArticleItem : public SelectableItem
{
    Q_OBJECT

public:
    enum Roles {
        TitleRole = Qt::DisplayRole,
        IdRole = Qt::UserRole,
        UrlRole
    };

public:
    ArticleItem(QObject *parent = nullptr): SelectableItem(parent) {}
    explicit ArticleItem(const QString &id,
                      const QString &title,
                      const QString &url,
                      QObject *parent = nullptr);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_id; }
    inline QString title() const { return m_title; }
    inline QString url() const { return m_url; }

private:
    QString m_id;
    QString m_title;
    QString m_url;
};

class ArticleModel : public SelectableItemModel
{
    Q_OBJECT
public:
    static ArticleModel* instance(QObject *parent = nullptr);

private:
    static ArticleModel* m_instance;

    explicit ArticleModel(QObject *parent = nullptr);

    QList<ListItem*> makeItems();
};

#endif // ARTICLEMODEL_H
