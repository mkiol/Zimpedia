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

#ifndef ABSTRACTITEMMODEL_HPP
#define ABSTRACTITEMMODEL_HPP

#include <bb/cascades/DataModel>

#include <QAbstractItemModel>

class AbstractItemModel : public bb::cascades::DataModel
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel *sourceModel READ sourceModel WRITE setSourceModel NOTIFY sourceModelChanged)
    Q_PROPERTY(QString itemTypeRole READ itemTypeRole WRITE setItemTypeRole NOTIFY itemTypeRoleChanged)
    Q_PROPERTY(int count READ readCount NOTIFY countChanged)

public:
    AbstractItemModel(QObject *parent = 0);
    ~AbstractItemModel();

    QAbstractItemModel* sourceModel() const;
    void setSourceModel(QAbstractItemModel *sourceModel);
    Q_INVOKABLE void resetSourceModel();

    QString itemTypeRole() const;
    void setItemTypeRole(const QString &roleName);

    int childCount(const QVariantList &indexPath);
    bool hasChildren(const QVariantList &indexPath);
    QString itemType(const QVariantList &indexPath);
    QVariant data(const QVariantList &indexPath);
    int readCount();

public Q_SLOTS:
    void fetchMore(const QVariantList &indexPath);

Q_SIGNALS:
    void sourceModelChanged();
    void itemTypeRoleChanged();
    void countChanged();

private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT(d, void dataChanged(const QModelIndex&, const QModelIndex&))
    Q_PRIVATE_SLOT(d, void headerDataChanged(Qt::Orientation, int, int))
    Q_PRIVATE_SLOT(d, void layoutAboutToBeChanged())
    Q_PRIVATE_SLOT(d, void layoutChanged())
    Q_PRIVATE_SLOT(d, void rowsAboutToBeInserted(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void rowsInserted(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void rowsAboutToBeRemoved(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void rowsRemoved(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void columnsAboutToBeInserted(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void columnsInserted(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void columnsAboutToBeRemoved(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void columnsRemoved(const QModelIndex&, int, int))
    Q_PRIVATE_SLOT(d, void modelAboutToBeReset())
    Q_PRIVATE_SLOT(d, void modelReset())
    Q_PRIVATE_SLOT(d, void rowsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int))
    Q_PRIVATE_SLOT(d, void rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int))
    Q_PRIVATE_SLOT(d, void columnsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int))
    Q_PRIVATE_SLOT(d, void columnsMoved(const QModelIndex&, int, int, const QModelIndex&, int))
};

#endif
