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
    objectName: "settings"

    SilicaFlickable {
        id: flick
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: root.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Settings")
            }

            ComboBox {
                label: qsTr("Search mode")
                description: qsTr("You can search for articles by title only (option 'By title') or, " +
                                  "alternatively, search through the full text of all articles in the archives (option 'Full-text'). " +
                                  "Full-text search only works when ZIM file contains the appropriate index.")
                currentIndex: settings.searchMode === Settings.FullTextSearch ? 1 : 0
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("By title")
                        Image {
                            source: "image://theme/icon-m-file-other?" + (parent.highlighted ? Theme.highlightColor : Theme.primaryColor)
                            height: Theme.iconSizeMedium
                            width: Theme.iconSizeMedium
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    MenuItem {
                        text: qsTr("Full-text")
                        Image {
                            source: "image://theme/icon-m-file-document?" + (parent.highlighted ? Theme.highlightColor : Theme.primaryColor)
                            height: Theme.iconSizeMedium
                            width: Theme.iconSizeMedium
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                onCurrentIndexChanged: {
                    settings.searchMode = currentIndex === 1 ?
                                Settings.FullTextSearch : Settings.TitleSearch
                }
            }

            ComboBox {
                width: root.width
                label: qsTr("Default viewer")
                currentIndex: settings.browser

                menu: ContextMenu {
                    MenuItem { text: qsTr("Web view") }
                    MenuItem { text: qsTr("External app") }
                }

                onCurrentIndexChanged: {
                    settings.browser = currentIndex;
                }

                description: qsTr("Articles can be opened in the built-in web viewer or in an external app (e.g. Web Browser).")
            }

            Slider {
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Viewer font size level")
                minimumValue: 50
                maximumValue: 200
                value: Math.round(settings.zoom * 100)
                valueText: value + "%"
                stepSize: 10
                onValueChanged: settings.zoom = value/100
                onClicked: value = 100
            }

            Spacer {}
        }
    }

    VerticalScrollDecorator {
        flickable: flick
    }
}
