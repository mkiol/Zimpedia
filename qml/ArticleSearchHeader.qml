/* Copyright (C) 2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zimpedia.Settings 1.0

Column {
    id: root
    readonly property alias filesHeight: _files.height

    width: parent.width

    SearchField {
        id: searchField
        Connections {
            target: zimServer
            onLoadedChanged: {
                if (!zimServer.loaded) {
                    articleModel.filter = ""
                    searchField.text = ""
                }
            }
        }

        active: zimServer.loaded && !fileModel.busy && !zimServer.busy
        onActiveChanged: if (active) text = articleModel.filter
        width: parent.width
        placeholderText: qsTr("Search")
        onTextChanged: articleModel.filter = text.trim()
        EnterKey.iconSource: "image://theme/icon-m-enter-close"
        textRightMargin: Theme.paddingMedium
        EnterKey.onClicked: {
            Qt.inputMethod.hide();
            root.parent.focus = true
        }

        rightItem: Row {
            IconButton {
                icon.source: searchField.canHide && searchField.text.length === 0
                             ? "image://theme/icon-m-input-remove"
                             : "image://theme/icon-m-clear"
                visible: opacity > 0.0
                enabled: searchField.canHide || searchField.text.length > 0
                opacity: enabled ? 1.0 : 0.0
                Behavior on opacity { FadeAnimation {} }
                onClicked: {
                    searchField.text = ""
                }
            }
            IconButton {
                icon.source: settings.searchMode === Settings.FullTextSearch ?
                                     "image://icons/icon-m-fulltext" :
                                     "image://icons/icon-m-title"
                visible: opacity > 0.0
                enabled: searchField.enabled
                opacity: enabled ? 1.0 : 0.0
                Behavior on opacity { FadeAnimation {} }
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

    Column {
        id: _files
        width: parent.width
        Repeater {
            model: zimServer.files
            DoubleListItem {
                hidden: articleModel.busy || !searchField.active || articleModel.filter.length !== 0
                highlighted: down
                title: modelData[1]
                uuid: modelData[0]
                icon: modelData[2]

                onClicked: openMenu()

                menu: ContextMenu {
                    MenuItem {
                        enabled: modelData[3]
                        text: qsTr("Open main page")
                        onClicked: {
                            root.parent.focus = true
                            if (settings.browser === 1) {
                                Qt.openUrlExternally(zimServer.urlToMainPage(modelData[0]))
                            } else {
                                pageStack.push(Qt.resolvedUrl("WebViewPage.qml"), {"url": zimServer.urlToMainPage(modelData[0])});
                            }
                        }
                    }
                    MenuItem {
                        text: qsTr("Show details")
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("ZimInfoPage.qml"), {"uuid": modelData[0]});
                        }
                    }
                    MenuItem {
                        text: qsTr("Close archive")
                        onClicked: zimServer.unLoadZim(modelData[0])
                    }
                }
            }
        }
    }
}
