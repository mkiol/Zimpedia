/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

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

    default property alias children: container.children
    property string theme: "dimmer"

    property bool open: false
    property bool openable: true
    property bool showable: true

    property int showTime: 7000
    property real barShowMoveWidth: 20
    property var flickable: null
    readonly property bool shown: opacity == 1.0

    width: parent.width
    height: Theme.itemSizeMedium
    anchors.left: parent.left
    enabled: showable
    visible: showable

    clip: true
    opacity: root.open ? 1.0 : 0.0
    Behavior on opacity { FadeAnimation {duration: 200} }

    Behavior on y {NumberAnimation { duration: 250; easing.type: Easing.OutQuad }}
    y: open ? parent.height - height : parent.height - height/4

    function show() {
        if (!showable)
            return
        if (!open) {
            root.open = true;
            flick.contentX = 0;
        }
        timer.restart();
    }

    function hide() {
        if (open) {
            if (flick.dragging) {
                timer.restart();
            } else {
                root.open = false;
                timer.stop();
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: root.theme === "transparent" ? Theme.rgba(Theme.highlightColor, 0.2) :
               root.theme === "black" ? Theme.overlayBackgroundColor : root.theme === "highlighted" ? Theme.highlightBackgroundColor :
               root.theme === "dimmer" ? Theme.highlightDimmerColor :
               Theme.highlightDimmerColor
    }

    Image {
        anchors.fill: parent
        source: "image://theme/graphic-gradient-edge"
        opacity: root.theme !== "black" ? 0.7 : 0.5
    }

    MouseArea {
        id: mouse
        enabled: root.showable
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: parent.height
        onClicked: root.show();
    }

    Item {
        height: parent.height
        width: parent.width

        // Right
        Rectangle {
            color: root.theme === "transparent" ? Theme.secondaryColor :
                   root.theme === "black" ? Theme.secondaryColor :
                   root.theme === "highlighted" ? Theme.highlightBackgroundColor :
                   root.theme === "dimmer" ? Theme.secondaryColor :
                   Theme.secondaryColor

            height: parent.height
            width: Theme.paddingSmall
            opacity: flick.contentX < (flick.contentWidth - flick.width - Theme.paddingLarge) ? 0.5 : 0.0
            visible: opacity > 0
            anchors.right: visible ? parent.right : undefined
            Behavior on opacity {
                FadeAnimation {}
            }
        }

        // Left
        Rectangle {
            color: root.theme === "transparent" ? Theme.secondaryColor :
                   root.theme === "black" ? Theme.secondaryColor :
                   root.theme === "highlighted" ? Theme.highlightBackgroundColor :
                   root.theme === "dimmer" ? Theme.secondaryColor :
                   Theme.secondaryColor

            height: parent.height
            width: Theme.paddingSmall
            opacity: flick.contentX > Theme.paddingLarge ? 0.5 : 0.0
            visible: opacity > 0
            anchors.left: visible ? parent.left : undefined
            Behavior on opacity {
                FadeAnimation {}
            }
        }

        Flickable {
            id: flick

            enabled: root.open
            height: parent.height
            width: parent.width
            contentWidth: container.width + 2 * Theme.paddingLarge
            contentHeight: height
            pixelAligned: true

            Flow {
                id: container
                property alias theme: root.theme
                property alias open: root.open
                function show() {root.show()}

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: Theme.paddingLarge
                }

                spacing: Theme.paddingLarge
            }
        }
    }

    Timer {
        id: timer
        interval: root.showTime
        onTriggered: {
            hide();
        }
    }

    QtObject {
        id: m
        property real initialContentY: 0.0
        property real lastContentY: 0.0
        property int vector: 0
    }

    Connections {
        target: root.flickable

        onMovingChanged: {
            if (root.flickable.moving) {
                m.vector = 0;
                m.lastContentY=root.flickable.scrollableOffset.y;
                m.initialContentY=root.flickable.scrollableOffset.y;
                if (root.flickable.atYEnd && root.flickable.atYBeginning) {
                    root.show();
                }
            }
        }

        onScrollableOffsetChanged: {
            if (root.flickable.moving) {
                var dInit = root.flickable.scrollableOffset.y-m.initialContentY;
                var dLast = root.flickable.scrollableOffset.y-m.lastContentY;
                var lastV = m.vector;
                if (dInit<-barShowMoveWidth)
                    root.show();
                if (dInit>barShowMoveWidth)
                    root.hide();
                if (dLast>barShowMoveWidth)
                    root.hide();
                if (m.lastContentY!=0) {
                    if (dLast<0)
                        m.vector = -1;
                    if (dLast>0)
                        m.vector = 1;
                    if (dLast==0)
                        m.vector = 0;
                }
                if (lastV==-1 && m.vector==1)
                    m.initialContentY=root.flickable.scrollableOffset.y;
                if (lastV==1 && m.vector==-1)
                    m.initialContentY=root.flickable.scrollableOffset.y;
                m.lastContentY = root.flickable.scrollableOffset.y;
            }
        }
    }
}
