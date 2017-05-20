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

    property bool advancedVisible: false

    function isPortValid() {
      var result = parseInt(portField.text).toString()===portField.text && parseInt(portField.text) > 1023 && parseInt(portField.text) <= 65535;
      portErrorLabel.error = !result;
      return result;
    }

    //canAccept: isPortValid()

    SilicaFlickable {
        id: flick
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("UI")
            }

            ComboBox {
                width: root.width
                label: qsTr("Default viewer")
                currentIndex: settings.browser

                menu: ContextMenu {
                    MenuItem { text: qsTr("Web wiew") }
                    MenuItem { text: qsTr("External browser") }
                }

                onCurrentIndexChanged: {
                    settings.browser = currentIndex;
                }

                description: qsTr("Wiki pages can be opened in the built-in Web View or in an external browser.")
            }

            IconSlider {
                leftIconSource: "image://icons/icon-m-fontdown"
                rightIconSource: "image://icons/icon-m-fontup"
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

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: advancedVisible ? qsTr("Hide advanced options") : qsTr("Show advanced options")
                onClicked: advancedVisible = !advancedVisible;
            }

            SectionHeader {
                visible: advancedVisible
                text: qsTr("Advanced")
            }

            TextField {
                id: portField
                visible: advancedVisible
                anchors {left: parent.left;right: parent.right}

                inputMethodHints: Qt.ImhDigitsOnly
                placeholderText: qsTr("Enter port number here!")
                label: qsTr("Local server listening port number")

                Component.onCompleted: {
                    text = settings.port
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: {
                    Qt.inputMethod.hide();
                }
            }

            Label {
                id: portErrorLabel
                property bool error: false
                visible: error && advancedVisible
                anchors {left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge;rightMargin: Theme.paddingLarge}
                color: "Red"
                text: qsTr("Port number is invalid! Allowed port range is 1024-65535.")
                wrapMode: Text.WordWrap
            }

            Spacer {}

        }
    }

    VerticalScrollDecorator {
        flickable: flick
    }

    /*onAccepted: {
        settings.port = portField.text
        settings.fontSize = fontSizeBox.currentIndex + 1;
    }*/
}
