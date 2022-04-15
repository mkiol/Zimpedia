/* Copyright (C) 2018 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "itemmodel.h"

#include <QDebug>

ItemWorker::ItemWorker(ItemModel *model, const QString &data)
    : data{data}, model{model} {}

ItemWorker::~ItemWorker() {
    requestInterruption();
    wait();
}

void ItemWorker::run() {
    if (model->m_delayedStart) QThread::msleep(startDelay);
    if (!isInterruptionRequested()) items = model->makeItems();
    if (!isInterruptionRequested()) model->postMakeItems(items);
}

ItemModel::ItemModel(ListItem *prototype, QObject *parent)
    : ListModel{prototype, parent} {}

void ItemModel::updateModel(const QString &data) {
    if (m_worker && m_worker->isRunning()) {
        m_worker->requestInterruption();
        return;
    }

    setBusy(true);
    m_worker = std::make_unique<ItemWorker>(this, data);
    connect(m_worker.get(), &QThread::finished, this, &ItemModel::workerDone);
    m_worker->start(QThread::IdlePriority);
}

void ItemModel::postMakeItems(const QList<ListItem *> &) {}

void ItemModel::clear() {
    if (m_list.length() == 0) return;
    removeRows(0, rowCount());
    emit countChanged();
}

void ItemModel::workerDone() {
    auto *worker = qobject_cast<ItemWorker *>(sender());
    if (worker) {
        int old_l = m_list.length();
        if (m_list.length() != 0) removeRows(0, rowCount());
        if (!worker->items.isEmpty()) appendRows(worker->items);
        if (old_l != m_list.length()) emit countChanged();
        m_worker.reset();
    }

    setBusy(false);
}

void ItemModel::setBusy(bool busy) {
    if (busy != m_busy) {
        m_busy = busy;
        emit busyChanged();
    }
}

void SelectableItem::setSelected(bool value) {
    if (m_selected != value) {
        m_selected = value;
        emit dataChanged();
    }
}

SelectableItemModel::SelectableItemModel(SelectableItem *prototype,
                                         bool delayedStart, QObject *parent)
    : ItemModel{prototype, parent} {
    m_delayedStart = delayedStart;
}

void SelectableItemModel::clear() {
    ItemModel::clear();
    m_selectedCount = 0;
    emit selectedCountChanged();
}

void SelectableItemModel::postMakeItems(const QList<ListItem *> &items) {
    int selectable_count = 0;
    int selected_count = 0;
    for (const auto *item : items) {
        const auto *si = qobject_cast<const SelectableItem *>(item);
        if (si->selectable()) selectable_count++;
        if (si->selected()) selected_count++;
    }

    if (selectable_count != m_selectableCount) {
        m_selectableCount = selectable_count;
        emit selectableCountChanged();
    }
    if (selected_count != m_selectedCount) {
        m_selectedCount = selected_count;
        emit selectedCountChanged();
    }
}

void SelectableItemModel::setFilter(const QString &filter) {
    if (m_filter != filter) {
        m_filter = filter;
        emit filterChanged();
        updateModel();
    }
}

void SelectableItemModel::setFilterNoUpdate(const QString &filter) {
    if (m_filter != filter) {
        m_filter = filter;
        emit filterChanged();
    }
}

void SelectableItemModel::setSelected(int index, bool value) {
    if (index >= m_list.length()) {
        qWarning() << "index is invalid";
        return;
    }

    auto *item = qobject_cast<SelectableItem *>(m_list.at(index));

    if (item->selectable()) {
        bool cvalue = item->selected();

        if (cvalue != value) {
            item->setSelected(value);

            if (value)
                m_selectedCount++;
            else
                m_selectedCount--;

            emit selectedCountChanged();
        }
    }
}

void SelectableItemModel::setAllSelected(bool value) {
    if (m_list.isEmpty()) return;

    int c = m_selectedCount;
    foreach (auto *li, m_list) {
        auto *item = qobject_cast<SelectableItem *>(li);
        if (item->selectable()) {
            bool cvalue = item->selected();
            if (cvalue != value) {
                item->setSelected(value);
                if (value)
                    m_selectedCount++;
                else
                    m_selectedCount--;
            }
        }
    }

    if (c != m_selectedCount) emit selectedCountChanged();
}

QVariantList SelectableItemModel::selectedItems() { return {}; }

void SelectableItemModel::workerDone() {
    if (m_worker && m_worker->data != m_filter) {
        updateModel(m_filter);
    } else {
        ItemModel::workerDone();
    }
}

void SelectableItemModel::updateModel(const QString &) {
    setAllSelected(false);
    ItemModel::updateModel(m_filter);
}
