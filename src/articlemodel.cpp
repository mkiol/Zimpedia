/* Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "articlemodel.h"

#include "zimserver.h"

ArticleModel *ArticleModel::m_instance = nullptr;

ArticleModel *ArticleModel::instance(QObject *parent) {
    if (ArticleModel::m_instance == nullptr) {
        ArticleModel::m_instance = new ArticleModel{parent};
    }

    return ArticleModel::m_instance;
}

ArticleModel::ArticleModel(QObject *parent)
    : SelectableItemModel{new ArticleItem, parent} {}

QList<ListItem *> ArticleModel::makeItems() {
    QList<ListItem *> items;

    auto results = ZimServer::instance()->search(getFilter());

    if (results.isEmpty()) return items;

    items.reserve(results.size());
    for (int i = 0; i < results.size(); ++i) {
        const auto &item = results.at(i);
        items.push_back(
            new ArticleItem{QString::number(i), item.title, item.url});
    }

    return items;
}

ArticleItem::ArticleItem(const QString &id, const QString &title,
                         const QUrl &url, QObject *parent)
    : SelectableItem(parent), m_id{id}, m_title{title}, m_url{url} {}

QHash<int, QByteArray> ArticleItem::roleNames() const {
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[TitleRole] = "title";
    names[UrlRole] = "url";
    return names;
}

QVariant ArticleItem::data(int role) const {
    switch (role) {
        case IdRole:
            return id();
        case TitleRole:
            return title();
        case UrlRole:
            return url();
        default:
            return {};
    }
}
