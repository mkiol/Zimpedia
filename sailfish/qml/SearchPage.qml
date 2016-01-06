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

Page {
    id: page

    property real preferredItemHeight: page && page.isLandscape ? Theme.itemSizeSmall : Theme.itemSizeLarge

    SilicaListView {
        id: listView
        anchors { top: parent.top; left: parent.left; right: parent.right }
        clip:true

        height: page.height - panel.height
        Behavior on height {
            NumberAnimation {
                duration: 500; easing.type: Easing.OutQuad
            }
        }

        model: articleModel

        header: Spacer {}

        /*header: PageHeader {
            title: qsTr("")
        }*/

        delegate: ListItem {
            id: listItem

            contentHeight: Theme.itemSizeMedium

            Label {
                wrapMode: Text.AlignLeft
                anchors.left: parent.left; anchors.right: parent.right;
                anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Theme.fontSizeMedium
                text: model.title
            }

            onClicked: {
                //console.log("model.url:",model.url);
                zimServer.setScreenSize(Math.min(page.width,page.height));
                Qt.openUrlExternally(model.url);
            }

            onEntered: {
                listView.focus = true;
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !panel.moving
            text: qsTr("Find article, by typing in the search field")

        }

        PageMenu {}

    }

    VerticalScrollDecorator {
        flickable: listView
    }

    DockedPanel {
        id: panel

        width: parent.width
        height: searchField.height + Theme.paddingLarge //Theme.itemSizeExtraLarge
        open: true

        dock: Dock.Bottom

        TextField {
            id: searchField
            anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
            anchors.left: parent.left; anchors.right: parent.right
            anchors.bottom: parent.bottom
            placeholderText: qsTr("Search")
            onTextChanged: zimServer.findTitle(text);

            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: {
                listView.focus = true;
            }
        }
    }
}


