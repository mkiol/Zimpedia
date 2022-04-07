/* Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "filemodel.h"

#include <zim/archive.h>
#include <zim/blob.h>
#include <zim/error.h>
#include <zim/item.h>

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardPaths>
#include <string>

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
    QDir dir{dirName};

    if (!dir.exists(dirName)) {
        qWarning() << "directory does not exist:" << dirName;
        return;
    }

    // Not following symlinks to avoid duplicates
    auto infoList =
        dir.entryInfoList({"*.zim"},
                          QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files |
                              QDir::NoSymLinks | QDir::Readable,
                          QDir::DirsFirst);

    for (const auto &info : infoList) {
        if (info.isDir()) {
            findFiles(info.absoluteFilePath());
        } else {
            ZimMetaData meta;
            meta.path = info.absoluteFilePath();
            meta.filename = info.fileName();
            meta.size = info.size();
            meta.fields = ZimMetaData::Title | ZimMetaData::Language |
                          ZimMetaData::Favicon | ZimMetaData::Checksum;
            if (scanZimFile(&meta)) {
                files.insert(meta.checksum, meta);
            }
        }
    }
}

static QString extractFavIcon(zim::Archive archive, const QString &checksum) {
    QString favUrl;

    if (!archive.hasIllustration()) {
        return favUrl;
    }

    //    qDebug() << "illustration sizes:";
    //    for (auto s : archive.getIllustrationSizes()) {
    //        qDebug() << "size:" << s;
    //    }

    auto sizes = archive.getIllustrationSizes();
    if (sizes.empty()) {
        return favUrl;
    }

    try {
        auto item = archive.getIllustrationItem(*sizes.rbegin());
        qDebug() << "favicon mime:"
                 << QString::fromStdString(item.getMimetype());
        if (item.getMimetype() != "image/png") {
            return favUrl;
        }

        auto blob = item.getData();
        if (blob.size() == 0) {
            return favUrl;
        }

        QString filename = "zim-favicon-" + checksum;
        auto cacheDir =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QFile file{cacheDir + "/" + filename + ".png"};
        if (!file.exists()) {
            if (file.open(QFile::WriteOnly)) {
                file.write(
                    QByteArray{blob.data(), static_cast<int>(blob.size())});
            } else {
                qWarning() << "unable to open file:" << file.fileName();
                return favUrl;
            }
        }
        favUrl = "image://icons/" + filename;
    } catch (const zim::EntryNotFound &) {
    }

    return favUrl;
}

bool FileModel::scanZimFile(ZimMetaData *meta) {
    qDebug() << "scanning file:" << meta->path;

    try {
        zim::Archive archive{meta->path.toStdString()};

        // qDebug() << "ZIM major version:" << fh.getMajorVersion();
        // qDebug() << "ZIM minor version:" << fh.getMinorVersion();
        qDebug() << "multipart:" << archive.isMultiPart();

        //        qDebug() << "zim file meta keys:";
        //        for (const auto &key : archive.getMetadataKeys()) {
        //            qDebug() << QString::fromStdString(key);
        //        }

        meta->filename = QFileInfo{meta->path}.fileName();
        meta->checksum = QString::fromStdString(archive.getChecksum());
        if (meta->checksum.isEmpty()) {
            qDebug() << "meta checksum is empty";
            meta->checksum = meta->filename.toLocal8Bit().toHex();
        }
        if (meta->fields & ZimMetaData::Time) {
            meta->time = QFileInfo{meta->path}.created().toLocalTime().toString(
                "d MMMM yyyy");
        }
        if (meta->fields & ZimMetaData::Size)
            meta->size = QFileInfo{meta->path}.size();
        if (meta->fields & ZimMetaData::ArticleCount)
            meta->article_count = archive.getArticleCount();
        if (meta->fields & ZimMetaData::Tags) {
            try {
                meta->tags = QString::fromStdString(archive.getMetadata("Tags"))
                                 .toLower();
                qDebug() << "tags:" << meta->tags;
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Tags";
            }
        }
        if (meta->fields & ZimMetaData::Name) {
            try {
                meta->name = QString::fromStdString(archive.getMetadata("Name"))
                                 .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Name";
            }
        }
        if (meta->fields & ZimMetaData::Title) {
            try {
                meta->title =
                    QString::fromStdString(archive.getMetadata("Title"))
                        .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Title";
            }
        }
        if (meta->fields & ZimMetaData::Creator) {
            try {
                meta->creator =
                    QString::fromStdString(archive.getMetadata("Creator"))
                        .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Creator";
            }
        }
        if (meta->fields & ZimMetaData::Description) {
            try {
                meta->description =
                    QString::fromStdString(archive.getMetadata("Description"))
                        .left(5 * max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Description";
            }
        }
        if (meta->fields & ZimMetaData::Language) {
            try {
                meta->language =
                    QString::fromStdString(archive.getMetadata("Language"))
                        .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Language";
            }
        }
        if (meta->fields & ZimMetaData::Publisher) {
            try {
                meta->publisher =
                    QString::fromStdString(archive.getMetadata("Publisher"))
                        .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Publisher";
            }
        }
        if (meta->fields & ZimMetaData::Source) {
            try {
                meta->source =
                    QString::fromStdString(archive.getMetadata("Source"))
                        .left(max_size);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Source";
            }
        }
        if (meta->fields & ZimMetaData::Date) {
            try {
                meta->date =
                    QString::fromStdString(archive.getMetadata("Date"));
                qDebug() << "date:" << meta->date;
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Date";
            }
        }
        if (meta->fields & ZimMetaData::Favicon) {
            meta->favicon = extractFavIcon(archive, meta->checksum);
            if (meta->favicon.isEmpty()) {
                qWarning() << "archive does not have Favicon";
            }
        }
        return true;
    } catch (const std::exception &e) {
        qWarning() << "unable to open file:" << meta->path << e.what();
    }

    return false;
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
        items << new FileItem{
            metaData.path,        metaData.name,     metaData.path,
            metaData.time,        metaData.checksum, metaData.size,
            metaData.title,       metaData.creator,  metaData.date,
            metaData.description, metaData.language, metaData.favicon};
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
            return {};
    }
}
