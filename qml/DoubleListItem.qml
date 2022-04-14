/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
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
    property alias subtitle: _subtitle.text
    property alias uuid: _icon.uuid
    property alias icon: _icon.source
    property bool dimmed: false
    property bool invalid: false
    property bool subForIcon: false

    property bool _iconDisabled: false

    contentHeight: Theme.itemSizeMedium

    anchors {
        left: parent.left
        right: parent.right
    }

    FileIcon {
        id: _icon
        title: root.invalid ? "?" : root.subForIcon ? root.subtitle : root.title
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
    }

    Column {
        width: parent.width

        anchors {
            left: _iconDisabled ? parent.left : _icon.right
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            leftMargin: _iconDisabled ? Theme.horizontalPageMargin : Theme.paddingLarge
            verticalCenter: parent.verticalCenter
        }

        Label {
            id: _title

            anchors {
                left: parent.left
                right: parent.right
            }

            truncationMode: TruncationMode.Fade
            font.italic: root.invalid
            color: root.highlighted || !root.enabled ?
                       root.invalid ? Theme.secondaryHighlightColor : Theme.highlightColor :
                       root.invalid ? Theme.secondaryColor : Theme.primaryColor
        }

        Label {
            id: _subtitle
            visible: text.length > 0

            anchors {
                left: parent.left
                right: parent.right
            }

            font.pixelSize: Theme.fontSizeSmall
            truncationMode: TruncationMode.Fade
            font.italic: root.invalid
            color: root.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
        }
    }
}
