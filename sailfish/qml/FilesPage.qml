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
import harbour.zimpedia.FileModel 1.0

Page {
    id: page

    SilicaListView {
        id: listView
        anchors { top: parent.top; left: parent.left; right: parent.right }
        clip:true

        height: page.height

        model: FileModel {
            id: fileModel
        }

        header: PageHeader {
            title: qsTr("Choose ZIM file")
        }

        delegate: ListItem {
            id: listItem

            //enabled: settings.zimFile !== model.id

            contentHeight: Theme.itemSizeMedium

            Label {
                wrapMode: Text.AlignLeft
                anchors.left: parent.left; anchors.right: parent.right;
                anchors.leftMargin: Theme.horizontalPageMargin; anchors.rightMargin: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Theme.fontSizeMedium
                color: listItem.enabled ? Theme.primaryColor : Theme.secondaryColor
                text: model.name
            }

            onClicked: {
                settings.zimFile = model.id;
                zimServer.loadZimFile();
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !fileModel.searching
            text: qsTr("No files were found")
        }

        Bubble {
            enabled: listView.count == 0 && !fileModel.searching && !menu.active
            text: qsTr("The ZIM is an open file format that stores wiki content for offline usage. The collection of nice wikis can be downloaded from <a href='http://www.kiwix.org/wiki/Content_in_all_languages'>this page</a>. If you already have some ZIM files, put them to any folder you like under your home directory or SD card.")
        }

        PageMenu {
            id: menu
            showChangeZIM: false
        }

    }

    BusyIndicator {
        anchors.centerIn: parent
        running: fileModel.searching
        size: BusyIndicatorSize.Large
    }

    VerticalScrollDecorator {
        flickable: listView
    }
}


