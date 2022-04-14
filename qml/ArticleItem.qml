/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    id: root

    property alias title: _title.text
    property alias showIcon: _icon.visible
    property string zimTitle
    property alias zimUuid: _icon.uuid
    property alias icon: _icon.source
    property bool invalid: false

    contentHeight: Theme.itemSizeSmall

    anchors {
        left: parent.left
        right: parent.right
    }

    Column {
        anchors {
            left: parent.left
            right: _icon.visible ? _icon.left : parent.right
            rightMargin: Theme.horizontalPageMargin
            leftMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }

        Label {
            id: _title

            anchors {
                left: parent.left
                right: parent.right
            }

            truncationMode: TruncationMode.Fade
            color: root.invalid ?
                       root.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor :
                       root.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
    }

    FileIcon {
        id: _icon
        anchors {
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        title: root.invalid ? "?" : root.zimTitle
        width: Theme.iconSizeSmallPlus
        height: Theme.iconSizeSmallPlus
    }
}
