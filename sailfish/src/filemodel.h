/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <QModelIndex>
#include <QThread>
#include <QFileInfo>

#include "listmodel.h"

class FileItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        DirRole = Qt::UserRole + 3,
        SizeRole = Qt::UserRole + 4
    };

public:
    FileItem(QObject *parent = 0): ListItem(parent) {}
    explicit FileItem(const QString &id,
                      const QString &name,
                      const QString &dir,
                      const qint64 size,
                      QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_id; }
    inline QString name() const { return m_name; }
    inline QString dir() const { return m_dir; }
    inline qint64 size() const { return m_size; }

private:
    QString m_id;
    QString m_name;
    QString m_dir;
    qint64 m_size;
};

class FileFinder : public QThread
{
    Q_OBJECT

public:
    explicit FileFinder(QObject *parent = 0);

protected:
    void run();

signals:
    void fileFound(const QFileInfo & fileInfo);

private:
    void findFiles(const QString &dirName);
};

class FileModel : public ListModel
{
    Q_OBJECT
    Q_PROPERTY (bool searching READ getSearching NOTIFY searchingChanged)

public:
    explicit FileModel(QObject *parent = 0);
    void clear();
    bool getSearching();

signals:
    void searchingChanged();

public slots:
    void fileFoundHandler(const QFileInfo & fileInfo);
    void finderStatusHandler();

private:
    FileFinder finder;
};

#endif // FILEMODEL_H
