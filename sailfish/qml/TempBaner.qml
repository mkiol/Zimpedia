/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0


Rectangle {
    id: root

    property alias text: label.text
    property alias interval: timer.interval

    function show(text) {
        root.text = text
        timer.restart()
    }

    width: 2*Theme.iconSizeLarge
    height: 2*Theme.iconSizeMedium
    radius: 10
    color: Theme.rgba("black", 0.7)
    opacity: timer.running ? 1.0 : 0.0
    visible: opacity > 0.0
    Behavior on opacity { FadeAnimation {} }

    Timer {
        id: timer
        interval: 1000
    }

    Label {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left; anchors.right: parent.right
        font.pixelSize: Theme.fontSizeExtraLarge
    }
}
