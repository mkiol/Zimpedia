/**
 *  Copyright (c) 2013, Kläralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *    - Neither the name of Kläralvdalens Datakonsult AB nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "abstractitemmodel.hpp"

class AbstractItemModel::Private
{
public:
    Private(AbstractItemModel *qq)
        : q(qq)
        , m_sourceModel(0)
    {
    }

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void headerDataChanged(Qt::Orientation orientation, int first, int last);
    void layoutAboutToBeChanged();
    void layoutChanged();
    void rowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void rowsInserted(const QModelIndex &parent, int first, int last);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void rowsRemoved(const QModelIndex &parent, int first, int last);
    void columnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void columnsInserted(const QModelIndex &parent, int first, int last);
    void columnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void columnsRemoved(const QModelIndex &parent, int first, int last);
    void modelAboutToBeReset();
    void modelReset();
    void rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row );
    void columnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    void columnsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int column );

    QModelIndex indexForPath(const QVariantList &indexPath) const;
    QVariantList pathForIndex(const QModelIndex &index) const;

    AbstractItemModel *q;
    QPointer<QAbstractItemModel> m_sourceModel;
    QString m_itemTypeRole;

    QHash<int, QByteArray> m_roleNames;
    QHash<QByteArray, int> m_reverseRoleNames;
};


void AbstractItemModel::Private::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    // we support only one column at the moment
    const int column = topLeft.column();
    const QModelIndex parentIndex = topLeft.parent();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        const QModelIndex index = m_sourceModel->index(row, column, parentIndex);
        const QVariantList indexPath = pathForIndex(index);
        emit q->itemUpdated(indexPath);
    }
}

void AbstractItemModel::Private::headerDataChanged(Qt::Orientation orientation, int first, int last)
{
    // nothing to do at the moment
    Q_UNUSED(orientation)
    Q_UNUSED(first)
    Q_UNUSED(last)
}

void AbstractItemModel::Private::layoutAboutToBeChanged()
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::layoutChanged()
{
    emit q->itemsChanged(bb::cascades::DataModelChangeType::Init);
}

void AbstractItemModel::Private::rowsAboutToBeInserted(const QModelIndex&, int, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::rowsInserted(const QModelIndex &parent, int first, int last)
{
    const QVariantList parentPath = pathForIndex(parent);
    for (int pos = first; pos <= last; pos++) {
        QVariantList indexPath = parentPath;
        emit q->itemAdded(indexPath << pos);
    }
    emit q->countChanged();
}

void AbstractItemModel::Private::rowsAboutToBeRemoved(const QModelIndex&, int, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::rowsRemoved(const QModelIndex &parent, int first, int last)
{
    const QVariantList parentPath = pathForIndex(parent);
    for (int pos = first; pos <= last; pos++) {
        QVariantList indexPath = parentPath;
        emit q->itemRemoved(indexPath << pos);
    }
    emit q->countChanged();
}

void AbstractItemModel::Private::columnsAboutToBeInserted(const QModelIndex&, int, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::columnsInserted(const QModelIndex &parent, int first, int last)
{
    //TODO: support column selection?
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)
}

void AbstractItemModel::Private::columnsAboutToBeRemoved(const QModelIndex&, int, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::columnsRemoved(const QModelIndex &parent, int first, int last)
{
    //TODO: support column selection?
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)
}

void AbstractItemModel::Private::modelAboutToBeReset()
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::modelReset()
{
    emit q->itemsChanged(bb::cascades::DataModelChangeType::Init);
}

void AbstractItemModel::Private::rowsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)
{
    emit q->itemsChanged(bb::cascades::DataModelChangeType::Init);
}

void AbstractItemModel::Private::columnsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int)
{
    // nothing to do at the moment
}

void AbstractItemModel::Private::columnsMoved(const QModelIndex&, int, int, const QModelIndex&, int)
{
    emit q->itemsChanged(bb::cascades::DataModelChangeType::Init);
}

QModelIndex AbstractItemModel::Private::indexForPath(const QVariantList &indexPath) const
{
    Q_ASSERT(m_sourceModel);

    QModelIndex index;
    QModelIndex parentIndex;
    for (int i = 0; i < indexPath.count(); ++i) {
        index = m_sourceModel->index(indexPath[i].toInt(), 0, parentIndex);
        parentIndex = index;
    }

    return index;
}

QVariantList AbstractItemModel::Private::pathForIndex(const QModelIndex &index) const
{
    QVariantList indexPath;

    QModelIndex currentIndex = index;
    while (currentIndex.isValid()) {
        indexPath.prepend(currentIndex.row());
        currentIndex = currentIndex.parent();
    }

    return indexPath;
}


AbstractItemModel::AbstractItemModel(QObject *parent)
    : bb::cascades::DataModel(parent)
    , d(new Private(this))
{
}

AbstractItemModel::~AbstractItemModel()
{
    delete d;
}

QAbstractItemModel* AbstractItemModel::sourceModel() const
{
    return d->m_sourceModel;
}

void AbstractItemModel::resetSourceModel()
{
    //emit itemsChanged(bb::cascades::DataModelChangeType::Init);
    setSourceModel(d->m_sourceModel);
}

void AbstractItemModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    /*if (d->m_sourceModel == sourceModel)
       return;*/

    if (d->m_sourceModel) {
        disconnect(d->m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
        disconnect(d->m_sourceModel, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged(Qt::Orientation,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(layoutAboutToBeChanged()), this, SLOT(layoutAboutToBeChanged()));
        disconnect(d->m_sourceModel, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));
        disconnect(d->m_sourceModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(rowsAboutToBeInserted(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(columnsAboutToBeInserted(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(columnsInserted(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(columnsAboutToBeRemoved(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(columnsRemoved(QModelIndex,int,int)));
        disconnect(d->m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SLOT(modelAboutToBeReset()));
        disconnect(d->m_sourceModel, SIGNAL(modelReset()), this, SLOT(modelReset()));
        disconnect(d->m_sourceModel, SIGNAL(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)));
        disconnect(d->m_sourceModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rowsMoved(QModelIndex,int,int,QModelIndex,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(columnsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)));
        disconnect(d->m_sourceModel, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(columnsMoved(QModelIndex,int,int,QModelIndex,int)));
    }

    d->m_sourceModel = sourceModel;

    if (d->m_sourceModel) {
        connect(d->m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
        connect(d->m_sourceModel, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(headerDataChanged(Qt::Orientation,int,int)));
        connect(d->m_sourceModel, SIGNAL(layoutAboutToBeChanged()), this, SLOT(layoutAboutToBeChanged()));
        connect(d->m_sourceModel, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));
        connect(d->m_sourceModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(rowsAboutToBeInserted(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(columnsAboutToBeInserted(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(columnsInserted(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(columnsAboutToBeRemoved(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(columnsRemoved(QModelIndex,int,int)));
        connect(d->m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SLOT(modelAboutToBeReset()));
        connect(d->m_sourceModel, SIGNAL(modelReset()), this, SLOT(modelReset()));
        connect(d->m_sourceModel, SIGNAL(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)));
        connect(d->m_sourceModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rowsMoved(QModelIndex,int,int,QModelIndex,int)));
        connect(d->m_sourceModel, SIGNAL(columnsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(columnsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)));
        connect(d->m_sourceModel, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(columnsMoved(QModelIndex,int,int,QModelIndex,int)));

        d->m_roleNames = d->m_sourceModel->roleNames();

        // update reverse role names
        d->m_reverseRoleNames.clear();
        QHashIterator<int, QByteArray> it(d->m_roleNames);
        while (it.hasNext()) {
            it.next();
            d->m_reverseRoleNames.insert(it.value(), it.key());
        }

    } else {
        d->m_roleNames.clear();
        d->m_reverseRoleNames.clear();
    }

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
    emit sourceModelChanged();
}

QString AbstractItemModel::itemTypeRole() const
{
    return d->m_itemTypeRole;
}

void AbstractItemModel::setItemTypeRole(const QString &roleName)
{
    if (d->m_itemTypeRole == roleName)
        return;

    d->m_itemTypeRole = roleName;
    emit itemTypeRoleChanged();

    emit itemsChanged(bb::cascades::DataModelChangeType::Init);
}

int AbstractItemModel::childCount(const QVariantList &indexPath)
{
    if (!d->m_sourceModel)
        return 0;

    return d->m_sourceModel->rowCount(d->indexForPath(indexPath));
}

bool AbstractItemModel::hasChildren(const QVariantList &indexPath)
{
    if (!d->m_sourceModel)
        return false;

    return d->m_sourceModel->hasChildren(d->indexForPath(indexPath));
}

QString AbstractItemModel::itemType(const QVariantList &indexPath)
{
    if (!d->m_sourceModel)
        return QString();

    if (d->m_itemTypeRole.isEmpty())
        return QString();

    return d->m_sourceModel->data(d->indexForPath(indexPath),
                                  d->m_reverseRoleNames.value(d->m_itemTypeRole.toUtf8())).toString();
}

QVariant AbstractItemModel::data(const QVariantList &indexPath)
{
    if (!d->m_sourceModel)
        return QVariant();

    QVariantMap result;

    const QModelIndex index = d->indexForPath(indexPath);

    QHashIterator<int, QByteArray> it(d->m_roleNames);
    while (it.hasNext()) {
        it.next();

        result[it.value()] = index.data(it.key());
    }

    return result;
}

void AbstractItemModel::fetchMore(const QVariantList &indexPath)
{
    if (!d->m_sourceModel)
        return;

    const QModelIndex index = d->indexForPath(indexPath);

    if (d->m_sourceModel->canFetchMore(index))
        d->m_sourceModel->fetchMore(index);
}

// Only for QAbstractListModel
int AbstractItemModel::readCount()
{
    if (!d->m_sourceModel)
        return 0;

    return d->m_sourceModel->rowCount();
}

#include "moc_abstractitemmodel.cpp"
