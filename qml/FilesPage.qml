/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: root
    objectName: "files"

    allowedOrientations: Orientation.All
    canAccept: fileModel.selectedCount > 0
    onAccepted: {
        settings.zimFiles = fileModel.selectedItems();
        zimServer.loadZim();
    }

    SilicaListView {
        id: listView

        anchors.fill: parent
        currentIndex: -1
        model: fileModel

        header: SearchDialogHeader {
            implicitWidth: root.width
            dialog: root
            search: false
            view: listView
            onActiveFocusChanged: {
                listView.currentIndex = -1
            }
        }

        PullDownMenu {
            id: menu
            busy: fileModel.busy

            MenuItem {
                text: qsTr("Open %1").arg("library.kiwix.org")
                onClicked: Qt.openUrlExternally("http://library.kiwix.org")
            }
            MenuItem {
                enabled: !fileModel.busy
                text: qsTr("Refresh")
                onClicked: fileModel.refresh()
            }
        }

        delegate: DoubleListItem {
            highlighted: down || model.selected
            title: model.title
            uuid: model.uuid
            visible: !fileModel.busy && listView.count > 0
            icon: model.icon
            onClicked: {
                var selected = model.selected
                fileModel.setSelected(model.index, !selected);
            }
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Show details")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ZimInfoPage.qml"), {"uuid": model.uuid});
                    }
                }
            }
        }

        ViewPlaceholder {
            id: placeholder
            enabled: listView.count == 0 && !fileModel.busy
            verticalOffset: root.isPortrait ? Theme.itemSizeLarge : Theme.itemSizeSmall
            text: qsTr("It looks like you don't have any ZIM archives")
            hintText: qsTr("The ZIM is an open file format that stores content coming from the Web for offline use. " +
                           "The collection of various archives can be found on " +
                           "%1 website. " +
                           "If you have already downloaded ZIM files, put them to any folder you like under SD card, Download or Documents directory.")
            .arg("<b>library.kiwix.org</b>")
        }

        Button {
            visible: placeholder.enabled
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Theme.iconSizeMedium
            text: qsTr("Open %1").arg("library.kiwix.org")
            onClicked: Qt.openUrlExternally("http://library.kiwix.org")
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


