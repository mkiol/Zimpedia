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
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

#include <zim/file.h>

#include <string>

#include "filefinder.h"
#include "zimserver.h"

FileFinder* FileFinder::inst = Q_NULLPTR;

FileFinder::FileFinder(QObject *parent) : QThread(parent)
{
    connect(this, &QThread::finished, this, &FileFinder::finishedHandler);
    connect(this, &QThread::started, this, &FileFinder::startedHandler);
    init();
}

void FileFinder::init()
{
    files.clear();
    this->start(QThread::IdlePriority);
}

void FileFinder::startedHandler()
{
    busy = true;
    emit busyChanged();
}

void FileFinder::finishedHandler()
{
    busy = false;
    emit busyChanged();
}

FileFinder* FileFinder::instance()
{
    if (FileFinder::inst == Q_NULLPTR) {
        FileFinder::inst = new FileFinder();
    }

    return FileFinder::inst;
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
    const QString homeLocation = homeLocationList.first();
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
        QFileInfoList infoList = dir.entryInfoList(QStringList("*.zim"),
                                                   QDir::NoDotAndDotDot |
                                                   QDir::AllDirs |
                                                   QDir::Files |
                                                   QDir::NoSymLinks |
                                                   QDir::Readable,
                                                   QDir::DirsFirst);

        Q_FOREACH(QFileInfo info, infoList) {
            if (info.isDir()) {
                findFiles(info.absoluteFilePath());
            } else {
                //qDebug() << info.absoluteFilePath();
                ZimMetaData metaData;
                metaData.path = info.absoluteFilePath();
                metaData.filename = info.fileName();
                metaData.size = info.size();
                metaData.fields =
                        ZimMetaData::Title |
                        ZimMetaData::Language |
                        ZimMetaData::Favicon |
                        ZimMetaData::Checksum;
                if (FileFinder::scanZimFile(metaData)) {
                    files.insert(metaData.checksum, metaData);
                    emit fileFound(metaData);
                }
            }
        }
    }
}

bool FileFinder::scanZimFile(ZimMetaData &metaData)
{
    zim::File *zimfile = Q_NULLPTR;

    try
    {
        zimfile = new zim::File(metaData.path.toStdString());
    }
    catch (const std::exception& e)
    {
        qWarning() << "Unable to open ZIM file" << metaData.path << "!";
        qWarning() << "Details:" << e.what();
        return false;
    }


    metaData.filename = QFileInfo(metaData.path).fileName();
    metaData.checksum = QString::fromStdString(zimfile->getChecksum());
    if (metaData.checksum == "") {
        qWarning() << "Metadata Checksum is missing!";
        metaData.checksum = metaData.filename.toLocal8Bit().toHex();
        qDebug() << "Hex checksum:" << metaData.checksum;
    }

    if (metaData.fields & ZimMetaData::Time)
        metaData.time = QDateTime::fromTime_t(zimfile->getMTime()).toLocalTime().toString("d MMMM yyyy");
    if (metaData.fields & ZimMetaData::Size)
        metaData.size = QFileInfo(metaData.path).size();
    if (metaData.fields & ZimMetaData::ArticleCount)
        metaData.article_count = zimfile->getCountArticles();

    QByteArray data; QString mimeType;

    if (metaData.fields & ZimMetaData::Name) {
        if (ZimServer::getArticle(zimfile, "M/Name", data, mimeType))
            metaData.name = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Name is missing!";
    }
    if (metaData.fields & ZimMetaData::Title) {
        if (ZimServer::getArticle(zimfile, "M/Title", data, mimeType)) {
            metaData.title = QString::fromUtf8(data);
        } else {
            qWarning() << "Metadata Title is missing!";
            metaData.title = metaData.filename;
        }
    }
    if (metaData.fields & ZimMetaData::Creator) {
        if (ZimServer::getArticle(zimfile, "M/Creator", data, mimeType))
            metaData.creator = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Creator is missing!";
    }
    if (metaData.fields & ZimMetaData::Date) {
        if (ZimServer::getArticle(zimfile, "M/Date", data, mimeType))
            metaData.date = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Date is missing!";
    }
    if (metaData.fields & ZimMetaData::Description) {
        if (ZimServer::getArticle(zimfile, "M/Description", data, mimeType))
            metaData.description = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Description is missing!";
    }
    if (metaData.fields & ZimMetaData::Language) {
        if (ZimServer::getArticle(zimfile, "M/Language", data, mimeType))
            metaData.language = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Language is missing!";
    }
    if (metaData.fields & ZimMetaData::Publisher) {
        if (ZimServer::getArticle(zimfile, "M/Publisher", data, mimeType))
            metaData.publisher = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Publisher is missing!";
    }
    if (metaData.fields & ZimMetaData::Source) {
        if (ZimServer::getArticle(zimfile, "M/Source", data, mimeType))
            metaData.source = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Source is missing!";
    }
    if (metaData.fields & ZimMetaData::Tags) {
        if (ZimServer::getArticle(zimfile, "M/Tags", data, mimeType))
            metaData.tags = QString::fromUtf8(data);
        else
            qWarning() << "Metadata Tags is missing!";
    }
    if (metaData.fields & ZimMetaData::Favicon) {
        if (ZimServer::getArticle(zimfile, "-/favicon", data, mimeType)) {
            QString filename = "zim-favicon-" + metaData.checksum ;
    #if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
            const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    #else
            const QString cacheDir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    #endif
            QFile file(cacheDir + "/" + filename + ".png");
            if (!file.exists()) {
                if (file.open(QFile::WriteOnly)) {
                    file.write(data);
                    file.close();
                    metaData.favicon = "image://icons/" + filename;
                } else {
                    qWarning() << "Unable to open file" << file.fileName() << " to write!";
                }
            } else {
                metaData.favicon = "image://icons/" + filename;
            }
        } else {
            qWarning() << "Favicon is missing!";
        }
    }

    if (zimfile != Q_NULLPTR)
        delete zimfile;

    return true;
}
