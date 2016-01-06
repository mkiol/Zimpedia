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

Dialog {
    id: root

    property bool advancedVisible: false

    function isPortValid() {
      var result = parseInt(portField.text).toString()==portField.text && parseInt(portField.text) > 1023 && parseInt(portField.text) <= 65535;
      portErrorLabel.error = !result;
      return result;
    }

    canAccept: isPortValid()

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            DialogHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("UI")
            }

            ComboBox {
                id: fontSizeBox
                width: root.width
                label: qsTr("Font size")
                currentIndex: settings.fontSize - 1

                menu: ContextMenu {
                    MenuItem { text: qsTr("Normal") }
                    MenuItem { text: qsTr("Large") }
                    MenuItem { text: qsTr("Extra large") }
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

    onAccepted: {
        settings.port = portField.text
        settings.fontSize = fontSizeBox.currentIndex + 1;
    }
}
