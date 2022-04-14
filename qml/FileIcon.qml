/* Copyright (C) 2017-2021 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: root
    property bool highlighted: false
    property string uuid: ""
    property string title: ""
    property alias source: _image.source

    width: Theme.iconSizeMedium
    height: Theme.iconSizeMedium

    Rectangle {
        anchors.fill: parent
        color: utils.colorFromUuid(root.uuid)
    }

    Image {
        id: _image
        anchors.fill: parent
        sourceSize.height: Theme.iconSizeMedium
        sourceSize.width: Theme.iconSizeMedium
    }

    Label {
        anchors.centerIn: parent
        visible: _image.status !== Image.Ready
        text: title.substring(0,2)
        font.pixelSize: root.width > Theme.iconSizeSmallPlus ?
                            Theme.fontSizeMedium : Theme.fontSizeSmall
        font.bold: true
        color: "black"
    }
}
