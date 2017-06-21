/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import org.labsquare 1.0

Container {
    id: root
    
    property alias title: label.text
    property alias subtitle: label2.text
    property alias imageSource: image.source
    property variant titleColor: ui.palette.text
    property variant subtitleColor: ui.palette.secondaryTextOnPlain
    property string placeholderText: title
    property bool pressed
    property alias showLine: _line.visible
    property real imageSize: ui.du(7)
    property real padding: ui.du(2)
    
    signal clicked
    
    layout: DockLayout {}
    
    background: pressed ? ui.palette.plainBase : ui.palette.background

    onTouch: {
        pressed = event.isDown()||event.isMove() ? true : false;
        if (event.isUp())
            clicked();
    }

    Container {
        id: _line
        visible: true
        verticalAlignment: VerticalAlignment.Bottom
        background: ui.palette.plainBase
        minHeight: 2
        maxHeight: minHeight
        preferredWidth: Qt.display.pixelSize.width
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        leftPadding: root.padding
        rightPadding: root.padding
        topPadding: root.padding
        bottomPadding: root.padding
        horizontalAlignment: HorizontalAlignment.Left
        
        Icon {
            id: image
            showPlaceholder: true
            verticalAlignment: VerticalAlignment.Center
            size: root.imageSize
            text: root.placeholderText
        }

        Container {
            layout: DockLayout {}
            leftPadding: ui.du(2)
            preferredWidth: Qt.display.pixelSize.width
            preferredHeight: 1.5 * root.imageSize
            verticalAlignment: VerticalAlignment.Center

            Label {
                id: label
                verticalAlignment: root.subtitle === "" ? VerticalAlignment.Center : VerticalAlignment.Top
                textStyle.base: SystemDefaults.TextStyles.PrimaryText
                textStyle.color: root.titleColor
                multiline: false
            }
            
            Label {
                id: label2
                verticalAlignment: VerticalAlignment.Bottom
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.color: root.subtitleColor
                multiline: false
            }
        }
    }
}
