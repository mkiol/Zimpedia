/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import net.mkiol.zimpedia.FileModel 1.0
import com.kdab.components 1.0
import "tools.js" as Tools

Page {
    id: root
    objectName: "bookmarkfiles"
    
    property string uuid
    property string title
    property string language
    property string favicon
    
    onCreationCompleted: {
        Qt.root = root;
        fileModel.init(false);
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
        title: qsTr("Change ZIM file")
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
                        property bool active: ListItemData.checksum === Qt.root.uuid
                        title: ListItemData.title + (ListItemData.language === "" ? "" : " (" + ListItemData.language + ")")
                        subtitle: Tools.friendlyPath(ListItemData.dir, Qt.utils.homeDir())
                        imageSource: ListItemData.favicon
                        titleColor: active ? ui.palette.primary : ui.palette.text
                        subtitleColor: active ? ui.palette.primaryBase : ui.palette.secondaryTextOnPlain
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                if (chosenItem.checksum) {
                    root.title = chosenItem.title;
                    root.language = chosenItem.language;
                    root.favicon = chosenItem.favicon;
                    root.uuid = chosenItem.checksum;
                }
                nav.pop();
            }
            
            accessibility.name: "List of ZIM files"
        }
        
        ViewPlaceholder {
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("No files were found")
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
    }
}