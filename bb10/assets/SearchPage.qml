/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3
import bb.device 1.3
import com.kdab.components 1.0

Page {
    id: root
    
    attachedObjects: [
        AbstractItemModel {
            id: bbArticleModel
            sourceModel: articleModel
        },
        DisplayInfo {
            id: displayInfo
        }
    ]
    
    onCreationCompleted: {
        zimServer.searchReady.connect(resetModel);
    }
    
    function resetModel() {
        bbArticleModel.resetSourceModel();
    }
    
    titleBar: TitleBar {
        kind: TitleBarKind.TextField
        kindProperties: TextFieldTitleBarKindProperties {
            textField {
                inputMode: TextFieldInputMode.Text
                hintText: qsTr("Enter search entry")

                onTextChanging: {
                    zimServer.findTitle(text)
                }
            }

        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Change ZIM file")
            ActionBar.placement: ActionBarPlacement.Default
            
            onTriggered: {
                var obj = filesPageDefinition.createObject();
                nav.push(obj);
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
            dataModel: bbArticleModel
            layout: StackListLayout {
                headerMode: ListHeaderMode.None
            }
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    StandardListItem {
                        title: ListItemData.title
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                console.log("width:",displayInfo.pixelSize.width);
                zimServer.setScreenSize(displayInfo.pixelSize.width);
                utils.launchBrowser(chosenItem.url);
            }
            
            accessibility.name: "List of titles of articles"
        }
        
        ViewPlaceholder {
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("Find article, by typing in the search field")
            visible: bbArticleModel.count == 0
        }
    }
}
