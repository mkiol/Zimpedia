/*
 * Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 * 
 * This file is part of Zimpedia application.
 * 
 * This Source Code Form is subject to the terms of
 * the Mozilla Public License, v.2.0. If a copy of
 * the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Page {
    id: root
    objectName: "bookmarkedit"

    property alias title: _title.text
    property string url
    property string zimTitle
    property string zimLanguage
    property string zimUuid
    property bool valid
    property string favicon

    property bool canAccept: title.trim() !== "" && url.trim() !== "" && zimUuid.trim() !== ""

    signal accepted

    function accept() {
        var newUrl = bookmarks.changeUuid(root.url, root.zimUuid)
        if (bookmarks.validateUrl(newUrl)) {
            bookmarks.updateBookmark(root.url, root.title, newUrl, root.favicon);
            accepted();
        } else {
            notification.show(qsTr("Something went wrong and bookmark can't be updated"));
        }
    }

    actions: [
        ActionItem {
            title: qsTr("Copy URL")
            imageSource: "asset:///copy_link.png"
            onTriggered: {
                notification.show(qsTr("URL was copied to the clipboard"));
                utils.copyToClipboard(bookmarks.changeUuid(root.url, root.zimUuid));
            }
        }
    ]

    titleBar: TitleBar {
        acceptAction: ActionItem {
            title: qsTr("Save")
            enabled: root.canAccept
            onTriggered: {
                accept();
                nav.pop();
            }
        }
        dismissAction: ActionItem {
            title: qsTr("Cancel")
            onTriggered: {
                nav.pop();
            }
        }
        title: qsTr("Edit bookmark")
        appearance: TitleBarAppearance.Plain
    }

    shortcuts: [
        Shortcut {
            key: "Backspace"
            onTriggered: {
                if (nav.count() > 1)
                    nav.pop();
            }
        },
        Shortcut {
            key: "Delete"
            onTriggered: {
                if (nav.count() > 1)
                    nav.pop();
            }
        }
    ]

    ScrollView {
        Container {
            Container {
                leftPadding: ui.du(2)
                rightPadding: ui.du(2)
                topPadding: ui.du(2)
                bottomPadding: ui.du(2)

                TextField {
                    id: _title
                    hintText: qsTr("Title")
                    textStyle.base: SystemDefaults.TextStyles.PrimaryText
                }

                WebImageListItem {
                    showLine: false
                    imageSource: root.valid ? root.favicon : ""
                    titleColor: ui.palette.secondaryTextOnPlain
                    placeholderText: root.valid ? root.zimTitle : "?"
                    title: root.valid ? root.zimTitle + (root.zimLanguage === "" ? "" : " (" + root.zimLanguage + ")") : qsTr("ZIM file is missing")
                    onClicked: {
                        var obj = bookmarkFileChooseDefinition.createObject();
                        obj.uuid = root.zimUuid;
                        obj.uuidChanged.connect(function() {
                                //console.log("UUID changed, old=" + root.zimUuid + " new=" + obj.uuid);
                                root.favicon = obj.favicon
                                root.zimUuid = obj.uuid
                                root.zimTitle = obj.title
                                root.zimLanguage = obj.language
                                root.valid = true
                            })
                        nav.push(obj);
                    }
                }
            }

            Header {
                title: qsTr("URL")
            }

            Container {
                leftPadding: ui.du(2)
                rightPadding: ui.du(2)
                topPadding: ui.du(2)
                bottomPadding: ui.du(2)

                Label {
                    text: bookmarks.changeUuid(root.url, root.zimUuid)
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    textStyle.fontStyle: FontStyle.Italic
                    textStyle.color: ui.palette.secondaryTextOnPlain
                    multiline: true
                }
            }
        }
    }
}
