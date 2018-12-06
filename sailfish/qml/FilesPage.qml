/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

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
    objectName: "files"

    property real preferredItemHeight: root && root.isLandscape ?
                                           Theme.itemSizeSmall :
                                           Theme.itemSizeLarge

    Component.onCompleted: fileModel.updateModel()

    SilicaListView {
        id: listView

        anchors.fill: parent

        currentIndex: -1

        model: fileModel

        header: PageHeader {
            title: qsTr("ZIM files")
        }

        delegate: ListItem {
            id: listItem

            property bool active: settings.zimFile === model.id

            visible: !fileModel.busy

            contentHeight: Theme.itemSizeMedium

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Open file")
                    onClicked: {
                        settings.zimFile = model.id;
                        zimServer.loadZimFile();
                    }
                }

                MenuItem {
                    text: qsTr("Show details")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ZimInfoPage.qml"),
                                       {"path": model.dir,
                                       "icon": model.favicon,
                                       "title": model.title});
                    }
                }
            }

            onClicked: {
                settings.zimFile = model.id;
                zimServer.loadZimFile();
            }

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
                    color: listItem.active || listItem.down ? Theme.highlightColor : Theme.primaryColor
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
        }

        Bubble {
            enabled: listView.count == 0 && !fileModel.busy && !menu.active
            text: qsTr("The ZIM is an open file format that stores wiki content for offline usage. "+
                       "The collection of nice wikis can be downloaded from "+
                       "<a href='http://www.kiwix.org/wiki/Content_in_all_languages'>this page</a>. "+
                       "If you already have some ZIM files, put them to any folder you like under your home directory or SD card.")
        }

        PullDownMenu {
            id: menu

            MenuItem {
                text: qsTr("About")
                visible: !zimServer.loaded
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                visible: !zimServer.loaded
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: qsTr("Bookmarks")
                visible: !zimServer.loaded
                enabled: !bookmarkModel.busy
                onClicked: pageStack.push(Qt.resolvedUrl("BookmarksPage.qml"))
            }

            MenuItem {
                text: qsTr("Refresh")
                onClicked: fileModel.refresh()
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !fileModel.busy
            text: qsTr("No files were found")
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


