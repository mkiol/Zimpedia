/* Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
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

#include "itemmodel.h"
#include "listmodel.h"

class FileItem : public ListItem {
    Q_OBJECT
   public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IdRole = Qt::UserRole,
        DirRole,
        TimeRole,
        ChecksumRole,
        SizeRole,
        TitleRole,
        FaviconRole,
        CreatorRole,
        DescriptionRole,
        DateRole,
        LanguageRole
    };

   public:
    FileItem(QObject *parent = nullptr) : ListItem(parent) {}
    explicit FileItem(const QString &id, const QString &name,
                      const QString &dir, const QString &time,
                      const QString &checksum, const qint64 size,
                      const QString &title, const QString &creator,
                      const QString &date, const QString &description,
                      const QString &language, const QString &favicon,
                      QObject *parent = nullptr);
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    inline QString id() const override { return m_id; }
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

struct ZimMetaData {
    enum FieldFlags {
        None = 0,
        Size = 1,
        Time = 2,
        Checksum = 4,
        Name = 8,
        Title = 16,
        Creator = 32,
        Date = 64,
        Description = 128,
        Language = 256,
        Favicon = 512,
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
    QString checksum;
    QString name;
    QString title;
    QString creator;
    QString date;
    QString description;
    QString language;
    QString favicon;
    QString publisher;
    QString source;
    QString tags;
    int article_count;

    ZimMetaData() { clear(); }

    void clear() {
        fields = None;
        path.clear();
        filename.clear();
        size = 0;
        time.clear();
        checksum.clear();
        name.clear();
        title.clear();
        creator.clear();
        date.clear();
        description.clear();
        language.clear();
        favicon.clear();
        publisher.clear();
        source.clear();
        tags.clear();
        article_count = 0;
    }

    inline bool empty() const { return fields == None; }
};

class FileModel : public ItemModel {
    Q_OBJECT

   public:
    QMap<QString, ZimMetaData> files;

    static FileModel *instance(QObject *parent = nullptr);
    static bool scanZimFile(ZimMetaData *meta);

    Q_INVOKABLE void refresh();

   private:
    static const int max_size = 50;  // maximum size of title
    static FileModel *m_instance;

    explicit FileModel(QObject *parent = nullptr);
    void findFiles(const QString &dirName);

    QList<ListItem *> makeItems() override;
};

#endif  // FILEMODEL_H
