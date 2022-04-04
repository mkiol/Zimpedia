/* Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <zim/file.h>

#include <string>

#include "filemodel.h"
#include "zimserver.h"

FileModel *FileModel::m_instance = nullptr;

FileModel *FileModel::instance(QObject *parent) {
    if (FileModel::m_instance == nullptr) {
        FileModel::m_instance = new FileModel(parent);
    }

    return FileModel::m_instance;
}

FileModel::FileModel(QObject *parent) : ItemModel(new FileItem, parent) {}

void FileModel::findFiles(const QString &dirName) {
    QDir dir(dirName);
    // qDebug() << "Scanning dir:" << dirName;

    if (dir.exists(dirName)) {
        // Not following symlinks to avoid duplicates!
        QFileInfoList infoList = dir.entryInfoList(
            QStringList("*.zim"),
            QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files |
                QDir::NoSymLinks | QDir::Readable,
            QDir::DirsFirst);

        for (const auto &info : infoList) {
            if (info.isDir()) {
                findFiles(info.absoluteFilePath());
            } else {
                // qDebug() << info.absoluteFilePath();
                ZimMetaData metaData;
                metaData.path = info.absoluteFilePath();
                metaData.filename = info.fileName();
                metaData.size = info.size();
                metaData.fields = ZimMetaData::Title | ZimMetaData::Language |
                                  ZimMetaData::Favicon | ZimMetaData::Checksum;
                if (scanZimFile(metaData)) {
                    files.insert(metaData.checksum, metaData);
                }
            }
        }
    }
}

bool FileModel::scanZimFile(ZimMetaData &metaData) {
    zim::File *zimfile = nullptr;

    try {
        qDebug() << "Scanning file:" << metaData.path;
        zimfile = new zim::File(metaData.path.toStdString());
    } catch (const std::exception &e) {
        qWarning() << "Unable to open file" << metaData.path << "!";
        qWarning() << "Details:" << e.what();
        return false;
    }

    const auto &fh = zimfile->getFileheader();
    qDebug() << "ZIM major version:" << fh.getMajorVersion();
    qDebug() << "ZIM minor version:" << fh.getMinorVersion();
    qDebug() << "multipart:" << zimfile->is_multiPart();

    metaData.filename = QFileInfo(metaData.path).fileName();
    metaData.checksum = QString::fromStdString(zimfile->getChecksum());
    if (metaData.checksum == "") {
        qWarning() << "Metadata Checksum is missing!";
        metaData.checksum = metaData.filename.toLocal8Bit().toHex();
        qDebug() << "Hex checksum:" << metaData.checksum;
    }

    if (metaData.fields & ZimMetaData::Time)
        metaData.time = QDateTime::fromTime_t(zimfile->getMTime())
                            .toLocalTime()
                            .toString("d MMMM yyyy");
    if (metaData.fields & ZimMetaData::Size)
        metaData.size = QFileInfo(metaData.path).size();
    if (metaData.fields & ZimMetaData::ArticleCount)
        metaData.article_count = zimfile->getCountArticles();

    QByteArray data;
    QString mimeType;

    if (metaData.fields & ZimMetaData::Name) {
        if (ZimServer::getArticle(zimfile, "M/Name", data, mimeType)) {
            metaData.name = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "name:" << metaData.name;
        } else {
            qWarning() << "Metadata Name is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Title) {
        if (ZimServer::getArticle(zimfile, "M/Title", data, mimeType)) {
            metaData.title = QString::fromUtf8(data).left(max_size);
            // qDebug() << "title:" << metaData.title;
        } else {
            qWarning() << "Metadata Title is missing";
            metaData.title = metaData.filename;
        }
    }
    if (metaData.fields & ZimMetaData::Creator) {
        if (ZimServer::getArticle(zimfile, "M/Creator", data, mimeType)) {
            metaData.creator = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "creator:" << metaData.creator;
        } else {
            qWarning() << "Metadata Creator is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Date) {
        if (ZimServer::getArticle(zimfile, "M/Date", data, mimeType)) {
            metaData.date = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "date:" << metaData.date;
        } else {
            qWarning() << "Metadata Date is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Description) {
        if (ZimServer::getArticle(zimfile, "M/Description", data, mimeType)) {
            metaData.description = QString::fromUtf8(data).left(4 * max_size);
            ;
            // qDebug() << "description:" << metaData.description;
        } else {
            qWarning() << "Metadata Description is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Language) {
        if (ZimServer::getArticle(zimfile, "M/Language", data, mimeType)) {
            metaData.language = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "lang:" << metaData.language;
        } else {
            qWarning() << "Metadata Language is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Publisher) {
        if (ZimServer::getArticle(zimfile, "M/Publisher", data, mimeType)) {
            metaData.publisher = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "publisher:" << metaData.publisher;
        } else {
            qWarning() << "Metadata Publisher is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Source) {
        if (ZimServer::getArticle(zimfile, "M/Source", data, mimeType)) {
            metaData.source = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "source:" << metaData.source;
        } else {
            qWarning() << "Metadata Source is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Tags) {
        if (ZimServer::getArticle(zimfile, "M/Tags", data, mimeType)) {
            metaData.tags = QString::fromUtf8(data).left(max_size);
            ;
            // qDebug() << "tags:" << metaData.tags;
        } else {
            qWarning() << "Metadata Tags is missing";
        }
    }
    if (metaData.fields & ZimMetaData::Favicon) {
        if (ZimServer::getArticle(zimfile, "-/favicon", data, mimeType)) {
            if (mimeType == "image/png") {
                QString filename = "zim-favicon-" + metaData.checksum;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                const QString cacheDir = QStandardPaths::writableLocation(
                    QStandardPaths::CacheLocation);
#else
                const QString cacheDir = QDesktopServices::storageLocation(
                    QDesktopServices::CacheLocation);
#endif
                QFile file(cacheDir + "/" + filename + ".png");
                if (!file.exists()) {
                    if (file.open(QFile::WriteOnly)) {
                        file.write(data);
                        file.close();
                        metaData.favicon = "image://icons/" + filename;
                    } else {
                        qWarning() << "Unable to open file" << file.fileName()
                                   << " to write!";
                    }
                } else {
                    metaData.favicon = "image://icons/" + filename;
                }
            } else {
                qWarning() << "Favicon mimeType is not image/png. It is:"
                           << mimeType;
            }
        } else {
            qWarning() << "Favicon is missing";
        }
    }

    if (zimfile) delete zimfile;

    return true;
}

void FileModel::refresh() {
    files.clear();
    updateModel();
}

QList<ListItem *> FileModel::makeItems() {
    if (files.isEmpty()) {
        auto paths =
            QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
        findFiles(paths.first());
        if (QFileInfo::exists("/media/sdcard"))
            findFiles("/media/sdcard");
        else
            findFiles("/run/media/nemo");
    }

    QList<ListItem *> items;

    for (auto it = files.begin(); it != files.end(); ++it) {
        const auto &metaData = it.value();
        items << new FileItem(metaData.path, metaData.name, metaData.path,
                              metaData.time, metaData.checksum, metaData.size,
                              metaData.title, metaData.creator, metaData.date,
                              metaData.description, metaData.language,
                              metaData.favicon);
    }

    return items;
}

FileItem::FileItem(const QString &id, const QString &name, const QString &dir,
                   const QString &time, const QString &checksum,
                   const qint64 size, const QString &title,
                   const QString &creator, const QString &date,
                   const QString &description, const QString &language,
                   const QString &favicon, QObject *parent)
    : ListItem(parent),
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
      m_favicon(favicon) {}

QHash<int, QByteArray> FileItem::roleNames() const {
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

QVariant FileItem::data(int role) const {
    switch (role) {
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
