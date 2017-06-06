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
    id: page
    objectName: "search"

    property real preferredItemHeight: page && page.isLandscape ? Theme.itemSizeSmall : Theme.itemSizeLarge

    SilicaListView {
        id: listView
        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: page.height
        clip:true

        model: articleModel

        /*header: PageHeader {
            title: qsTr("")
        }*/

        header: SearchField {
            id: searchField
            width: parent.width
            placeholderText: qsTr("Search")
            onTextChanged: zimServer.findTitle(text);
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: {
                Qt.inputMethod.hide();
                listView.focus = true
            }
        }

        // prevent newly added list delegates from stealing focus away from the search field
        currentIndex: -1

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

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("Find article, by typing in the search field")

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
            }

            MenuItem {
                text: qsTr("Open main page")
                visible: zimServer.hasMainPage
                onClicked: {
                    listView.focus = true
                    var url = zimServer.serverUrl()+"A/mainpage";
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

    }

    VerticalScrollDecorator {
        flickable: listView
    }
}


