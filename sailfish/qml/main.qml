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

    //initialPage: Component { FirstPage { } }
    cover: Qt.resolvedUrl("CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    function reload() {
        if (!zimServer.listening) {
            Qt.quit();
            return;
        }
        if (zimServer.loaded) {
            pageStack.replaceAbove(null,Qt.resolvedUrl("SearchPage.qml"))
        } else {
            pageStack.replaceAbove(null,Qt.resolvedUrl("FilesPage.qml"))
        }
    }

    /*Connections {
        target: settings
        onZimFileChanged: zimServer.loadZimFile();
    }*/

    Connections {
        target: zimServer
        onLoadedChanged: reload()
    }

    Component.onCompleted: {
        zimServer.loadZimFile();
    }
}

