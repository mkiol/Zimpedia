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
import bb.system 1.2

NavigationPane {
    id: nav
    
    attachedObjects: [
        DisplayInfo {
            id: display
        },
        Notification {
            id: notification
        },
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
        },
        ComponentDefinition {
            id: zimInfoPageDefinition
            source: "ZimInfoPage.qml"
        },
        ComponentDefinition {
            id: changelogPageDefinition
            source: "ChangelogPage.qml"
        },
        ComponentDefinition {
            id: bookmarksPageDefinition
            source: "BookmarksPage.qml"
        },
        ComponentDefinition {
            id: webviewPageDefinition
            source: "WebViewPage.qml"
        },
        ComponentDefinition {
            id: bookmarkEditPageDefinition
            source: "BookmarkEditPage.qml"
        },
        ComponentDefinition {
            id: bookmarkFileChooseDefinition
            source: "BookmarkFileChoose.qml"
        },
        SystemProgressDialog {
            id: progressDialog
            progress: -1
            defaultButton: null
            body: qsTr("Preparing ZIM file...")
        }
    ]
    
    onCreationCompleted: {
        Qt.utils = utils;
        Qt.zimInfoPageDefinition = zimInfoPageDefinition;
        Qt.webviewPageDefinition = webviewPageDefinition;
        Qt.bookmarkEditPageDefinition = bookmarkEditPageDefinition;
        Qt.nav = nav;
        Qt.zimServer = zimServer;
        Qt.settings = settings;
        Qt.display = display;
        Qt.bookmarks = bookmarks;
        Qt.utils = utils;
        
        bookmarks.bookmarkAdded.connect(function(){notification.show(qsTr("Bookmark has been added"))});
        bookmarks.bookmarkExists.connect(function(){notification.show(qsTr("Bookmark already exists"))});
        bookmarks.bookmarkUpdated.connect(function(){notification.show(qsTr("Bookmark has been updated"))});
        zimServer.busyChanged.connect(function(){
                if (zimServer.busy)
                    progressDialog.show();
                else
                    progressDialog.cancel();
        });
        
        zimServer.loadedChanged.connect(function(){reload("","")});
        zimServer.urlReady.connect(function(url, title){reload(url,title)});
        zimServer.loadZimFile();
    }
    
    onTopChanged: {
        Application.menuEnabled = nav.count() <= 1;
    }
    
    function reload(url, title) {
        if (!zimServer.listening) {
            Qt.quit();
            return;
        }
        if (zimServer.loaded) {
            if (url !== "" && title !== "") {
                nav.insert(0, searchPageDefinition.createObject());
                nav.navigateTo(nav.at(0));
                if (settings.browser === 0) {
                    var obj = webviewPageDefinition.createObject();
                    obj.url = url;
                    obj.init();
                    nav.push(obj);
                } else {
                    utils.launchBrowser(url);
                }
            } else {
                nav.insert(0, searchPageDefinition.createObject());
                nav.navigateTo(nav.at(0));
            }
        } else {
            nav.insert(0, filesPageDefinition.createObject());
            nav.navigateTo(nav.at(0));
        }
    }
    
    function zimServerloadedChangedHandler() {
        reload();
    }
    
    function openBrowser(url) {
        notification.show(qsTr("Launching an external browser..."));
        utils.launchBrowser(encodeURI(url));
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
}
