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

PullDownMenu {
    id: root

    property alias showChangeZIM: changeZIMItem.visible
    property alias showOpenMainPage: openMainPageItem.visible
    property bool showTitle: true
    signal openMainPageClicked

    function fileName(path) {
        return path.replace(/^.*[\\\/]/, '');
    }

    MenuItem {
        text: qsTr("About")
        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
    }

    MenuItem {
        text: qsTr("Settings")
        onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
    }

    MenuItem {
        id: changeZIMItem
        text: qsTr("Change ZIM file")
        onClicked: pageStack.push(Qt.resolvedUrl("FilesPage.qml"))
    }

    MenuItem {
        id: openMainPageItem
        text: qsTr("Open main page")
        visible: zimServer.hasMainPage
        onClicked: openMainPageClicked()
    }

    MenuLabel {
        id: titleItem
        visible: root.showTitle && zimServer.title != ""
        text: zimServer.title + (zimServer.language != "" ? " (" + zimServer.language + ")" : "")
    }
}
