/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: root
    objectName: "bookmarks"

    property real preferredItemHeight: root && root.isLandscape ?
                                           Theme.itemSizeSmall :
                                           Theme.itemSizeLarge

    Component.onCompleted: bookmarkModel.updateModel()

    SilicaListView {
        id: listView

        anchors.fill: parent

        currentIndex: -1

        model: bookmarkModel

        header: PageHeader {
            title: qsTr("Bookmarks")
        }

        delegate: ListItem {
            id: listItem

            visible: !bookmarkModel.busy && !fileModel.busy

            contentHeight: Theme.itemSizeMedium

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Edit")
                    onClicked: {
                        var obj = pageStack.push(Qt.resolvedUrl("BookmarkEditPage.qml"),
                                    {"url": url,
                                     "title": model.title,
                                     "favicon": model.favicon,
                                     "valid": model.valid,
                                     "zimTitle": model.zimTitle,
                                     "zimLanguage": model.zimLanguage,
                                     "zimUuid": model.zimUuid,
                                    });
                    }
                }

                MenuItem {
                    text: qsTr("Delete")
                    onClicked: {
                        bookmarkModel.deleteBookmark(model.url)
                    }
                }
            }

            FileIcon {
                id: icon
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                showPlaceholder: true
                source: model.valid ? model.favicon : ""
                text: model.valid ? model.zimTitle : "?"
                height: Theme.iconSizeMedium
                width: Theme.iconSizeMedium
            }

            Column {
                anchors {
                    left: icon.right
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                spacing: Theme.paddingSmall

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeMedium
                    color: listItem.active || listItem.down ? Theme.highlightColor : model.valid ? Theme.primaryColor : Theme.secondaryColor
                    text: model.title
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: listItem.active || listItem.down ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    text: model.valid ? model.zimTitle + (model.zimLanguage === "" ? "" : " (" + model.zimLanguage + ")") :
                                        qsTr("ZIM file is missing")
                    font.italic: !model.valid
                }
            }

            onClicked: {
                if (model.valid)
                    zimServer.openUrl(model.url, model.title)
                else
                    notification.show(qsTr("Bookmark's ZIM file is missing"))
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0 && !bookmarkModel.busy && !fileModel.busy
            text: qsTr("No bookmarks")
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: bookmarkModel.busy || fileModel.busy
        size: BusyIndicatorSize.Large
    }

    VerticalScrollDecorator {
        flickable: listView
    }
}
