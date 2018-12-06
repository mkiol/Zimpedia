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
    objectName: "settings"

    property bool advancedVisible: false

    function isPortValid() {
      var result = parseInt(portField.text).toString()===portField.text && parseInt(portField.text) > 1023 && parseInt(portField.text) <= 65535;
      portErrorLabel.error = !result;
      return result;
    }

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
                description: qsTr("By default when you enter a phase, search only by article title is performed. " +
                                  "A different mode is a search within full article text. " +
                                  "The full-text search mode works only when ZIM file contains appropriate index.")
                currentIndex: settings.searchMode === Settings.FullTextSearch ? 1 : 0
                menu: ContextMenu {
                    MenuItem { text: qsTr("By title") }
                    MenuItem { text: qsTr("Full-text") }
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
                    MenuItem { text: qsTr("Browser") }
                }

                onCurrentIndexChanged: {
                    settings.browser = currentIndex;
                }

                description: qsTr("Wiki pages can be opened in the built-in web viewer or in an external browser.")
            }

            IconSlider {
                leftIconSource: "image://icons/icon-m-fontdown?" + Theme.primaryColor
                rightIconSource: "image://icons/icon-m-fontup?" + Theme.primaryColor
                label: qsTr("Viewer font size level")
                minimumValue: 50
                maximumValue: 200
                value: Math.round(settings.zoom * 100)
                valueText: value + "%"
                stepSize: 10
                onValueChanged: settings.zoom = value/100
                onClicked: {
                    // Default value
                    value = 100;
                }
            }

            Spacer {}
        }
    }

    VerticalScrollDecorator {
        flickable: flick
    }
}
