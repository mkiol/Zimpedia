/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ZIMMETADATAREADER_H
#define ZIMMETADATAREADER_H

#include <QObject>
#include <QString>

#include "filemodel.h"

class ZimMetaDataReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString path READ path NOTIFY dataChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(QString title READ title NOTIFY dataChanged)
    Q_PROPERTY(QString creator READ creator NOTIFY dataChanged)
    Q_PROPERTY(QString date READ date NOTIFY dataChanged)
    Q_PROPERTY(QString description READ description NOTIFY dataChanged)
    Q_PROPERTY(QString language READ language NOTIFY dataChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY dataChanged)
    Q_PROPERTY(QString publisher READ publisher NOTIFY dataChanged)
    Q_PROPERTY(QString tags READ tags NOTIFY dataChanged)
    Q_PROPERTY(QUrl icon READ icon NOTIFY dataChanged)
    Q_PROPERTY(bool ftindex READ ftindex NOTIFY dataChanged)
    Q_PROPERTY(bool pictures READ pictures NOTIFY dataChanged)
    Q_PROPERTY(bool videos READ videos NOTIFY dataChanged)
    Q_PROPERTY(int articleCount READ articleCount NOTIFY dataChanged)
    Q_PROPERTY(qint64 size READ size NOTIFY dataChanged)
   public:
    explicit ZimMetaDataReader(QObject *parent = nullptr) : QObject{parent} {};

   signals:
    void dataChanged();
    void uuidChanged();

   public:
    void setUuid(const QString &uuid);
    inline auto uuid() const { return m_data.uuid; }
    inline auto path() const { return m_data.path; }
    inline auto name() const { return m_data.name; }
    inline auto title() const { return m_data.title; }
    inline auto creator() const { return m_data.creator; }
    inline auto date() const { return m_data.date; }
    inline auto description() const { return m_data.description; }
    inline auto language() const { return m_data.language; }
    inline auto filename() const { return m_data.filename; }
    inline auto publisher() const { return m_data.publisher; }
    inline auto tags() const { return m_data.tags; }
    inline auto icon() const { return m_data.icon; }
    inline auto ftindex() const {
        return FileModel::tagExists(QStringLiteral("_ftindex"), m_data.tags);
    }
    inline auto pictures() const {
        return FileModel::tagExists(QStringLiteral("_pictures"), m_data.tags);
    }
    inline auto videos() const {
        return FileModel::tagExists(QStringLiteral("_videos"), m_data.tags);
    }
    inline auto size() const { return m_data.size; }
    inline auto articleCount() const { return m_data.article_count; }

   private:
    ZimMetaData m_data;
};

#endif  // ZIMMETADATAREADER_H
