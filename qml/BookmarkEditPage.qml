/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: root

    property alias title: _title.text
    property url url
    property string zimTitle
    property string zimUuid
    property bool valid
    property url icon

    canAccept: title.trim() !== "" && url.toString().trim() !== ""

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

            DoubleListItem {
                title: root.valid ? root.zimTitle : qsTr("Archive is missing")
                uuid: root.zimUuid
                icon: root.icon
                invalid: !root.valid
                enabled: false
            }

            SectionHeader {
                text: qsTr("URL")
            }

            ListItem {
                id: _urlitem
                contentHeight: _urllabel.height + 2 * Theme.paddingMedium
                menu: _menu
                onClicked: openMenu()

                Label {
                    id: _urllabel
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                        leftMargin: Theme.horizontalPageMargin
                        rightMargin: Theme.horizontalPageMargin
                    }
                    wrapMode: Text.WrapAnywhere
                    text: root.url
                    color: _urlitem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Component {
                    id: _menu
                    ContextMenu {
                        MenuItem {
                            text: qsTr("Copy")
                            onClicked: {
                                utils.copyToClipboard(_urllabel.text)
                            }
                        }
                    }
                }
            }

            Spacer {}
        }

        VerticalScrollDecorator {}
    }

    onAccepted: {
        var newUrl = root.url;
        if (bookmarkModel.validateUrl(newUrl)) {
            bookmarkModel.updateBookmark(root.url, root.title, newUrl)
        } else {
            console.warn("can't update bookmark, invalid new url:", newUrl)
        }
    }
}
