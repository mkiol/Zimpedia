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

Dialog {
    id: root

    property alias title: _title.text
    property string url
    property string zimTitle
    property string zimLanguage
    property string zimUuid
    property bool valid
    property string favicon

    canAccept: title.trim() !== "" && url.trim() !== "" && zimUuid.trim() !== ""

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
            }

            spacing: Theme.paddingLarge

            DialogHeader {
                //title: qsTr("Edit bookmark")
                acceptText: qsTr("Save")
            }

            TextField {
                id: _title
                anchors {
                    left: parent.left
                    right: parent.right
                }
                label: qsTr("Title")
                placeholderText: qsTr("Title")
            }

            ListItem {
                id: _zim
                anchors {
                    left: parent.left
                    right: parent.right
                }

                contentHeight: Theme.itemSizeMedium

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Change")
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("BookmarkFileChoose.qml"),{"uuid": root.uuid});
                        }
                    }
                }

                Icon {
                    id: icon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }
                    showPlaceholder: true
                    source: root.valid ? root.favicon : ""
                    text: root.valid ? root.zimTitle : "?"
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
                        color: _zim.down ? Theme.highlightColor : Theme.primaryColor
                        text: root.valid ? root.zimTitle + (root.zimLanguage === "" ? "" : " (" + root.zimLanguage + ")") :
                                            qsTr("ZIM file is missing")
                        font.italic: !root.valid
                    }
                }

                onClicked: {
                    var obj = pageStack.push(Qt.resolvedUrl("BookmarkFileChoose.qml"),{"uuid": root.zimUuid});
                    obj.uuidChanged.connect(function() {
                        //console.log("UUID changed, old=" + root.zimUuid + " new="+obj.uuid);
                        root.favicon = obj.favicon
                        root.zimUuid = obj.uuid
                        root.zimTitle = obj.title
                        root.zimLanguage = obj.language
                        root.valid = true
                    })
                }
            }

            SectionHeader {
                text: qsTr("URL")
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.horizontalPageMargin
                    rightMargin: Theme.horizontalPageMargin
                }
                wrapMode: Text.WrapAnywhere
                text: bookmarks.changeUuid(root.url, root.zimUuid)
                color: Theme.highlightColor
            }

            Spacer {}
        }

        VerticalScrollDecorator {}
    }

    onAccepted: {
        var newUrl = bookmarks.changeUuid(root.url, root.zimUuid)
        if (bookmarks.validateUrl(newUrl)) {
            /*console.log("title: " + root.title)
            console.log("url: " + newUrl)
            console.log("favicon: " + root.favicon)
            console.log("zimTitle: " + root.zimTitle)
            console.log("zimLanguage: " + root.zimLanguage)
            console.log("zimUuid: " + root.zimUuid)*/
            bookmarks.updateBookmark(root.url, root.title, newUrl, root.favicon)
        } else {
            notification.show(qsTr("Something went wrong and bookmark can't be updated"))
        }
    }
}
