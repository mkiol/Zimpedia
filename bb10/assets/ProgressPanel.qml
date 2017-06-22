/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Container {
    id: root

    property alias text: label.text
    property bool cancelable: true
    property bool open: true
    property alias progress: indicator.value
    property alias showProgress: indicator.visible
    signal cancelClicked

    layout: DockLayout {}
    
    preferredWidth: display.pixelSize.width
    preferredHeight: ui.du(10)
    
    background: ui.palette.plain

    enabled: open
    visible: open

    function show(text) {
        root.progress = -1.0;
        root.text = text;
        root.open = true;
    }

    function hide() {
        root.open = false;
        root.progress = -1.0;
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        leftPadding: ui.du(2)
        rightPadding: ui.du(2)
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Left

        ActivityIndicator {
            running: root.open
            verticalAlignment: VerticalAlignment.Center
        }

        Label {
            id: label
            verticalAlignment: VerticalAlignment.Center
            multiline: true
            visible: text != ""
        }
    }

    PressableContainer {
        layout: DockLayout {
        }
        visible: root.cancelable
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Center

        onClicked: {
            root.cancelClicked();
        }

        preferredWidth: ui.du(10)
        preferredHeight: ui.du(10)

        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///cancel-red.png"
            preferredWidth: ui.du(8)
            preferredHeight: ui.du(8)
        }
    }

    ProgressIndicator {
        id: indicator
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Bottom
        preferredWidth: display.pixelSize.width
        minWidth: display.pixelSize.width
        maxWidth: display.pixelSize.width
    }

}