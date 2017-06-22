/*
 * Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 * 
 * This file is part of Zimpedia application.
 * 
 * This Source Code Form is subject to the terms of
 * the Mozilla Public License, v.2.0. If a copy of
 * the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Container {
    id: root

    property alias title: drop.title
    property alias description: descriptionLabel.text
    property alias options: drop.options
    property alias selectedOption: drop.selectedOption
    property alias selectedIndex: drop.selectedIndex

    signal selectedOptionChanged

    topPadding: ui.du(1)
    bottomPadding: ui.du(2)

    DropDown {
        id: drop
        onSelectedOptionChanged: {
            root.selectedOptionChanged()
        }
    }

    Container {
        //topMargin: utils.du(1)
        visible: root.description != ""

        Label {
            id: descriptionLabel
            multiline: true
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
            textStyle.color: ui.palette.secondaryTextOnPlain
        }
    }
}