/* Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "zimmetadatareader.h"

#include <QDebug>

ZimMetaDataReader::ZimMetaDataReader(QObject *parent)
    : QObject(parent)
{
}

QString ZimMetaDataReader::getName() const { return this->m_data.name; }

QString ZimMetaDataReader::getTitle() const { return this->m_data.title; }

QString ZimMetaDataReader::getFilename() const { return this->m_data.filename; }

QString ZimMetaDataReader::getCreator() const { return this->m_data.creator; }

QString ZimMetaDataReader::getLanguage() const { return this->m_data.language; }

QString ZimMetaDataReader::getDescription() const {
    return this->m_data.description;
}

QString ZimMetaDataReader::getDate() const { return this->m_data.date; }

QString ZimMetaDataReader::getPath() const { return this->m_data.path; }

QString ZimMetaDataReader::getPublisher() const {
    return this->m_data.publisher;
}

QString ZimMetaDataReader::getTags() const { return this->m_data.tags; }

bool ZimMetaDataReader::getFtindex() const {
    return this->m_data.tags.split(';').contains("_ftindex");
}

/*QString ZimMetaDataReader::getSource() const
{
    return this->m_data.source;
}*/

/*QString ZimMetaDataReader::getChecksum() const
{
    return this->m_data.checksum;
}*/

/*QString ZimMetaDataReader::getVersion() const
{
    return this->m_data.version;
}*/

int ZimMetaDataReader::getArticleCount() const {
    return this->m_data.article_count;
}

qint64 ZimMetaDataReader::getSize() const { return this->m_data.size; }

void ZimMetaDataReader::setPath(const QString &path)
{
    if (m_data.path != path) {
        m_data.path = path;
        emit pathChanged();
        m_data.fields = ZimMetaData::Name | ZimMetaData::Title |
                        ZimMetaData::Creator | ZimMetaData::Language |
                        ZimMetaData::Date | ZimMetaData::Size |
                        ZimMetaData::Description | ZimMetaData::Publisher |
                        ZimMetaData::Tags | ZimMetaData::Filename |
                        ZimMetaData::ArticleCount;
        if (!FileModel::scanZimFile(&m_data)) {
            qWarning() << "Error while getting ZIM file info!";
            return;
        }

        emit dataChanged();
    }
}
