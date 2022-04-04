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


Item {
    id: root

    property string text
    property bool cancelable: true
    property bool open: false
    property real progress: 0.0
    property bool transparent: true
    signal closeClicked

    enabled: open
    opacity: open ? 1.0 : 0.0
    visible: opacity > 0.0

    height: Theme.itemSizeMedium
    width: parent.width

    onVisibleChanged: {
        if (!visible) {
            progress = 0;
        }
    }

    function show(text) {
        root.text = text;
        root.open = true;
    }

    function hide() {
        root.open = false;
        root.progress = 0.0;
    }

    Behavior on opacity { FadeAnimation {} }

    Rectangle {
        anchors.left: parent.left; anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height
        visible: !root.transparent
        color: Theme.highlightColor
    }

    Rectangle {
        id: progressRect
        height: parent.height - 0
        anchors.left: parent.left; anchors.bottom: parent.bottom
        width: root.progress * parent.width
        color: root.transparent ? Theme.rgba(Theme.highlightBackgroundColor, 0.3) : Theme.overlayBackgroundColor //Theme.rgba(Theme.highlightDimmerColor, 0.2)

        Behavior on width {
            SmoothedAnimation {
                velocity: 480; duration: 200
            }
        }
    }

    Image {
        anchors.fill: parent
        source: "image://theme/graphic-gradient-edge?" + Theme.primaryColor
    }

    Item {
        anchors.left: parent.left; anchors.right: parent.right
        anchors.bottom: parent.bottom; height: parent.height - 0

        Image {
            id: icon
            height: 0.6*root.height; width: 0.6*root.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingMedium
            source: "image://theme/graphic-busyindicator-medium?"+(root.transparent ? Theme.highlightDimmerColor : Theme.overlayBackgroundColor)
            RotationAnimation on rotation {
                loops: Animation.Infinite
                from: 0
                to: 360
                duration: 1200
                running: root.open && Qt.application.active
            }
        }

        Label {
            id: titleBar
            height: icon.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: icon.right; anchors.right: closeButton.left
            anchors.leftMargin: Theme.paddingMedium; anchors.rightMargin: Theme.paddingMedium
            font.pixelSize: Theme.fontSizeSmall
            font.family: Theme.fontFamily
            text: root.text
            color: root.transparent ? Theme.highlightDimmerColor : Theme.overlayBackgroundColor
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        IconButton {
            id: closeButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            icon.source: "image://theme/icon-m-dismiss?"+(root.transparent ? Theme.highlightDimmerColor : Theme.overlayBackgroundColor)
            onClicked: root.closeClicked()
            visible: root.cancelable
        }
    }
}
