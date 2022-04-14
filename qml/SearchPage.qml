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
import harbour.zimpedia.Settings 1.0

Page {
    id: root
    objectName: "search"
    Component.onCompleted: articleModel.updateModel()

    Connections {
        target: settings
        onSearchModeChanged: articleModel.refresh()
    }

    SilicaListView {
        id: listView

        anchors.fill: parent
        currentIndex: -1
        model: articleModel

        header: SearchField {
            id: search
            Connections {
                target: zimServer
                onLoadedChanged: {
                    if (!zimServer.loaded) {
                        articleModel.filter = ""
                        search.text = ""
                    }
                }
            }

            active: zimServer.loaded && !fileModel.busy && !zimServer.busy
            onActiveChanged: if (active) text = articleModel.filter
            width: parent.width
            placeholderText: qsTr("Search")
            onTextChanged: articleModel.filter = text.trim()
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: {
                Qt.inputMethod.hide();
                listView.focus = true
            }

            Item {
                parent: search
                anchors.fill: parent
                IconButton {
                    anchors {
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    width: icon.width
                    height: parent.height
                    icon.source: search.text.length == 0 ?
                                     settings.searchMode === Settings.FullTextSearch ?
                                         "image://theme/icon-m-file-document" :
                                         "image://theme/icon-m-file-other" : ""
                    enabled: search.enabled
                    opacity: icon.status === Image.Ready ? 1 : 0
                    Behavior on opacity {
                        FadeAnimation {}
                    }
                    onClicked: {
                        if (settings.searchMode === Settings.FullTextSearch) {
                            settings.searchMode = Settings.TitleSearch
                        } else {
                            settings.searchMode = Settings.FullTextSearch
                        }
                    }
                }
            }
        }

        delegate: ArticleItem {
            hidden: !zimServer.loaded || articleModel.busy || zimServer.busy || fileModel.busy
            title: model.title
            showIcon: zimServer.filesCount > 1
            zimTitle: model.zimTitle
            zimUuid: model.zimUuid
            icon: model.zimIcon

            menu: ContextMenu {
                MenuItem {
                    text: settings.browser === 1 ? qsTr("Open in viewer") : qsTr("Open in an external app")
                    onClicked: {
                        listView.focus = true
                        if (settings.browser === 1 ) {
                            pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": model.url});
                        } else {
                            Qt.openUrlExternally(model.url)
                        }
                    }
                }
                MenuItem {
                    text: qsTr("Add to bookmarks")
                    onClicked: {
                        listView.focus = true
                        bookmarkModel.addBookmark(model.title, model.url, zimServer.favicon)
                    }
                }
            }

            onClicked: {
                listView.focus = true
                if (settings.browser === 1) {
                     Qt.openUrlExternally(model.url)
                } else {
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"), {"url": model.url});
                }
            }
        }

        PullDownMenu {
            busy: articleModel.busy || zimServer.busy || fileModel.busy
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: qsTr("Select archives")
                onClicked: pageStack.push(Qt.resolvedUrl("FilesPage.qml"))
            }

            MenuItem {
                text: qsTr("Bookmarks")
                onClicked: pageStack.push(Qt.resolvedUrl("BookmarksPage.qml"))
                enabled: !bookmarkModel.busy
            }

//            MenuItem {
//                text: qsTr("Open main page")
//                visible: zimServer.hasMainPage
//                onClicked: {
//                    listView.focus = true
//                    var url = zimServer.serverUrl() + "A/mainpage";
//                    if (settings.browser === 1) {
//                        app.openUrlEntryInBrowser(url)
//                    } else {
//                        pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": url});
//                    }
//                }
//            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0 && !articleModel.busy && !zimServer.busy && !fileModel.busy
            text: {
                if (zimServer.loaded) {
                    return articleModel.filter.length > 0 ?
                                qsTr("No results") :
                                qsTr("Find article, by typing in the search field")
                }
                return qsTr("You have not selected any archives to search")
            }
            hintText: qsTr("Open pulley menu and select one")
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: articleModel.busy || zimServer.busy || fileModel.busy
        size: BusyIndicatorSize.Large
    }

    VerticalScrollDecorator {
        flickable: listView
    }
}


