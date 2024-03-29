/*
 * Author: Christophe Dumez <dchris@gmail.com>
 * License: Public domain (No attribution required)
 * Website: http://cdumez.blogspot.com/
 * Version: 1.0
 */

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>

class ListItem : public QObject {
    Q_OBJECT

   public:
    ListItem(QObject* parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

   signals:
    void dataChanged();
};

class ListModel : public QAbstractListModel {
    Q_OBJECT

   public:
    explicit ListModel(ListItem* prototype, QObject* parent = 0);
    ~ListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void appendRow(ListItem* item);
    void appendRows(const QList<ListItem*>& items);
    void insertRow(int row, ListItem* item);
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count,
                    const QModelIndex& parent = QModelIndex());
    void moveRow(int orig, int dest, const QModelIndex& parent = QModelIndex());
    ListItem* takeRow(int row);
    ListItem* readRow(int row);
    ListItem* find(const QString& id) const;
    QModelIndex indexFromItem(const ListItem* item) const;
    int indexFromId(const QString& id) const;
    void clear();

   private slots:
    void handleItemChange();

   protected:
    QList<ListItem*> m_list;

   private:
    ListItem* m_prototype;
};

#endif  // LISTMODEL_H
