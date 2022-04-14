/* Copyright (C) 2018 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QList>
#include <QObject>
#include <QString>
#include <QThread>
#include <memory>

#include "listmodel.h"

class ItemModel;

class ItemWorker : public QThread {
    Q_OBJECT
    friend class ItemModel;
    friend class SelectableItemModel;

   public:
    explicit ItemWorker(ItemModel *model, const QString &data = {});
    ~ItemWorker() override;

   private:
    QString data;
    ItemModel *model;
    QList<ListItem *> items;
    void run() override;
};

class ItemModel : public ListModel {
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    friend class ItemWorker;

   public:
    explicit ItemModel(ListItem *prototype, QObject *parent = nullptr);
    inline int count() const { return m_list.size(); };
    inline bool busy() const { return m_busy; }
    virtual void updateModel(const QString &data = {});

   signals:
    void busyChanged();
    void countChanged();

   protected:
    std::unique_ptr<ItemWorker> m_worker;
    virtual void workerDone();
    virtual QList<ListItem *> makeItems() = 0;
    virtual void postMakeItems(const QList<ListItem *> &items);
    virtual void clear();
    void setBusy(bool busy);

   private:
    bool m_busy = true;
};

class SelectableItem : public ListItem {
    Q_OBJECT

   public:
    SelectableItem(QObject *parent = nullptr) : ListItem{parent} {}
    inline bool selected() const { return m_selected; }
    inline bool selectable() const { return m_selectable; }
    void setSelected(bool value);

   protected:
    bool m_selectable = true;

   private:
    bool m_selected = false;
};

class SelectableItemModel : public ItemModel {
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY selectedCountChanged)
    Q_PROPERTY(
        int selectableCount READ selectableCount NOTIFY selectableCountChanged)

   public:
    explicit SelectableItemModel(SelectableItem *prototype,
                                 QObject *parent = nullptr);
    void setFilter(const QString &filter);
    void setFilterNoUpdate(const QString &filter);
    inline QString filter() const { return m_filter; }
    inline int selectedCount() const { return m_selectedCount; }
    inline int selectableCount() const { return m_selectableCount; }

    Q_INVOKABLE void setSelected(int index, bool value);
    Q_INVOKABLE void setAllSelected(bool value);
    Q_INVOKABLE virtual QVariantList selectedItems();

   public slots:
    virtual void updateModel(const QString &data = {}) override;

   signals:
    void filterChanged();
    void selectedCountChanged();
    void selectableCountChanged();

   protected slots:
    virtual void workerDone() override;

   private:
    QString m_filter;
    int m_selectedCount = 0;
    int m_selectableCount = 0;
    void clear() override;
    void postMakeItems(const QList<ListItem *> &items) override;
};

#endif  // ITEMMODEL_H
