/* Copyright (C) 2017-2025 Michal Kosciesza <michal@mkiol.net>
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
                text: qsTr("Version %1").arg("3.3")
            }

            LogItem {
                title: "Fix: Some ZIM archives were unusable due to disabled JavaScript."
                description: "Some websites from the ZIM archives require JavaScript to be enabled in order to display their content. " +
                             "To support these ZIM archives, JavaScript has been enabled."
            }

            SectionHeader {
                text: qsTr("Version %1").arg("3.2")
            }

            LogItem {
                title: "Fix: The articles could not be opened in the latest Wikipedia dumps."
                description: "The most recently created Wikipedia ZIM archives were not functioning correctly. This issue has now been resolved."
            }

            LogItem {
                title: "LibZIM update to version 9.3.0"
                description: "LibZIM and other dependent libraries have been " +
                             "updated to the most recent versions."
            }

            SectionHeader {
                text: qsTr("Version %1").arg("3.1")
            }

            LogItem {
                title: "Italian translation"
                description: "App has been translated to Italian. Many thanks to Albano Battistella. "
            }

            LogItem {
                title: "LibZIM update to version 8.2.1"
                description: "LibZIM and other dependent libraries have been " +
                             "updated to the most recent versions."
            }

            LogItem {
                title: "Fixes for bugs in a WebView"
                description: "Annoying bug which made page in a WebView partially visible has been fixed."
            }

            SectionHeader {
                text: qsTr("Version %1").arg("3.0")
            }

            LogItem {
                title: "Improved web viewer"
                description: "Old WebKit based web view is replaced with new Sailfish WebView based on Gecko engine. " +
                             "Thanks to this change many web rendering bugs disappeared and html pages look much better now."
            }

            LogItem {
                title: "Search multiple archives"
                description: "You can select multiple ZIM archives and search all of them at once."
            }

            LogItem {
                title: "Sandboxing"
                description: "SailJail is now explicitly enabled. " +
                             "Due to sandboxing app no longer can read user main directory. " +
                             "Please make sure your ZIM files are in any folder under Downloads or Documents or on SD card."
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
