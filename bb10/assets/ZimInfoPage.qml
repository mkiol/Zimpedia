/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import net.mkiol.zimpedia.ZimMetaDataReader 1.0
import org.labsquare 1.0
import "tools.js" as Tools

Page {
    id: root
    objectName: "ziminfo"
    
    property string title
    property string path
    property string icon
    
    property int imageSize: ui.du(11)
    
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
    
    attachedObjects: [
        ZimMetaDataReader {
            id: zim
            path: root.path
        }
    ]
    
    titleBar: TitleBar {
        title: qsTr("Details")
    }

    ScrollView {
        
        Container {
            preferredWidth: Qt.display.pixelSize.width
            verticalAlignment: VerticalAlignment.Center
            
            Container {
                topPadding: ui.du(4)
            }
            
            Icon {
                horizontalAlignment: HorizontalAlignment.Center
                size: root.imageSize
                source: root.icon
                showPlaceholder: true
                text: root.title
            }
            
            AttValue {
                att: qsTr("Name")
                value: zim.name
            }
            
            AttValue {
                att: qsTr("Title")
                value: root.title
            }
            
            AttValue {
                att: qsTr("Date")
                value: zim.date
            }
            
            AttValue {
                att: qsTr("Creator")
                value: zim.creator
            }
            
            AttValue {
                att: qsTr("Publisher")
                value: zim.creator
            }
            
            AttValue {
                att: qsTr("Description")
                value: zim.description
            }
            
            AttValue {
                att: qsTr("Language")
                value: zim.language
            }
            
            AttValue {
                att: qsTr("Tags")
                value: zim.tags
            }
            
            AttValue {
                att: qsTr("Source")
                value: zim.source
            }
            
            AttValue {
                att: qsTr("Article count")
                value: zim.articleCount
            }
            
            AttValue {
                att: qsTr("Filename")
                value: zim.filename
            }
            
            AttValue {
                att: qsTr("Directory")
                value: Tools.friendlyDir(zim.path, utils.homeDir())
            }
            
            AttValue {
                att: qsTr("Size")
                value: Tools.bytesToSize(zim.size)
            }
            
            AttValue {
                att: qsTr("Checksum")
                value: zim.checksum
            }

        }
    }
}
