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
    
    property alias att: label.text
    property string value
    property variant attColor: ui.palette.secondaryTextOnPlain
    property variant valueColor: ui.palette.text
    property real size: ui.du(12)
    property real horizontalPadding: ui.du(2)
    property real verticalPadding: ui.du(1)
    
    layout: DockLayout {}
    
    Container {
        verticalAlignment: VerticalAlignment.Bottom
        background: ui.palette.plainBase
        minHeight: 2
        maxHeight: minHeight
        preferredWidth: Qt.display.pixelSize.width
    }

    Container {
        layout: DockLayout {}
        leftPadding: root.horizontalPadding
        rightPadding: root.horizontalPadding
        topPadding: root.verticalPadding
        bottomPadding: root.verticalPadding
        preferredWidth: Qt.display.pixelSize.width
        //preferredHeight: label.
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Left

        Label {
            id: label
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Left
            textStyle.base: SystemDefaults.TextStyles.BodyText
            textStyle.color: root.attColor
            multiline: false
        }

        Container {
            topPadding: ui.du(5)
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Left
            Label {
                id: label2
                textStyle.base: SystemDefaults.TextStyles.PrimaryText
                textStyle.color: root.valueColor
                multiline: true
                text: root.value == "" ? "-" : root.value
            }
        }
    }
}
