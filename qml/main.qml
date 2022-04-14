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

    initialPage: Qt.resolvedUrl("SearchPage.qml")
    cover: Qt.resolvedUrl("CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    function setSearchPage() {
        pageStack.replaceAbove(null, Qt.resolvedUrl("SearchPage.qml"), {}, PageStackAction.Immediate);
    }

    function reload(url, title) {
        if (zimServer.loaded && url !== "" && title !== "") {
            setSearchPage();
            if (settings.browser === 0) {
                pageStack.push(Qt.resolvedUrl("WebViewPage.qml"), {"url": url});
            } else {
                openUrlEntryInBrowser(url)
            }
        }
    }


    Connections {
        target: zimServer
        onUrlReady: reload(url, title)
    }

    Connections {
        target: fileModel
        onNoFilesFound: {
            setSearchPage();
            pageStack.push(Qt.resolvedUrl("FilesPage.qml"), {}, PageStackAction.Immediate);
        }
    }

    Connections {
        target: bookmarkModel
        onBookmarkAdded: notification.show(qsTr("Bookmark added"))
        onBookmarkExists: notification.show(qsTr("Bookmark already exists"))
        onBookmarkUpdated: notification.show(qsTr("Bookmark updated"))
    }

    Component.onCompleted: {
        console.log("=== QML component completed ===")
        fileModel.updateModel();
        zimServer.loadZim();
    }

    Notification {
        id: notification
    }
}
