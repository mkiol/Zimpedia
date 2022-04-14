/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QDebug>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QUrl>

#include "itemmodel.h"
#include "listmodel.h"

class FileItem : public SelectableItem {
    Q_OBJECT
   public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IconRole = Qt::DecorationRole,
        IdRole = Qt::UserRole,
        DirRole,
        TimeRole,
        UuidRole,
        SizeRole,
        TitleRole,
        CreatorRole,
        DescriptionRole,
        DateRole,
        LanguageRole,
        SelectedRole
    };

   public:
    FileItem(QObject *parent = nullptr) : SelectableItem{parent} {}
    explicit FileItem(const QString &id, const QString &name,
                      const QString &dir, const QString &time,
                      const QString &uuid, const qint64 size,
                      const QString &title, const QString &creator,
                      const QString &date, const QString &description,
                      const QString &language, const QUrl &icon,
                      bool selected = false, QObject *parent = nullptr);
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    inline QString id() const override { return m_id; }
    inline auto name() const { return m_name; }
    inline auto dir() const { return m_dir; }
    inline auto time() const { return m_time; }
    inline auto uuid() const { return m_uuid; }
    inline auto size() const { return m_size; }
    inline auto title() const { return m_title; }
    inline auto creator() const { return m_creator; }
    inline auto date() const { return m_date; }
    inline auto description() const { return m_description; }
    inline auto language() const { return m_language; }
    inline auto icon() const { return m_icon; }

   private:
    QString m_id;
    QString m_name;
    QString m_dir;
    QString m_time;
    QString m_uuid;
    qint64 m_size = 0;
    QString m_title;
    QString m_creator;
    QString m_date;
    QString m_description;
    QString m_language;
    QUrl m_icon;
};

struct ZimMetaData {
    enum FieldFlags {
        None = 0,
        Size = 1,
        Time = 2,
        Uuid = 4,
        Name = 8,
        Title = 16,
        Creator = 32,
        Date = 64,
        Description = 128,
        Language = 256,
        Icon = 512,
        Filename = 1024,
        Publisher = 2048,
        Source = 4096,
        Tags = 8192,
        // Version = 16384,
        ArticleCount = 32768
    };

    int fields;
    QString path;
    QString filename;
    qint64 size;
    QString time;
    QString uuid;
    QString name;
    QString title;
    QString creator;
    QString date;
    QString description;
    QString language;
    QUrl icon;
    QString publisher;
    QString source;
    QString tags;
    int article_count;
    bool mainPage = false;
    bool ftindex = false;

    ZimMetaData() { clear(); }

    void clear() {
        fields = None;
        path.clear();
        filename.clear();
        size = 0;
        time.clear();
        uuid.clear();
        name.clear();
        title.clear();
        creator.clear();
        date.clear();
        description.clear();
        language.clear();
        icon.clear();
        publisher.clear();
        source.clear();
        tags.clear();
        article_count = 0;
        mainPage = false;
        ftindex = false;
    }

    inline bool empty() const { return fields == None; }
};

class FileModel : public SelectableItemModel {
    Q_OBJECT

   public:
    static FileModel *instance(QObject *parent = nullptr);
    static bool scanZim(ZimMetaData *meta);
    Q_INVOKABLE QVariantList selectedItems() override;
    inline QString uuidToFilepath(const QString &uuid) const {
        return m_files.contains(uuid) ? m_files[uuid].path : QString{};
    }
    inline bool uuidExists(const QString &uuid) const {
        return m_files.contains(uuid);
    }
    inline ZimMetaData &uuidToMeta(const QString &uuid) {
        if (!uuidExists(uuid)) throw std::runtime_error("uuid doesn't exist");
        return m_files[uuid];
    }
    static bool tagExists(const QString &tag, const QString &tags);

    Q_INVOKABLE void refresh();

   signals:
    void noFilesFound();

   private:
    static const int maxSize = 50;               // maximum size of title
    static const int maxDescSize = 5 * maxSize;  // maximum size of description
    static const int maxDepth = 1;               // maximum depth of dirs scan
    static FileModel *m_instance;
    QHash<QString, ZimMetaData> m_files;  // uuid => meta
    bool m_initalScanDone = false;

    explicit FileModel(QObject *parent = nullptr);
    void findFiles(const QString &dirName, int depth = 0);

    QList<ListItem *> makeItems() override;
};

#endif  // FILEMODEL_H
