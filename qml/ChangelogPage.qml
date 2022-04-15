/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: root

    SilicaFlickable {
        id: flick
        anchors.fill: parent
        contentHeight: content.height

        Column {
            id: content

            width: root.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Changes")
            }

            SectionHeader {
                text: qsTr("Version %1").arg("3.0")
            }

            LogItem {
                title: "Search multiple archives"
                description: "You can select multiple ZIM archives and search all of them at once."
            }

            LogItem {
                title: "Sandboxing"
                description: "SailJail is now enabled. Due to sandboxing, " +
                             "app can only open ZIM files from Documents and Download folders and SD card."
            }

            LogItem {
                title: "LibZIM update to version 7.2.0"
                description: "LibZIM and other dependent libraries have been " +
                             "updated to the most recent versions."
            }

        }
    }

    VerticalScrollDecorator {
        flickable: flick
    }
}
