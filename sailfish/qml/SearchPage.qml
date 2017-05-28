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

    property real preferredItemHeight: page && page.isLandscape ? Theme.itemSizeSmall : Theme.itemSizeLarge

    function openUrlEntryInBrowser(url) {
        notification.show(qsTr("Launching an external browser..."))
        Qt.openUrlExternally(encodeURI(url))
    }

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
                font.pixelSize: Theme.fontSizeMedium
                text: model.title
            }

            menu: ContextMenu {
                MenuItem {
                    text: settings.browser === 1 ? qsTr("Open in built-in viewer") : qsTr("Open in browser")
                    onClicked: {
                        listView.focus = true

                        if (settings.browser === 1) {
                            pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),
                                           {"url": model.url, "title": model.title});
                        } else {
                            openUrlEntryInBrowser(model.url)
                        }
                    }
                }
            }

            onClicked: {
                listView.focus = true

                if (settings.browser === 1) {
                    openUrlEntryInBrowser(model.url)
                } else {
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),
                                   {"url": model.url, "title": model.title});
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("Find article, by typing in the search field")

        }

        PageMenu {
            onOpenMainPageClicked: {
                listView.focus = true
                var url = zimServer.serverUrl()+"A/mainpage";
                if (settings.browser === 1) {
                    openUrlEntryInBrowser(url)
                } else {
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),
                                   {"url": url, "title": "Main page"});
                }
            }
        }

    }

    VerticalScrollDecorator {
        flickable: listView
    }
}


