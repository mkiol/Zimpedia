/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3

Page {
    id: root

    titleBar: TitleBar {
        title: qsTr("About")
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
    
    actions: [
        ActionItem {
            title: qsTr("Changelog")
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///changelog.png"
            onTriggered: {
                var obj = changelogPageDefinition.createObject();
                nav.push(obj);
            }
        }
    ]
    
    ScrollView {

        Container {
            preferredWidth: Qt.display.pixelSize.width
            verticalAlignment: VerticalAlignment.Center
            
            Container {
                topPadding: ui.du(4)
            }

            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                imageSource: "asset:///icon.png"
                accessibility.name: "Zimpedia icon"
            }

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                textStyle.base: SystemDefaults.TextStyles.BigText
                text: APP_NAME
            }

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                textStyle.color: ui.palette.primary
                text: qsTr("Version %1").arg(VERSION)
            }

            PaddedLabel {
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                text: qsTr("Offline reader for Wikipedia and other Wiki projects")
            }

            Container {
                topPadding: ui.du(2)
            }
            
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: qsTr("Website")
                onClicked: {
                    utils.launchBrowser(PAGE);
                }
            }
            
            /*Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: qsTr("Changelog")
                onClicked: {
                    var obj = changelogPageDefinition.createObject();
                    nav.push(obj);
                }
            }*/
            
            Container {
                topPadding: ui.du(2)
            }
            
            Header {
                title: qsTr("Copyright & license")
            }
            
            PaddedLabel {
                textStyle.textAlign: TextAlign.Left
                textFormat: TextFormat.Html
                text: "Copyright &copy; 2016-2017 Michal Kosciesza"
            }
            
            PaddedLabel {
                textStyle.textAlign: TextAlign.Left
                textFormat: TextFormat.Html
                text: qsTr("This software is distributed under the terms of the Mozilla Public License v.2.0")
            }
            
            Container {
                topPadding: utils.du(2)
            }
            
            Header {
                title: qsTr("Third party components copyrights")
            }
            
            /*PaddedLabel {
                textStyle.textAlign: TextAlign.Left
                text: qsTr("%1 utilizes third party open source software. Such third party software is copyrighted by their owners as indicated below.").arg(APP_NAME)
            }*/
            
            PaddedLabel {
                textFormat: TextFormat.Html
                text: "QHTTPServer - Copyright &copy; 2011-2013 Nikhil Marathe"
            }
            
            PaddedLabel {
                textFormat: TextFormat.Html
                text: "ZimLib - Copyright &copy; 2003-2010 Tommi Maekitalo"
            }
            
            PaddedLabel {
                textFormat: TextFormat.Html
                text: "Qjson - Copyright &copy; 2009 Flavio Castelli"
            }
            
            Container {
                topPadding: ui.du(5)
            }
        }
        accessibility.name: "About application"
    }
}
