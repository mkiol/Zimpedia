/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zimpedia.ZimMetaDataReader 1.0
import "tools.js" as Tools

Page {
    id: root
    objectName: "ziminfo"

    property string title
    property string path
    property string icon

    ZimMetaDataReader {
        id: zim
        path: root.path
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

            Icon {
                anchors.horizontalCenter: parent.horizontalCenter
                showPlaceholder: true
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                source: root.icon
                text: root.title
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
                    value: root.title
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
                    label: qsTr("Full text search possible")
                    value: zim.ftindex ? qsTr("Yes") : qsTr("No")
                }

                DetailItem {
                    label: qsTr("Size")
                    value: Tools.bytesToSize(zim.size)
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
