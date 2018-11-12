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
import harbour.zimpedia.FileModel 1.0
import "tools.js" as Tools

Page {
    id: root
    objectName: "files"

    SilicaListView {
        id: listView

        anchors.fill: parent

        model: FileModel {
            id: fileModel
            Component.onCompleted: init(false)
        }

        header: PageHeader {
            title: qsTr("Choose ZIM file")
        }

        delegate: ListItem {
            id: listItem

            property bool active: settings.zimFile === model.id

            contentHeight: Theme.itemSizeMedium

            /*Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Theme.rgba(Theme.highlightColor, 0.0) }
                    GradientStop { position: 1.0; color: Theme.rgba(Theme.highlightColor, 0.1) }
                }
                visible: listItem.active
            }*/

            menu: ContextMenu {
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
                    //text: Tools.bytesToSize(model.size) + " • " + Tools.friendlyPath(model.dir, utils.homeDir())
                    text: Tools.friendlyPath(model.dir, utils.homeDir())
                }
            }

            onClicked: {
                settings.zimFile = model.id;
                zimServer.loadZimFile();
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !fileModel.busy
            text: qsTr("No files were found")
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
                enabled: !bookmarks.busy
                onClicked: pageStack.push(Qt.resolvedUrl("BookmarksPage.qml"))
            }

            MenuItem {
                text: qsTr("Refresh")
                onClicked: fileModel.init(true)
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


