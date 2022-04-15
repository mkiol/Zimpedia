/* Copyright (C) 2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: root
    property alias running: _busy.running
    property alias text: _label.text
    property alias size: _busy.size

    visible: opacity > 0.0
    opacity: running ? 1.0 : 0.0
    Behavior on opacity { FadeAnimation {} }

    Column {
        width: parent.width
        height: _label.height + _busy.height + Theme.paddingMedium
        spacing: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter

        InfoLabel {
            visible: text.length !== 0
            id: _label
        }

        BusyIndicator {
            id: _busy
            anchors.horizontalCenter: parent.horizontalCenter
            size: BusyIndicatorSize.Large
        }
    }
}
