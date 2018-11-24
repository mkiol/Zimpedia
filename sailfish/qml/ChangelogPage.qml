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
                title: qsTr("Changelog")
            }

            SectionHeader {
                text: qsTr("Version %1").arg("2.2.0")
            }

            LogItem {
                title: "Support for the latest ZIM file format"
                description: "The LibZIM library has been updated (v4.0.4). " +
                             "This update brings support for the latest ZIM format.";
            }

            LogItem {
                title: "Full-text search"
                description: "By default when you enter a phase, search only by article title is performed. " +
                             "The new mode enables a search within full article text. " +
                             "It works only when ZIM file contains appropriate full-text index. " +
                             "You can switch between search modes by changing an option in the settings."
            }

            SectionHeader {
                text: qsTr("Version %1").arg("2.0.1")
            }

            LogItem {
                title: "Chinese translation"
                description: "Chinese translation thanks to Jeff Huang";
            }

            LogItem {
                title: "French translation"
                description: "French translation thanks to lutinotmalin";
            }

            SectionHeader {
                text: qsTr("Version %1").arg("2.0.0")
            }

            LogItem {
                title: "Built-in web viewer"
                description: "Wiki pages can be opened in the built-in web viewer or in an external browser.";
            }

            LogItem {
                title: "Night mode"
                description: "Night mode reduces the brightness of websites by inverting colors (heavily inspired and code partially borrowed from harbour-webpirate project).";
            }

            LogItem {
                title: "Main page"
                description: "If ZIM file contains main page, pull-down menu will show option to open it.";
            }

            LogItem {
                title: "ZIM details view"
                description: "Context menu has option to show details of ZIM repository.";
            }

            LogItem {
                title: "Bookmarks"
                description: "Every article can be added to bookmarks. Click on a bookmark will automatically load an appropriate ZIM file.";
            }

            Spacer {}
        }
    }

    VerticalScrollDecorator {
        flickable: flick
    }
}
