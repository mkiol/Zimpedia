/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Page {
    id: root
    objectName: "changelog"
    
    titleBar: TitleBar {
        title: qsTr("Changelog")
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
    
    Container {
        ScrollView {
            Container {
                
                Header {
                    title: qsTr("Version %1").arg("2.0.0")
                }
                
                Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(2)
                    bottomPadding: ui.du(2)
                    
                    LogItem {
                        title: "Built-in web viewer"
                        description: "Wiki pages can be opened in the built-in web viewer or in an external browser.";
                    }
                    
                    LogItem {
                        title: "Main page"
                        description: "If ZIM file contains main page, bar menu will contain action to open it.";
                    }
                    
                    LogItem {
                        title: "ZIM details view"
                        description: "Context menu has option to show details of ZIM repository.";
                    }
                    
                    LogItem {
                        title: "Bookmarks"
                        description: "Every article can be added to bookmarks. Click on a bookmark will automatically load an appropriate ZIM file.";
                    }
                }
            }
        }
    }
}

