/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
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
#include <QUrl>
#include <QVariant>

#include "itemmodel.h"

struct Bookmark {
    QString title;
    QUrl url;
    QString favicon;
    QString zimUuid;
    QString zimTitle;
    QString zimLang;
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
        ZimUuidRole
    };

   public:
    BookmarkItem(QObject *parent = nullptr) : SelectableItem(parent) {}
    explicit BookmarkItem(const QString &id, const QString &title,
                          const QUrl &url, const QUrl &icon,
                          const QString &zimtitle, const QString &zimuuid,
                          bool valid, QObject *parent = 0);
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    inline QString id() const override { return m_id; }
    inline auto title() const { return m_title; }
    inline auto url() const { return m_url; }
    inline auto icon() const { return m_icon; }
    inline auto zimtitle() const { return m_zimtitle; }
    inline auto zimuuid() const { return m_zimuuid; }
    inline auto valid() const { return m_valid; }

   private:
    QString m_id;
    QString m_title;
    QUrl m_url;
    QUrl m_icon;
    QString m_zimtitle;
    QString m_zimuuid;
    bool m_valid;
};

class BookmarkModel : public SelectableItemModel {
    Q_OBJECT

   public:
    static BookmarkModel *instance(QObject *parent = nullptr);

    Q_INVOKABLE void addBookmark(const QString &title, const QUrl &url);
    Q_INVOKABLE void updateBookmark(const QUrl &oldUrl, const QString &title,
                                    const QUrl &url);
    Q_INVOKABLE void deleteBookmark(const QUrl &url);
    Q_INVOKABLE bool validateUrl(const QUrl &url);

   signals:
    void bookmarkAdded();
    void bookmarkDeleted();
    void bookmarkUpdated();
    void bookmarkExists();

   private:
    static BookmarkModel *m_instance;
    static const QString bookmarkFilename;

    explicit BookmarkModel(QObject *parent = nullptr);
    QList<ListItem *> makeItems() override;
    static QJsonArray readBookmarks();
    static bool writeBookmarks(const QJsonArray &json);
};

#endif  // BOOKMARKMODEL_H
