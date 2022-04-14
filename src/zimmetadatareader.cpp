/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "zimmetadatareader.h"

#include <QDebug>

void ZimMetaDataReader::setUuid(const QString &uuid) {
    if (m_data.uuid != uuid) {
        m_data.uuid = uuid;
        emit uuidChanged();
        m_data.fields = ZimMetaData::Name | ZimMetaData::Title |
                        ZimMetaData::Creator | ZimMetaData::Language |
                        ZimMetaData::Date | ZimMetaData::Size |
                        ZimMetaData::Description | ZimMetaData::Publisher |
                        ZimMetaData::Tags | ZimMetaData::Filename |
                        ZimMetaData::ArticleCount | ZimMetaData::Icon;
        if (!FileModel::scanZim(&m_data)) {
            qWarning() << "error while getting zim file info";
            return;
        }

        emit dataChanged();
    }
}
