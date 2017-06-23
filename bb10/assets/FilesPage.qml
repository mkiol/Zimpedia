/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3
import net.mkiol.zimpedia.FileModel 1.0
import com.kdab.components 1.0
import "tools.js" as Tools

Page {
    id: root
    objectName: "files"
    
    property bool showHelp: bbFileModel.count == 0 && !fileModel.busy
    
    onCreationCompleted: {
        fileModel.init(true);
        fileModel.busyChanged.connect(resetModel);
    }
    
    attachedObjects: [
        FileModel {
            id: fileModel
        },
        AbstractItemModel {
            id: bbFileModel
            sourceModel: fileModel
        }
    ]
    
    function resetModel() {
        bbFileModel.resetSourceModel();
    }
    
    titleBar: TitleBar {
        title: qsTr("Choose ZIM file")
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
    
    actions: [
        ActionItem {
            title: qsTr("Bookmarks")
            imageSource: "asset:///bookmark.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: !bookmarks.busy
            shortcuts: Shortcut {
                key: "b"
            }
            
            onTriggered: {
                var obj = bookmarksPageDefinition.createObject();
                nav.push(obj);
            }
        },
        ActionItem {
            title: qsTr("Refresh")
            imageSource: "asset:///refresh.png"
            ActionBar.placement: ActionBarPlacement.Default
            shortcuts: Shortcut {
                key: "r"
            }
            
            onTriggered: {
                fileModel.init(true)
            }
        }
    ]

    Container {
        layout: DockLayout {
        }
        
        ListView {
            id: listView
            verticalAlignment: VerticalAlignment.Center
            scrollRole: ScrollRole.Main
            dataModel: bbFileModel
            layout: StackListLayout {
                headerMode: ListHeaderMode.None
            }
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    WebImageListItem {
                        property bool active: Qt.settings.zimFile === ListItemData.id
                        title: ListItemData.title + (ListItemData.language === "" ? "" : " (" + ListItemData.language + ")")
                        subtitle: Tools.friendlyPath(ListItemData.dir, Qt.utils.homeDir())
                        imageSource: ListItemData.favicon
                        titleColor: active ? ui.palette.primary : ui.palette.text
                        subtitleColor: active ? ui.palette.primaryBase : ui.palette.secondaryTextOnPlain
                        
                        contextActions: [
                            ActionSet {
                                //title: ListItemData.title
                                ActionItem {
                                    title: qsTr("Show details")
                                    imageSource: "asset:///details.png"
                                    onTriggered: {
                                        var obj = Qt.zimInfoPageDefinition.createObject();
                                        obj.title = ListItemData.title;
                                        obj.path = ListItemData.dir;
                                        obj.icon = ListItemData.favicon;
                                        Qt.nav.push(obj);
                                    }
                                    shortcuts: Shortcut {
                                        key: "d"
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                if (chosenItem.id)
                    settings.zimFile = chosenItem.id;
                zimServer.loadZimFile();
            }
            
            accessibility.name: "List of ZIM files"
        }
        
        ViewPlaceholder {
            textFormat: root.showHelp ? TextFormat.Html : TextFormat.Plain
            verticalAlignment: VerticalAlignment.Center
            text: root.showHelp ? 
                qsTr("No files were found") + ". " + 
                qsTr("The ZIM is an open file format that stores wiki content for offline usage. The collection of nice wikis can be downloaded from <a href='http://www.kiwix.org/wiki/Content_in_all_languages'>this page</a>. If you already have some ZIM files, put them to any folder you like under your home directory or SD card.") : 
                qsTr("No files were found")
            visible: bbFileModel.count == 0 && !fileModel.busy
        }
        
        ActivityIndicator {
            enabled: fileModel.busy
            running: enabled
            visible: enabled
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredHeight: ui.du(10)
            preferredWidth: ui.du(10)
            accessibility.name: "Searching ZIM files"
        }
        
        /*Container {
            leftPadding: ui.du(4)
            rightPadding: ui.du(4)
            topPadding: ui.du(4)
            bottomPadding: ui.du(4)
            
            visible: root.showHelp
            
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            
            Label {
                textFormat: TextFormat.Html
                multiline: true
                text: qsTr("The ZIM is an open file format that stores wiki content for offline usage. The collection of nice wikis can be downloaded from <a href='http://www.kiwix.org/wiki/Content_in_all_languages'>this page</a>. If you already have some ZIM files, put them to any folder you like under your home directory or SD card.")
            }
        }*/
    }
}
