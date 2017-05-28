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
                anchors {
                    left: parent.left
                    right: parent.right
                }
                spacing: Theme.paddingMedium

                AttValue {
                    att: qsTr("Name")
                    value: zim.name
                }

                AttValue {
                    att: qsTr("Title")
                    value: root.title
                }

                AttValue {
                    att: qsTr("Date")
                    value: zim.date
                }

                AttValue {
                    att: qsTr("Creator")
                    value: zim.creator
                }

                AttValue {
                    att: qsTr("Publisher")
                    value: zim.creator
                }

                AttValue {
                    att: qsTr("Description")
                    value: zim.description
                }

                AttValue {
                    att: qsTr("Language")
                    value: zim.language
                }

                AttValue {
                    att: qsTr("Tags")
                    value: zim.tags
                }

                AttValue {
                    att: qsTr("Source")
                    value: zim.source
                }

                AttValue {
                    att: qsTr("Article count")
                    value: zim.articleCount
                }

                AttValue {
                    att: qsTr("Filename")
                    value: zim.filename
                }

                AttValue {
                    att: qsTr("Path")
                    value: Tools.friendlyDir(zim.path, utils.homeDir())
                }

                AttValue {
                    att: qsTr("Size")
                    value: Tools.bytesToSize(zim.size)
                }
            }

            Spacer {}
        }

        VerticalScrollDecorator {}
    }
}
