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

#include "listmodel.h"
#include "filefinder.h"

class FileItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        DirRole = Qt::UserRole + 3,
        TimeRole = Qt::UserRole + 4,
        ChecksumRole = Qt::UserRole + 5,
        SizeRole = Qt::UserRole + 6,
        TitleRole = Qt::UserRole + 7,
        FaviconRole = Qt::UserRole + 8,
        CreatorRole = Qt::UserRole + 9,
        DescriptionRole = Qt::UserRole + 10,
        DateRole = Qt::UserRole + 11,
        LanguageRole = Qt::UserRole + 12
    };

public:
    FileItem(QObject *parent = 0): ListItem(parent) {}
    explicit FileItem(const QString &id,
                      const QString &name,
                      const QString &dir,
                      const QString &time,
                      const QString &checksum,
                      const qint64 size,
                      const QString &title,
                      const QString &creator,
                      const QString &date,
                      const QString &description,
                      const QString &language,
                      const QString &favicon,
                      QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_id; }
    inline QString name() const { return m_name; }
    inline QString dir() const { return m_dir; }
    inline QString time() const { return m_time; }
    inline QString checksum() const { return m_checksum; }
    inline qint64 size() const { return m_size; }
    inline QString title() const { return m_title; }
    inline QString creator() const { return m_creator; }
    inline QString date() const { return m_date; }
    inline QString description() const { return m_description; }
    inline QString language() const { return m_language; }
    inline QString favicon() const { return m_favicon; }
private:
    QString m_id;
    QString m_name;
    QString m_dir;
    QString m_time;
    QString m_checksum;
    qint64 m_size;
    QString m_title;
    QString m_creator;
    QString m_date;
    QString m_description;
    QString m_language;
    QString m_favicon;
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
    void fileFoundHandler(const ZimMetaData &metaData);
    void finderStatusHandler();

private:
    FileFinder finder;
};

#endif // FILEMODEL_H
