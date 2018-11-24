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

Page {
    id: root
    objectName: "bookmarkfiles"

    property string uuid
    property string title
    property string language
    property string favicon

    SilicaListView {
        id: listView

        anchors.fill: parent

        model: fileModel

        header: PageHeader {
            title: qsTr("Choose ZIM file")
        }

        delegate: ListItem {
            id: listItem

            property bool active: model.checksum === root.uuid

            visible: !fileModel.busy

            contentHeight: Theme.itemSizeMedium

            Icon {
                id: icon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                showPlaceholder: true
                source: model.favicon
                text: model.title
                height: Theme.iconSizeMedium
                width: Theme.iconSizeMedium
            }

            Column {
                anchors {
                    left: icon.right
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                spacing: Theme.paddingSmall

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeMedium
                    color:  listItem.active || listItem.down ? Theme.highlightColor : Theme.primaryColor
                    text: model.title + (model.language === "" ? "" : " (" + model.language + ")")
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: listItem.active || listItem.down ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    text: model.dir
                }
            }

            onClicked: {
                root.title = model.title
                root.language = model.language
                root.favicon = model.favicon

                root.uuid = model.checksum
                pageStack.pop()
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !fileModel.busy
            text: qsTr("No files were found")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: fileModel.refresh()
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: fileModel.busy
        size: BusyIndicatorSize.Large
    }

    VerticalScrollDecorator {
        flickable: listView
    }
}
