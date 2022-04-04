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

CoverBackground {

    function getFirstPage(page) {
        page = page ? page : pageStack.currentPage
        var p_page = pageStack.previousPage(page)
        if (p_page)
            return getFirstPage(p_page)
        else
            return page
    }

    function showBookmarks() {
        var firstPage = getFirstPage()
        if (firstPage === pageStack.currentPage)
            pageStack.push(Qt.resolvedUrl("BookmarksPage.qml"),{},PageStackAction.Immediate)
        else
            pageStack.replaceAbove(firstPage, Qt.resolvedUrl("BookmarksPage.qml"),{},PageStackAction.Immediate)
        app.activate()
    }

    function showSearch() {
        if (pageStack.currentPage.objectName !== "search") {
            if (zimServer.loaded) {
                pageStack.replaceAbove(null,Qt.resolvedUrl("SearchPage.qml"),{},PageStackAction.Immediate)
            } else {
                pageStack.replaceAbove(null,Qt.resolvedUrl("FilesPage.qml"),{},PageStackAction.Immediate)
            }
        }
        app.activate()
    }

    CoverPlaceholder {
        text: zimServer.loaded ? zimServer.title + (zimServer.language === "" ? "" : " (" + zimServer.language + ")") : APP_NAME
        icon.source: zimServer.loaded ? zimServer.favicon : "image://icons/icon-a-zimpedia"
        icon.width: Theme.iconSizeLarge
        icon.height: Theme.iconSizeLarge
    }

    CoverActionList {
        CoverAction {
            iconSource: "image://theme/icon-cover-search"
            onTriggered: showSearch()
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-favorite"
            onTriggered: showBookmarks()
        }
    }
}
