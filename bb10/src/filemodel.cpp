/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "filemodel.h"

FileModel::FileModel(QObject *parent) :
    ListModel(new FileItem, parent), busy(false)
{
    FileFinder* f = FileFinder::instance();
    //connect(f, &FileFinder::fileFound, this, &FileModel::fileFoundHandler);
    //connect(f, &FileFinder::busyChanged, this, &FileModel::finderBusyHandler);
    QObject::connect(f, SIGNAL(busyChanged()), this, SLOT(finderBusyHandler()));
    //init();
}

bool FileModel::getBusy()
{
    return busy;
}

void FileModel::fileFoundHandler(const ZimMetaData &metaData)
{
    appendRow(new FileItem(metaData.path, metaData.name, metaData.path,
                           metaData.time, metaData.checksum, metaData.size,
                           metaData.title, metaData.creator, metaData.date,
                           metaData.description, metaData.language,
                           metaData.favicon));
}

void FileModel::finderBusyHandler()
{
    FileFinder* f = FileFinder::instance();
    if (!f->busy) {
        QMap<QString, ZimMetaData>::iterator it = f->files.begin();
        QMap<QString, ZimMetaData>::iterator end = f->files.end();
        for (;it != end; ++it) {
            fileFoundHandler(*it);
        }
        busy = false;
        emit busyChanged();
    }
}

void FileModel::init(bool refresh = true)
{
    FileFinder* f = FileFinder::instance();

    if (refresh) {
        busy = true;
        emit busyChanged();
        clear();
        f->init();
    } else {
        clear();
        QMap<QString, ZimMetaData>::iterator it = f->files.begin();
        QMap<QString, ZimMetaData>::iterator end = f->files.end();
        for (;it != end; ++it) {
            fileFoundHandler(*it);
        }
    }
}

void FileModel::clear()
{
    if(rowCount()>0) removeRows(0,rowCount());
}

FileItem::FileItem(const QString &id,
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
                   QObject *parent) :
    ListItem(parent),
    m_id(id),
    m_name(name),
    m_dir(dir),
    m_time(time),
    m_checksum(checksum),
    m_size(size),
    m_title(title),
    m_creator(creator),
    m_date(date),
    m_description(description),
    m_language(language),
    m_favicon(favicon)
{}

QHash<int, QByteArray> FileItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[NameRole] = "name";
    names[DirRole] = "dir";
    names[TimeRole] = "time";
    names[ChecksumRole] = "checksum";
    names[SizeRole] = "size";
    names[TitleRole] = "title";
    names[FaviconRole] = "favicon";
    names[CreatorRole] = "creator";
    names[DescriptionRole] = "description";
    names[DateRole] = "date";
    names[LanguageRole] = "language";
    return names;
}

QVariant FileItem::data(int role) const
{
    switch(role) {
    case IdRole:
        return id();
    case NameRole:
        return name();
    case DirRole:
        return dir();
    case TimeRole:
        return time();
    case ChecksumRole:
        return checksum();
    case SizeRole:
        return size();
    case TitleRole:
        return title();
    case FaviconRole:
        return favicon();
    case CreatorRole:
        return creator();
    case DescriptionRole:
        return description();
    case DateRole:
        return date();
    case LanguageRole:
        return language();
    default:
        return QVariant();
    }
}

