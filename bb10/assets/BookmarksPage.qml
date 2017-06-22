/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import net.mkiol.zimpedia.BookmarkModel 1.0
import com.kdab.components 1.0

Page {
    id: root
    objectName: "bookmarks"
    
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
    
    onCreationCompleted: {
        Qt.bookmarkModel = bookmarkModel;
        bookmarkModel.busyChanged.connect(resetModel);
    }
    
    attachedObjects: [
        BookmarkModel {
            id: bookmarkModel
        },
        AbstractItemModel {
            id: bbBookmarkModel
            sourceModel: bookmarkModel
        }
    ]
    
    function resetModel() {
        bbBookmarkModel.resetSourceModel();
    }
    
    titleBar: TitleBar {
        title: qsTr("Bookmarks")
    }
    
    Container {
        layout: DockLayout {
        }
        
        ListView {
            id: listView
            verticalAlignment: VerticalAlignment.Center
            scrollRole: ScrollRole.Main
            dataModel: bbBookmarkModel
            layout: StackListLayout {
                headerMode: ListHeaderMode.None
            }
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    WebImageListItem {
                        title: ListItemData.title
                        subtitle: ListItemData.valid ? ListItemData.zimTitle + (ListItemData.zimLanguage === "" ? "" : " (" + ListItemData.zimLanguage + ")") : qsTr("ZIM file is missing")
                        imageSource: ListItemData.valid ? ListItemData.favicon : ""
                        placeholderText: ListItemData.valid ? ListItemData.title : "?"
                        titleColor: ListItemData.valid ? ui.palette.text : ui.palette.secondaryTextOnPlain
                        
                        contextActions: [
                            ActionSet {
                                //title: ListItemData.title
                                ActionItem {
                                    title: qsTr("Edit")
                                    imageSource: "asset:///edit.png"
                                    onTriggered: {
                                        var obj = Qt.bookmarkEditPageDefinition.createObject();
                                        obj.url = ListItemData.url;
                                        obj.title = ListItemData.title;
                                        obj.favicon = ListItemData.favicon;
                                        obj.valid = ListItemData.valid;
                                        obj.zimTitle = ListItemData.zimTitle;
                                        obj.zimLanguage = ListItemData.zimLanguage;
                                        obj.zimUuid = ListItemData.zimUuid;
                                        obj.accepted.connect(function(){Qt.bookmarkModel.refreshModel()})
                                        Qt.nav.push(obj);
                                    }
                                    shortcuts: Shortcut {
                                        key: "e"
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Delete")
                                    imageSource: "asset:///delete.png"
                                    onTriggered: {
                                        Qt.bookmarks.deleteBookmark(ListItemData.url)
                                        Qt.bookmarkModel.refreshModel()
                                    }
                                    shortcuts: Shortcut {
                                        key: "Delete"
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                if (chosenItem.valid)
                    zimServer.openUrl(chosenItem.url, chosenItem.title);
                else
                    notification.show(qsTr("Bookmark's ZIM file is missing"))
            }
            
            accessibility.name: "List of bookmarks"
        }
        
        ViewPlaceholder {
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("No bookmarks")
            visible: bbBookmarkModel.count == 0 && !bookmarkModel.busy
        }
        
        ActivityIndicator {
            enabled: bookmarkModel.busy
            running: enabled
            visible: enabled
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredHeight: ui.du(10)
            preferredWidth: ui.du(10)
        }
    }
}
