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

ApplicationWindow {
    id: app

    cover: Qt.resolvedUrl("CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    function reload(url, title) {
        console.log("reload:", url, title, zimServer.loaded)
        if (zimServer.loaded) {
            if (url !== "" && title !== "") {
                pageStack.replaceAbove(null,Qt.resolvedUrl("SearchPage.qml"), {}, PageStackAction.Immediate)
                if (settings.browser === 0) {
                    pageStack.push(Qt.resolvedUrl("WebViewPage.qml"),{"url": url});
                } else {
                    openUrlEntryInBrowser(url)
                }
            } else {
                pageStack.replaceAbove(null,Qt.resolvedUrl("SearchPage.qml"))
            }
        } else {
            pageStack.replaceAbove(null,Qt.resolvedUrl("FilesPage.qml"))
        }
    }

    Connections {
        target: zimServer
        onLoadedChanged: reload("", "")
        onUrlReady: reload(url, title)
    }

    Connections {
        target: bookmarkModel
        onBookmarkAdded: notification.show(qsTr("Bookmark added"))
        onBookmarkExists: notification.show(qsTr("Bookmark already exists"))
        onBookmarkUpdated: notification.show(qsTr("Bookmark updated"))
    }

    Component.onCompleted: {
        console.log("=== QML component completed ===")
        zimServer.loadZimFile();
    }

    Notification {
        id: notification
    }
}

