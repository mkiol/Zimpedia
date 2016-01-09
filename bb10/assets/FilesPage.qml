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

Page {
    id: root
    
    attachedObjects: [
        FileModel {
            id: fileModel
        },
        AbstractItemModel {
            id: bbFileModel
            sourceModel: fileModel
        }
    ]
    
    titleBar: TitleBar {
        title: qsTr("Choose ZIM file")
    }

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
                    StandardListItem {
                        title: ListItemData.name
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                settings.zimFile = chosenItem.id;
                zimServer.loadZimFile();
            }
            
            accessibility.name: "List of ZIM files"
        }
        
        ViewPlaceholder {
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("No files were found")
            visible: bbFileModel.count == 0 && !fileModel.searching
        }
        
        ActivityIndicator {
            enabled: fileModel.searching
            running: enabled
            visible: enabled
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredHeight: ui.du(10)
            preferredWidth: ui.du(10)
            accessibility.name: "Searching ZIM files"
        }
        
        Container {
            leftPadding: ui.du(4)
            rightPadding: ui.du(4)
            topPadding: ui.du(4)
            bottomPadding: ui.du(4)
            
            visible: bbFileModel.count == 0 && !fileModel.searching
            
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            
            Label {
                textFormat: TextFormat.Html
                multiline: true
                text: qsTr("The ZIM is an open file format that stores wiki content for offline usage. The collection of nice wikis can be downloaded from <a href='http://www.kiwix.org/wiki/Content_in_all_languages'>this page</a>. If you already have some ZIM files, put them to any folder you like under your home directory or SD card.")
            }
        }
    }
}