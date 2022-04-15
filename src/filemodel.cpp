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

#include "settings.h"
#include "zimserver.h"

FileModel *FileModel::m_instance = nullptr;

FileModel *FileModel::instance(QObject *parent) {
    if (FileModel::m_instance == nullptr) {
        FileModel::m_instance = new FileModel(parent);
    }

    return FileModel::m_instance;
}

FileModel::FileModel(QObject *parent)
    : SelectableItemModel{new FileItem, false, parent} {}

QVariantList FileModel::selectedItems() {
    QVariantList list;

    foreach (const auto item, m_list) {
        auto *file = qobject_cast<FileItem *>(item);
        if (file->selected()) list.push_back(file->uuid());
    }

    return list;
}

void FileModel::findFiles(const QString &dirName, int depth) {
    if (depth > maxDepth) return;

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
            findFiles(info.absoluteFilePath(), depth + 1);
        } else {
            ZimMetaData meta;
            meta.path = info.absoluteFilePath();
            meta.filename = info.fileName();
            meta.size = info.size();
            meta.fields = ZimMetaData::Title | ZimMetaData::Language |
                          ZimMetaData::Icon | ZimMetaData::Uuid;
            if (scanZim(&meta)) {
                m_files.insert(meta.uuid, meta);
            }
        }
    }
}

static QUrl extractFavIcon(const zim::Archive &archive,
                           const QString &checksum) {
    QUrl favUrl;

    if (!archive.hasIllustration()) {
        return favUrl;
    }

    /*qDebug() << "illustration sizes:";
    for (auto s : archive.getIllustrationSizes()) {
        qDebug() << "size:" << s;
    }*/

    auto sizes = archive.getIllustrationSizes();
    if (sizes.empty()) {
        return favUrl;
    }

    try {
        auto item = archive.getIllustrationItem(*sizes.rbegin());
        if (item.getMimetype() != "image/png") {
            qWarning() << "unsupported favicon type:"
                       << QString::fromStdString(item.getMimetype());
            return favUrl;
        }

        QString filename{"zim-favicon-" + checksum};
        auto cacheDir =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QFile file{cacheDir + "/" + filename + ".png"};
        if (!file.exists()) {
            auto blob = item.getData();
            if (blob.size() > 0) {
                if (file.open(QFile::WriteOnly)) {
                    file.write(
                        QByteArray{blob.data(), static_cast<int>(blob.size())});
                } else {
                    qWarning() << "unable to open file:" << file.fileName();
                    return favUrl;
                }
            }
        }
        favUrl = QUrl{"image://icons/" + filename};
    } catch (const zim::EntryNotFound &) {
    }

    return favUrl;
}

bool FileModel::tagExists(const QString &tag, const QString &tags) {
    auto tagList = tags.split(';');
    for (const auto &t : tagList) {
        auto fl = t.split(':');
        if (fl.size() == 1) {
            if (tag == fl.first().trimmed()) return true;
        } else if (fl.size() == 2) {
            if (tag == fl.first().trimmed()) return fl.at(1).trimmed() == "yes";
        }
    }
    return false;
}

