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

    objectName: "search"

    property real preferredItemHeight: root && root.isLandscape ?
                                           Theme.itemSizeSmall :
                                           Theme.itemSizeLarge

    Component.onCompleted: articleModel.updateModel()

    SilicaListView {
        id: listView

        anchors.fill: parent

        currentIndex: -1

        model: articleModel

        header: SearchField {
            id: searchField
            width: parent.width
            placeholderText: qsTr("Search")
            onTextChanged: articleModel.filter = text.trim()
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: {
                Qt.inputMethod.hide();
                listView.focus = true
            }
        }

        delegate: ListItem {
            id: listItem

            contentHeight: Theme.itemSizeMedium

            Label {
                wrapMode: Text.AlignLeft
                anchors.left: parent.left; anchors.right: parent.right;
                anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeMedium
                text: model.title
            }

            menu: ContextMenu {
                MenuItem {
                    text: settings.browser === 1 ? qsTr("Open in built-in viewer") : qsTr("Open in browser")
                    onClicked: {
                        listView.focus = true

                        if (settings.browser === 1) {
                            pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": model.url});
                        } else {
                            app.openUrlEntryInBrowser(model.url)
                        }
                    }
                }
                MenuItem {
                    text: qsTr("Add to bookmarks")
                    onClicked: {
                        listView.focus = true
                        bookmarks.addBookmark(model.title, model.url, zimServer.favicon)
                    }
                }
            }

            onClicked: {
                listView.focus = true

                if (settings.browser === 1) {
                    app.openUrlEntryInBrowser(model.url)
                } else {
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": model.url});
                }
            }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: qsTr("Change ZIM file")
                onClicked: pageStack.push(Qt.resolvedUrl("FilesPage.qml"))
            }

            MenuItem {
                text: qsTr("Bookmarks")
                onClicked: pageStack.push(Qt.resolvedUrl("BookmarksPage.qml"))
                enabled: !bookmarks.busy
            }

            MenuItem {
                text: qsTr("Open main page")
                visible: zimServer.hasMainPage
                onClicked: {
                    listView.focus = true
                    var url = zimServer.serverUrl() + "A/mainpage";
                    if (settings.browser === 1) {
                        app.openUrlEntryInBrowser(url)
                    } else {
                        pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": url});
                    }
                }
            }

            /*MenuLabel {
                visible: zimServer.title != ""
                text: zimServer.title + (zimServer.language != "" ? " (" + zimServer.language + ")" : "")
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }*/

            Item {
                visible: zimServer.title != ""

                height: Theme.itemSizeExtraSmall - (screen.sizeCategory <= Screen.Medium ? Theme.paddingLarge : Theme.paddingMedium)
                width: parent ? parent.width : Screen.width
                Label {
                    opacity: 0.6
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width - Theme.horizontalPageMargin*2
                    anchors {
                        centerIn: parent
                    }
                    //truncationMode: TruncationMode.Fade
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                    text: zimServer.title + (zimServer.language != "" ? " (" + zimServer.language + ")" : "")
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0 && !articleModel.busy
            text: articleModel.filter.length > 0 ?
                      qsTr("No articles") :
                      qsTr("Find article, by typing in the search field")
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: articleModel.busy
        size: BusyIndicatorSize.Large
    }

    VerticalScrollDecorator {
        flickable: listView
    }
}


