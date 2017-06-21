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
    
    property string title
    property string description
    
    Label {
        text: title
        textStyle.base: SystemDefaults.TextStyles.BodyText
        multiline: true
        topMargin: 0; bottomMargin: 0
    }
    
    Label {
        topMargin: ui.du(1)
        text: description
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
        multiline: true
    }
    
    Container {
        preferredHeight: ui.du(2)
    }
}
