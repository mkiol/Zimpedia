/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import net.mkiol.zimpedia.QTimer 1.0

Container {
    id: root
    
    property alias text: label.text
    property alias interval: timer.interval
    property real size: ui.du(40)
    property bool active: false
    
    function show(_text) {
        root.text = _text;
        timer.stop();
        timer.start();
        active = true;
        fadeIn.play();
    }
    
    attachedObjects: [
        QTimer {
            id: timer
            interval: 1000
            singleShot: true
            onTimeout: {
                root.active = false;
                fadeOut.play();
            }
        }
    ]

    layout: DockLayout {}
    
    animations: [
        FadeTransition {
            id: fadeIn
            duration: 200
            easingCurve: StockCurve.Linear
            fromOpacity: 0.0
            toOpacity: 1.0
        },
        FadeTransition {
            id: fadeOut
            duration: 200
            easingCurve: StockCurve.Linear
            fromOpacity: 1.0
            toOpacity: 0.0
        }
    ]
    
    leftPadding: ui.du(2)
    preferredWidth: size
    preferredHeight: size/2
    opacity: 0.0
    background: Color.create(0,0,0,0.5)
    
    Label {
        id: label
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.base: SystemDefaults.TextStyles.BigText
        textStyle.color: Color.create(1,1,1,0.8)
        multiline: false
    }
}
