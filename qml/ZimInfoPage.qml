/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zimpedia.ZimMetaDataReader 1.0

Page {
    id: root
    objectName: "ziminfo"

    property string uuid

    ZimMetaDataReader {
        id: zim
        uuid: root.uuid
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
            }
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Details")
            }

            FileIcon {
                anchors.horizontalCenter: parent.horizontalCenter
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                source: zim.icon
                title: zim.title
                uuid: zim.uuid
            }

            Column {
                width: parent.width
                spacing: Theme.paddingMedium

                DetailItem {
                    label: qsTr("Name")
                    value: zim.name
                }

                DetailItem {
                    label: qsTr("Title")
                    value: zim.title
                }

                DetailItem {
                    label: qsTr("Date")
                    value: zim.date
                }

                DetailItem {
                    label: qsTr("Creator")
                    value: zim.creator
                }

                DetailItem {
                    label: qsTr("Publisher")
                    value: zim.creator
                }

                DetailItem {
                    label: qsTr("Description")
                    value: zim.description
                }

                DetailItem {
                    label: qsTr("Language")
                    value: zim.language
                }

                DetailItem {
                    label: qsTr("Article count")
                    value: zim.articleCount
                }

                DetailItem {
                    label: qsTr("Full-text index")
                    value: zim.ftindex ? qsTr("Yes") : qsTr("No")
                }

                DetailItem {
                    label: qsTr("Pictures")
                    value: zim.pictures ? qsTr("Yes") : qsTr("No")
                }

                DetailItem {
                    label: qsTr("Videos")
                    value: zim.videos ? qsTr("Yes") : qsTr("No")
                }

                DetailItem {
                    label: qsTr("Size")
                    value: utils.humanSize(zim.size)
                }

                SectionHeader {
                    text: qsTr("Path")
                }

                PaddedLabel {
                    text: zim.path
                }
            }

            Spacer {}
        }

        VerticalScrollDecorator {}
    }
}
