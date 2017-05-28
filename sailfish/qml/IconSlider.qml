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

    property alias leftIconSource: leftIcon.source
    property alias rightIconSource: rightIcon.source
    property alias minimumValue: slider.minimumValue
    property alias maximumValue: slider.maximumValue
    property alias value: slider.value
    property alias label: slider.label
    property alias valueText: slider.valueText
    property alias stepSize: slider.stepSize

    signal clicked

    anchors { left: parent.left; right: parent.right}
    height: Theme.itemSizeSmall + 3 * Theme.paddingLarge

    Image {
        id: leftIcon
        width: Theme.itemSizeSmall
        height: Theme.itemSizeSmall
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    Slider {
        id: slider
        width: parent.width - 2*Theme.itemSizeSmall
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: root.clicked()
        leftMargin: 0
        rightMargin: 0
    }

    Image {
        id: rightIcon
        width: Theme.itemSizeSmall
        height: Theme.itemSizeSmall
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }

}
