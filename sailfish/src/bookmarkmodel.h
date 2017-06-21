/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QHash>

#include "listmodel.h"

class BookmarkItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole = Qt::UserRole + 2,
        UrlRole = Qt::UserRole + 3,
        IconRole = Qt::UserRole + 4,
        ValidRole = Qt::UserRole + 5,
        ZimTitleRole = Qt::UserRole + 6,
        ZimLangRole = Qt::UserRole + 7,
        ZimUuidRole = Qt::UserRole + 8
    };

public:
    BookmarkItem(QObject *parent = 0): ListItem(parent), busy(false) {}
    explicit BookmarkItem(
                      const QString &id,
                      const QString &title,
                      const QString &url,
                      const QString &favicon,
                      const QString &zimtitle,
                      const QString &zimlang,
                      const QString &zimuuid,
                      bool valid,
                      QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_id; }
    inline QString title() const { return m_title; }
    inline QString url() const { return m_url; }
    inline QString icon() const { return m_icon; }
    inline QString zimtitle() const { return m_zimtitle; }
    inline QString zimlang() const { return m_zimlang; }
    inline QString zimuuid() const { return m_zimuuid; }
    inline bool valid() const { return m_valid; }

private:
    QString m_id;
    QString m_title;
    QString m_url;
    QString m_icon;
    QString m_zimtitle;
    QString m_zimlang;
    QString m_zimuuid;
    bool m_valid;
    bool busy;
};

class BookmarkModel : public ListModel
{
    Q_OBJECT
    Q_PROPERTY (bool busy READ getBusy NOTIFY busyChanged)

public:
    explicit BookmarkModel(QObject *parent = 0);
    Q_INVOKABLE void refreshModel();
    void clear();
    bool getBusy();

signals:
    void busyChanged();

private slots:
    void bookmarksBusyChangedHandler();

private:
    bool busy;

    void init();
};

#endif // BOOKMARKMODEL_H
