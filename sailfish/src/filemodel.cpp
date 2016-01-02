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
#include <QStandardPaths>

#include "filemodel.h"

FileFinder::FileFinder(QObject *parent) : QThread(parent)
{
}

void FileFinder::run()
{
    // Start search for ZIM files
    const QStringList homeLocation = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString homePath = homeLocation.isEmpty() ? "/home/nemo" : homeLocation.first();
    findFiles(homePath);
}

void FileFinder::findFiles(const QString &dirName)
{
    QDir dir(dirName);

    //qDebug() << "dirName" << dirName;

    if (dir.exists(dirName)) {
        QFileInfoList infoList = dir.entryInfoList(QStringList("*.zim"),QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

        Q_FOREACH(QFileInfo info, infoList) {
            if (info.isDir())
                findFiles(info.absoluteFilePath());
            else
                emit fileFound(info);
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