bool FileModel::scanZim(ZimMetaData *meta) {
    qDebug() << "scanning file:" << meta->path << meta->uuid;

    if (meta->path.isEmpty()) {
        if (!FileModel::instance()->uuidExists(meta->uuid)) {
            qWarning()
                << "can't scan because no path or existing uuid provided";
            return false;
        }
        meta->path = FileModel::instance()->uuidToFilepath(meta->uuid);
    }

    try {
        zim::Archive archive{meta->path.toStdString()};
        qDebug() << "multipart:" << archive.isMultiPart();

        /*qDebug() << "zim file meta keys:";
        for (const auto &key : archive.getMetadataKeys()) {
            qDebug() << QString::fromStdString(key);
        }*/

        if (meta->filename.isEmpty()) {
            meta->filename = QFileInfo{meta->path}.fileName();
        }
        if (meta->uuid.isEmpty()) {
            meta->uuid = QString::fromStdString(
                static_cast<std::string>(archive.getUuid()));
        }
        if (meta->uuid.isEmpty()) {
            qDebug() << "meta uuid is empty";
            meta->uuid = meta->filename.toLocal8Bit().toHex();
        }
        if (meta->fields & ZimMetaData::Time) {
            meta->time = QFileInfo{meta->path}.created().toLocalTime().toString(
                "d MMMM yyyy");
        }
        meta->mainPage = archive.hasMainEntry();
        if (meta->fields & ZimMetaData::Size && meta->size != 0) {
            meta->size = QFileInfo{meta->path}.size();
        }
        if (meta->fields & ZimMetaData::ArticleCount)
            meta->article_count = archive.getArticleCount();
        if (meta->fields & ZimMetaData::Tags && meta->tags.isEmpty()) {
            try {
                meta->tags = QString::fromStdString(archive.getMetadata("Tags"))
                                 .toLower();
                qDebug() << "tags:" << meta->tags;
                meta->ftindex =
                    tagExists(QStringLiteral("_ftindex"), meta->tags);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Tags";
            }
        }
        if (meta->fields & ZimMetaData::Name && meta->name.isEmpty()) {
            try {
                meta->name = QString::fromStdString(archive.getMetadata("Name"))
                                 .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Name";
            }
        }
        if (meta->fields & ZimMetaData::Title && meta->title.isEmpty()) {
            try {
                meta->title =
                    QString::fromStdString(archive.getMetadata("Title"))
                        .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Title";
            }
        }
        if (meta->fields & ZimMetaData::Creator && meta->creator.isEmpty()) {
            try {
                meta->creator =
                    QString::fromStdString(archive.getMetadata("Creator"))
                        .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Creator";
            }
        }
        if (meta->fields & ZimMetaData::Description &&
            meta->description.isEmpty()) {
            try {
                meta->description =
                    QString::fromStdString(archive.getMetadata("Description"))
                        .left(maxDescSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Description";
            }
        }
        if (meta->fields & ZimMetaData::Language && meta->language.isEmpty()) {
            try {
                meta->language =
                    QString::fromStdString(archive.getMetadata("Language"))
                        .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Language";
            }
        }
        if (meta->fields & ZimMetaData::Publisher &&
            meta->publisher.isEmpty()) {
            try {
                meta->publisher =
                    QString::fromStdString(archive.getMetadata("Publisher"))
                        .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Publisher";
            }
        }
        if (meta->fields & ZimMetaData::Source && meta->source.isEmpty()) {
            try {
                meta->source =
                    QString::fromStdString(archive.getMetadata("Source"))
                        .left(maxSize);
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Source";
            }
        }
        if (meta->fields & ZimMetaData::Date && meta->date.isEmpty()) {
            try {
                meta->date =
                    QString::fromStdString(archive.getMetadata("Date"));
            } catch (const zim::EntryNotFound &e) {
                qWarning() << "archive does not have Date";
            }
        }
        if (meta->fields & ZimMetaData::Icon && meta->icon.isEmpty()) {
            meta->icon = extractFavIcon(archive, meta->uuid);
            if (meta->icon.isEmpty()) {
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
    m_files.clear();
    updateModel();
}

QList<ListItem *> FileModel::makeItems() {
    if (m_files.isEmpty()) {
        auto paths =
            QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
        findFiles(paths.first());
        if (QFileInfo::exists(QStringLiteral("/media/sdcard"))) {
            findFiles(QStringLiteral("/media/sdcard"), -1);
        } else {
            findFiles("/run/media/" + QDir::home().dirName(), -1);
        }
        if (!m_initalScanDone && m_files.isEmpty()) {
            emit noFilesFound();
        }
        m_initalScanDone = true;
    }

    auto uuids = Settings::instance()->zimFiles();

    QList<ListItem *> items;

    for (auto it = m_files.begin(); it != m_files.end(); ++it) {
        const auto &meta = it.value();
        items.push_back(new FileItem{
            meta.path, meta.name, meta.path, meta.time, meta.uuid, meta.size,
            meta.language.isEmpty()
                ? meta.title
                : QString{"%1 · %2"}.arg(meta.title, meta.language),
            meta.creator, meta.date, meta.description, meta.language, meta.icon,
            uuids.contains(meta.uuid)});
    }

    std::sort(items.begin(), items.end(), [](ListItem *a, ListItem *b) {
        auto *aa = qobject_cast<FileItem *>(a);
        auto *bb = qobject_cast<FileItem *>(b);
        return aa->title().compare(bb->title(), Qt::CaseInsensitive) < 0;
    });

    return items;
}

FileItem::FileItem(const QString &id, const QString &name, const QString &dir,
                   const QString &time, const QString &uuid, const qint64 size,
                   const QString &title, const QString &creator,
                   const QString &date, const QString &description,
                   const QString &language, const QUrl &icon, bool selected,
                   QObject *parent)
    : SelectableItem(parent),
      m_id(id),
      m_name(name),
      m_dir(dir),
      m_time(time),
      m_uuid(uuid),
      m_size(size),
      m_title(title),
      m_creator(creator),
      m_date(date),
      m_description(description),
      m_language(language),
      m_icon(icon) {
    setSelected(selected);
}

QHash<int, QByteArray> FileItem::roleNames() const {
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[NameRole] = "name";
    names[DirRole] = "dir";
    names[TimeRole] = "time";
    names[UuidRole] = "uuid";
    names[SizeRole] = "size";
    names[TitleRole] = "title";
    names[IconRole] = "icon";
    names[CreatorRole] = "creator";
    names[DescriptionRole] = "description";
    names[DateRole] = "date";
    names[LanguageRole] = "language";
    names[SelectedRole] = "selected";
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
        case UuidRole:
            return uuid();
        case SizeRole:
            return size();
        case TitleRole:
            return title();
        case IconRole:
            return icon();
        case CreatorRole:
            return creator();
        case DescriptionRole:
            return description();
        case DateRole:
            return date();
        case LanguageRole:
            return language();
        case SelectedRole:
            return selected();
        default:
            return {};
    }
}
