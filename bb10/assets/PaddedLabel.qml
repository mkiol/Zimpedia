/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3

Container {
    property alias text: label.text
    property alias textStyle: label.textStyle
    property alias textFormat: label.textFormat
    
    topMargin: ui.du(2)
    bottomMargin: ui.du(2)
    leftPadding: ui.du(2)
    rightPadding: ui.du(2)
    
    Label {
        id: label
        multiline: true
    }
    
}
