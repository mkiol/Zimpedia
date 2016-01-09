/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3

NavigationPane {
    id: nav
    
    attachedObjects: [
        ComponentDefinition {
            id: searchPageDefinition
            source: "SearchPage.qml"
        },
        ComponentDefinition {
            id: filesPageDefinition
            source: "FilesPage.qml"
        },
        ComponentDefinition {
                    id: settingsPageDefinition
                    source: "SettingsPage.qml"
        },
        ComponentDefinition {
            id: aboutPageDefinition
            source: "AboutPage.qml"
        }
    ]
    
    onCreationCompleted: {
        zimServer.loadedChanged.connect(zimServerloadedChangedHandler);
        zimServer.loadZimFile();
    }
    
    function reload() {
        if (!zimServer.listening) {
            Qt.quit();
            return;
        }
        if (zimServer.loaded) {
            nav.insert(0, searchPageDefinition.createObject());
            nav.navigateTo(nav.at(0));
        } else {
            nav.insert(0, filesPageDefinition.createObject());
            nav.navigateTo(nav.at(0));
        }
    }
    
    function zimServerloadedChangedHandler() {
        reload();
    }
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                nav.push(settingsPageDefinition.createObject());
            }
        }
        helpAction: HelpActionItem {
            title: qsTr("About")
            onTriggered: {
                nav.push(aboutPageDefinition.createObject());
            }
        }
    }

    onPopTransitionEnded: {
        page.destroy();
    }
}
