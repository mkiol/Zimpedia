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
#include <QDesktopServices>
#endif

#include <zim/file.h>

#include <string>

#include "filefinder.h"
#include "zimserver.h"

FileFinder* FileFinder::inst = 0;

FileFinder::FileFinder(QObject *parent) : QThread(parent)
{
    QObject::connect(this, SIGNAL(finished()), this, SLOT(finishedHandler()));
    QObject::connect(this, SIGNAL(started()), this, SLOT(startedHandler()));
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
    if (FileFinder::inst == 0) {
        FileFinder::inst = new FileFinder();
    }

    return FileFinder::inst;
}

void FileFinder::run()
{
    // Start search for ZIM files
    // All dirs under home, SD cards and other media
#ifdef SAILFISH
    findFiles(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
    findFiles("/media/sdcard");
    findFiles("/run/media/nemo");
#elif BB10
    findFiles(QDir::currentPath() + "/shared");
    findFiles(QDir::currentPath() + "/shared/misc/android/external_sd");
#endif
}

void FileFinder::findFiles(const QString &dirName)
{
    QDir dir(dirName);
    qDebug() << "Scanning dir:" << dirName;

    if (dir.exists(dirName)) {
        // Not following symlinks to avoid duplicates!
        QFileInfoList infoList = dir.entryInfoList(QStringList("*.zim"),
                                                   QDir::NoDotAndDotDot |
                                                   QDir::AllDirs |
                                                   QDir::Files |
                                                   QDir::NoSymLinks |
                                                   QDir::Readable,
                                                   QDir::DirsFirst);

        for (const auto& info : infoList) {
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
    zim::File *zimfile = 0;

    try
    {
        qDebug() << "Scanning file:" << metaData.path;
        zimfile = new zim::File(metaData.path.toStdString());
    }
    catch (const std::exception& e)
    {
        qWarning() << "Unable to open file" << metaData.path << "!";
        qWarning() << "Details:" << e.what();
        return false;
    }

    auto &fh = zimfile->getFileheader();
#ifdef LIBZIM
    qDebug() << "ZIM major version:" << fh.getMajorVersion();
    qDebug() << "ZIM minor version:" << fh.getMinorVersion();
    qDebug() << "multipart:" << zimfile->is_multiPart();
#elif ZIMLIB
    qDebug() << "ZIM version:" << fh.zimVersion;
#endif
    //qDebug() << "UUID:" << QByteArray(fh.getUuid().data).toHex();
    //qDebug() << "verify:" << zimfile->verify();

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
        if (ZimServer::getArticle(zimfile, "M/Name", data, mimeType)) {
            metaData.name = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "name:" << metaData.name;
        } else {
            qWarning() << "Metadata Name is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Title) {
        if (ZimServer::getArticle(zimfile, "M/Title", data, mimeType)) {
            metaData.title = QString::fromUtf8(data).left(max_size);
            //qDebug() << "title:" << metaData.title;
        } else {
            qWarning() << "Metadata Title is missing";
            metaData.title = metaData.filename;
        }
    }
    if (metaData.fields & ZimMetaData::Creator) {
        if (ZimServer::getArticle(zimfile, "M/Creator", data, mimeType)) {
            metaData.creator = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "creator:" << metaData.creator;
        } else {
            qWarning() << "Metadata Creator is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Date) {
        if (ZimServer::getArticle(zimfile, "M/Date", data, mimeType)) {
            metaData.date = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "date:" << metaData.date;
        } else {
            qWarning() << "Metadata Date is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Description) {
        if (ZimServer::getArticle(zimfile, "M/Description", data, mimeType)) {
            metaData.description = QString::fromUtf8(data).left(4 * max_size);;
            //qDebug() << "description:" << metaData.description;
        } else {
            qWarning() << "Metadata Description is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Language) {
        if (ZimServer::getArticle(zimfile, "M/Language", data, mimeType)) {
            metaData.language = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "lang:" << metaData.language;
        } else {
            qWarning() << "Metadata Language is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Publisher) {
        if (ZimServer::getArticle(zimfile, "M/Publisher", data, mimeType)) {
            metaData.publisher = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "publisher:" << metaData.publisher;
        } else {
            qWarning() << "Metadata Publisher is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Source) {
        if (ZimServer::getArticle(zimfile, "M/Source", data, mimeType)) {
            metaData.source = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "source:" << metaData.source;
        } else {
            qWarning() << "Metadata Source is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Tags) {
        if (ZimServer::getArticle(zimfile, "M/Tags", data, mimeType)) {
            metaData.tags = QString::fromUtf8(data).left(max_size);;
            //qDebug() << "tags:" << metaData.tags;
        } else {
            qWarning() << "Metadata Tags is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Favicon) {
        if (ZimServer::getArticle(zimfile, "-/favicon", data, mimeType)) {
            if (mimeType == "image/png") {
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
                qWarning() << "Favicon mimeType is not image/png. It is:" << mimeType;
            }
        } else {
            qWarning() << "Favicon is missing";
        }
    }

    if (zimfile != 0)
        delete zimfile;

    return true;
}
