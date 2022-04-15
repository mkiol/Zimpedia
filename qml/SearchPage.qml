/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

        header: ArticleSearchHeader {
            onFilesHeightChanged: {
                placeholder.verticalOffset = 0.8 * filesHeight
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
                text: qsTr("About %1").arg(APP_NAME)
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
        }

        ViewPlaceholder {
            id: placeholder
            enabled: listView.count === 0 && !articleModel.busy && !zimServer.busy && !fileModel.busy
            text: {
                if (zimServer.loaded) {
                    return articleModel.filter.length !== 0 ?
                                qsTr("No results") :
                                qsTr("Find article, by typing in the search field")
                }
                return qsTr("You have not selected any archives to search")
            }
            hintText: !zimServer.loaded ? qsTr("Open pulley menu and select one") : ""
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


