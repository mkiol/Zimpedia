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
        id: _search
        Connections {
            target: zimServer
            onLoadedChanged: {
                if (!zimServer.loaded) {
                    articleModel.filter = ""
                    _search.text = ""
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
            root.parent.focus = true
        }

        Item {
            parent: _search
            anchors.fill: parent
            IconButton {
                anchors {
                    right: parent.right
                    rightMargin: _search.text.length === 0 ? Theme.horizontalPageMargin :
                                                             Theme.iconSizeMedium + Theme.horizontalPageMargin +
                                                             Theme.paddingMedium
                }
                width: icon.width
                height: parent.height
                icon.source: settings.searchMode === Settings.FullTextSearch ?
                                     "image://icons/icon-m-fulltext" :
                                     "image://icons/icon-m-title"
                enabled: _search.enabled
                opacity: icon.status === Image.Ready ? 1 : 0
                Behavior on opacity { FadeAnimation {} }
                Behavior on anchors.rightMargin { NumberAnimation { duration: 200; easing.type: Easing.InOutQuad } }
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
                hidden: articleModel.busy || !_search.active || articleModel.filter.length !== 0
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
                        text: qsTr("Close archive")
                        onClicked: zimServer.unLoadZim(modelData[0])
                    }
                }
            }
        }
    }
}
