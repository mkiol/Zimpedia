/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    id: root

    property alias text: label.text

    enabled: false
    opacity: enabled ? 1.0 : 0
    Behavior on opacity { FadeAnimation { duration: 300 } }

    anchors {
        left: parent.left
        right: parent.right
        bottom: parent.bottom
        //leftMargin: Theme.horizontalPageMargin
        //rightMargin: Theme.horizontalPageMargin
        //bottomMargin: Theme.horizontalPageMargin
    }

    height: label.height + 2 * Theme.horizontalPageMargin

    /*border {
        width: 2
        color: Theme.secondaryHighlightColor
    }
    radius: Theme.paddingSmall*/
    color: Theme.highlightDimmerColor //"transparent"

    Label {
        id: label
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            margins: Theme.horizontalPageMargin
        }
        color: Theme.highlightColor
        wrapMode: Text.WordWrap
        onLinkActivated: Qt.openUrlExternally(link)
    }
}

