/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

        delegate: ArticleItem {
            hidden: bookmarkModel.busy || fileModel.busy
            title: model.title
            zimTitle: model.valid ? model.zimTitle : qsTr("Archive is missing")
            zimUuid: model.zimUuid
            icon: model.icon
            invalid: !model.valid

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Edit")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("BookmarkEditPage.qml"),
                                       {"url": url,
                                           "title": model.title,
                                           "icon": model.icon,
                                           "valid": model.valid,
                                           "zimTitle": model.zimTitle,
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

            onClicked: {
                if (model.valid) {
                    zimServer.openUrl(model.url, model.title)
                } else {
                    openMenu()
                }
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
