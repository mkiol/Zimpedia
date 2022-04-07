/* Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QByteArray>
#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QVariant>

#include "itemmodel.h"

struct Bookmark {
    QString title;
    QString url;
    QString favicon;
    QString zimUuid;
    QString zimTitle;
    QString zimLanguage;
    bool valid;
};

class BookmarkItem : public SelectableItem {
    Q_OBJECT

   public:
    enum Roles {
        TitleRole = Qt::DisplayRole,
        IdRole = Qt::UserRole,
        UrlRole,
        IconRole,
        ValidRole,
        ZimTitleRole,
        ZimLangRole,
        ZimUuidRole
    };

   public:
    BookmarkItem(QObject *parent = nullptr) : SelectableItem(parent) {}
    explicit BookmarkItem(const QString &id, const QString &title,
                          const QString &url, const QString &favicon,
                          const QString &zimtitle, const QString &zimlang,
                          const QString &zimuuid, bool valid,
                          QObject *parent = 0);
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    inline QString id() const override { return m_id; }
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
};

class BookmarkModel : public SelectableItemModel {
    Q_OBJECT

   public:
    static BookmarkModel *instance(QObject *parent = nullptr);

    Q_INVOKABLE void addBookmark(const QString &title, const QString &url,
                                 const QString &favicon);
    Q_INVOKABLE void updateBookmark(const QString &oldUrl, const QString &title,
                                    const QString &url, const QString &favicon);
    Q_INVOKABLE void deleteBookmark(const QString &url);
    Q_INVOKABLE QString changeUuid(const QString &url,
                                   const QString &newUuid) const;
    Q_INVOKABLE bool validateUrl(const QString &url);

   signals:
    void bookmarkAdded();
    void bookmarkDeleted();
    void bookmarkUpdated();
    void bookmarkExists();

   private:
    static BookmarkModel *m_instance;
    static const QString bookmarkFilename;

    explicit BookmarkModel(QObject *parent = nullptr);
    static QString articleUrl(QString url);
    QList<ListItem *> makeItems() override;
    QJsonArray readBookmarks();
    bool writeBookmarks(const QJsonArray &json);
};

#endif  // BOOKMARKMODEL_H
