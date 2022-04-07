/* Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ARTICLEMODEL_H
#define ARTICLEMODEL_H

#include <QByteArray>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QString>
#include <QUrl>
#include <QVariant>

#include "itemmodel.h"

class ArticleItem : public SelectableItem {
    Q_OBJECT

   public:
    enum Roles { TitleRole = Qt::DisplayRole, IdRole = Qt::UserRole, UrlRole };

   public:
    ArticleItem(QObject *parent = nullptr) : SelectableItem{parent} {}
    explicit ArticleItem(const QString &id, const QString &title,
                         const QUrl &url, QObject *parent = nullptr);
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    inline QString id() const override { return m_id; }
    inline QString title() const { return m_title; }
    inline QUrl url() const { return m_url; }

   private:
    QString m_id;
    QString m_title;
    QUrl m_url;
};

class ArticleModel : public SelectableItemModel {
    Q_OBJECT

   public:
    static ArticleModel *instance(QObject *parent = nullptr);
    Q_INVOKABLE void refresh() { updateModel(); }

   private:
    static ArticleModel *m_instance;

    explicit ArticleModel(QObject *parent = nullptr);

    QList<ListItem *> makeItems() override;
};

#endif  // ARTICLEMODEL_H
