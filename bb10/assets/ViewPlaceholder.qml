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
    id: root
    
    property alias text: label.text
    property alias textFormat: label.textFormat

    horizontalAlignment: HorizontalAlignment.Center
    verticalAlignment: VerticalAlignment.Center
    
    Label {
        id: label
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.base: SystemDefaults.TextStyles.TitleText
        textStyle.fontStyle: FontStyle.Italic
        textStyle.textAlign: TextAlign.Center
        textStyle.color: ui.palette.secondaryTextOnPlain
        multiline: true
    }
}

