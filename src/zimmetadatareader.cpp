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

QString ZimMetaDataReader::getName()
{
    return this->m_data.name;
}

QString ZimMetaDataReader::getTitle()
{
    return this->m_data.title;
}

QString ZimMetaDataReader::getFilename()
{
    return this->m_data.filename;
}

QString ZimMetaDataReader::getCreator()
{
    return this->m_data.creator;
}

QString ZimMetaDataReader::getLanguage()
{
    return this->m_data.language;
}

QString ZimMetaDataReader::getDescription()
{
    return this->m_data.description;
}

QString ZimMetaDataReader::getDate()
{
    return this->m_data.date;
}

QString ZimMetaDataReader::getPath()
{
    return this->m_data.path;
}

QString ZimMetaDataReader::getPublisher()
{
    return this->m_data.publisher;
}

QString ZimMetaDataReader::getTags()
{
    return this->m_data.tags;
}

bool ZimMetaDataReader::getFtindex()
{
    return this->m_data.tags.split(';').contains("_ftindex");
}

/*QString ZimMetaDataReader::getSource()
{
    return this->m_data.source;
}*/

/*QString ZimMetaDataReader::getChecksum()
{
    return this->m_data.checksum;
}*/

/*QString ZimMetaDataReader::getVersion()
{
    return this->m_data.version;
}*/

int ZimMetaDataReader::getArticleCount()
{
    return this->m_data.article_count;
}

qint64 ZimMetaDataReader::getSize()
{
    return this->m_data.size;
}

void ZimMetaDataReader::setPath(const QString &path)
{
    if (this->m_data.path != path) {
        this->m_data.path = path;
        emit pathChanged();
        this->m_data.fields =
                ZimMetaData::Name |
                ZimMetaData::Title |
                ZimMetaData::Creator |
                ZimMetaData::Language |
                ZimMetaData::Date |
                ZimMetaData::Size |
                ZimMetaData::Description |
                ZimMetaData::Publisher |
                ZimMetaData::Tags |
                ZimMetaData::Filename |
                ZimMetaData::ArticleCount;
        if (!FileModel::scanZimFile(this->m_data)) {
            qWarning() << "Error while getting ZIM file info!";
            return;
        }

        emit dataChanged();
    }
}
