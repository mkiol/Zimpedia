/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QDir>
#include <QFileInfoList>
#include <QFile>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

#include "filemodel.h"

FileFinder::FileFinder(QObject *parent) : QThread(parent)
{
}

void FileFinder::run()
{
    // Start search for ZIM files

#ifdef BB10
    // All dirs under shared folder
    const QString homeLocation = QDir::currentPath() + "/shared";
#elif SAILFISH
    // All dirs under home directory
    const QStringList homeLocationList = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    const QString homeLocation = homeLocationList.isEmpty() ? "/home/nemo" : homeLocationList.first();
#endif
    findFiles(homeLocation);

    // All dirs on SD card
#ifdef SAILFISH
    const QString sdLocation = "/media/sdcard";
#elif BB10
    const QString sdLocation = QDir::currentPath() + "/shared/misc/android/external_sd";
#endif
    findFiles(sdLocation);
}

void FileFinder::findFiles(const QString &dirName)
{
    QDir dir(dirName);
    //qDebug() << "dirName" << dirName;

    if (dir.exists(dirName)) {
        // Not following symlinks to avoid duplicates!
        QFileInfoList infoList = dir.entryInfoList(QStringList("*.zim"),QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::DirsFirst);

        Q_FOREACH(QFileInfo info, infoList) {
            if (info.isDir()) {
                findFiles(info.absoluteFilePath());
            } else {
                //qDebug() << info.absoluteFilePath();
                emit fileFound(info);
            }
        }
    }
}

FileModel::FileModel(QObject *parent) :
    ListModel(new FileItem, parent), finder(parent)
{
    connect(&finder, SIGNAL(fileFound(QFileInfo)), this, SLOT(fileFoundHandler(QFileInfo)));
    connect(&finder, SIGNAL(finished()), this, SLOT(finderStatusHandler()));
    connect(&finder, SIGNAL(started()), this, SLOT(finderStatusHandler()));
    finder.start(QThread::IdlePriority);
}

bool FileModel::getSearching()
{
    return finder.isRunning();
}

void FileModel::fileFoundHandler(const QFileInfo &fileInfo)
{
    appendRow(new FileItem(fileInfo.absoluteFilePath(), fileInfo.fileName(), fileInfo.filePath()));
}

void FileModel::finderStatusHandler()
{
    emit searchingChanged();
}

void FileModel::clear()
{
    if(rowCount()>0) removeRows(0,rowCount());
}

FileItem::FileItem(const QString &id,
                   const QString &name,
                   const QString &dir,
                   QObject *parent) :
    ListItem(parent),
    m_id(id),
    m_name(name),
    m_dir(dir)
{}

QHash<int, QByteArray> FileItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[NameRole] = "name";
    names[DirRole] = "dir";
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
    default:
        return QVariant();
    }
}

