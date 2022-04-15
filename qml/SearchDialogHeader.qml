/* Copyright (C) 2018-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

FocusScope {
    id: root

    property int noSearchCount: 10
    property bool search: true
    property bool canCancel: true
    property var dialog
    property var view

    height: column.height

    onHeightChanged: view.scrollToTop()

    Column {
        id: column
        width: parent.width
        Behavior on width { FadeAnimation {} }

        DialogHeader {
            id: header
            dialog: root.dialog
            spacing: 0
            cancelText: root.canCancel ? header.defaultCancelText : ""
            acceptText: {
                var count = root.view.model.selectedCount
                return count > 0 ? qsTr("%n selected", "", count) : header.defaultAcceptText
            }
        }

        SearchField {
            id: searchField
            width: parent.width
            opacity: (root.search && (root.view.model.filter.length !== 0 ||
                     root.view.model.count > root.noSearchCount)) ? 1.0 : 0.0
            visible: opacity > 0.0
            Behavior on opacity { FadeAnimation {} }
            text: root.view.model.filter

            onActiveFocusChanged: {
                if (activeFocus) root.view.currentIndex = -1
            }

            onTextChanged: {
                root.view.model.filter = text.trim()
            }

            Connections {
                target: root.view.model
                onFilterChanged: searchField.text = root.view.model.filter
            }
        }
    }
}
