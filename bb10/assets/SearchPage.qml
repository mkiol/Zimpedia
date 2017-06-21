/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3
import com.kdab.components 1.0

Page {
    id: root
    objectName: "search"
    
    attachedObjects: [
        AbstractItemModel {
            id: bbArticleModel
            sourceModel: articleModel
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
            title: qsTr("Open main page")
            imageSource: "asset:///home.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: zimServer.hasMainPage
            shortcuts: Shortcut {
                key: "m"
            }
            
            onTriggered: {
                var url = zimServer.serverUrl()+"A/mainpage";
                if (settings.browser === 1) {
                    nav.openBrowser(url);
                } else {
                    var obj = Qt.webviewPageDefinition.createObject();
                    obj.url = url;
                    obj.init();
                    nav.push(obj);
                }
            }
        },
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
            title: qsTr("Change ZIM file")
            imageSource: "asset:///file.png"
            ActionBar.placement: ActionBarPlacement.Default
            shortcuts: Shortcut {
                key: "f"
            }
            
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
                        
                        contextActions: [
                            ActionSet {
                                //title: ListItemData.title
                                ActionItem {
                                    title: qsTr("Open in built-in viewer")
                                    imageSource: "asset:///webview.png"
                                    onTriggered: {
                                        var obj = Qt.webviewPageDefinition.createObject();
                                        obj.url = ListItemData.url;
                                        obj.init();
                                        Qt.nav.push(obj);
                                    }
                                    shortcuts: Shortcut {
                                        key: "v"
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Open in browser")
                                    imageSource: "asset:///browser.png"
                                    onTriggered: {
                                        Qt.nav.openBrowser(ListItemData.url);
                                    }
                                    shortcuts: Shortcut {
                                        key: "b"
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Add to bookmarks")
                                    imageSource: "asset:///add_bookmark.png"
                                    onTriggered: {
                                        Qt.bookmarks.addBookmark(ListItemData.title, ListItemData.url, Qt.zimServer.favicon)
                                    }
                                    shortcuts: Shortcut {
                                        key: "a"
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                if (chosenItem.url) {
                    if (settings.browser === 0) {
                        var obj = webviewPageDefinition.createObject();
                        obj.url = chosenItem.url;
                        obj.init();
                        nav.push(obj);
                    } else {
                        utils.launchBrowser(chosenItem.url);
                    }
                }
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
