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

    function bytesToSize(bytes) {
       var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
       if (bytes == 0) return '0 Byte';
       var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
       return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[i];
    }

    function folderName(dir) {
        if (dir.lastIndexOf("/home/nemo/") === 0) {
            return dir.replace("/home/nemo/", "~/");
        }
        if (dir.lastIndexOf("/media/sdcard/") === 0) {
            return dir.replace(/\/media\/sdcard\/[^\/]*\//i, "[SD]/");
        }

        return dir
    }

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

            property bool active: settings.zimFile === model.id

            contentHeight: Theme.itemSizeMedium

            Column {
                anchors.left: parent.left; anchors.right: parent.right;
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter; anchors.rightMargin: Theme.horizontalPageMargin
                spacing: Theme.paddingSmall

                Label {
                    truncationMode: TruncationMode.Fade
                    anchors.left: parent.left; anchors.right: parent.right;
                    font.pixelSize: Theme.fontSizeMedium
                    color: listItem.active ? Theme.highlightColor : Theme.primaryColor
                    text: model.name
                }

                Label {
                    truncationMode: TruncationMode.Fade
                    anchors.left: parent.left; anchors.right: parent.right;
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: listItem.active ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    text: bytesToSize(model.size) + " • " + folderName(model.dir)
                }
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


