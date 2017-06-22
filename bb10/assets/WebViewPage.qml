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
    objectName: "webview"
    
    property string url
    
    property variant history
    property bool local: false
    property string title: ""
    property variant g_settings: settings
    
    function isLocal(url) {
        return url.lastIndexOf(zimServer.serverUrl()) === 0;
    }
    
    function pushHistory(url) {
        var _history = history;
        if (url !== _history[_history.length - 1]) {
            _history.push(url);
            history = _history;
        }
        /*console.log("addHistory, history.length:" + history.length)
        for (var i = 0; i < history.length; i++) {
            console.log("history["+i+"]:" + history[i]);
        }*/
    }
    
    function popHistory() {
        var _history = history;
        var url = _history[_history.length-2];
        _history.pop();
        history = _history;
        return url;
    }
    
    function init() {
        progressIndicator.show(qsTr("Loading page content..."));
        var _history = [];
        history = _history;
        navigate(url);
    }
    
    function navigate(url) {
        local = isLocal(url);
        title = local ? zimServer.getTitleFromUrl(url) : "";
        console.log("Opening " + (local ? "local" : "non-local") + " url: " + url);
        pushHistory(url);
        view.url = url;
    }
    
    function navigateBack() {
        //console.log("history.length:" + history.length)
        if (history.length > 1) {
            navigate(popHistory());
        } else {
            nav.pop();
        }
    }
    
    function spaceHandler(shift) {
        // ActionBar height: ui.du(12)
        var scale = scrollView.contentScale;
        var x = scrollView.viewableArea.x;
        var y = scrollView.viewableArea.y;
        var dy = scrollView.viewableArea.height - ui.du(18) * (1/scale);
        if (shift)
            y -= dy;
        else
            y += dy;
        scrollView.scrollToPoint(x, y);
    }
    
    function updateZoom(delta) {
        var zoom = settings.zoom;
        if (zoom + delta <= 0.5)
            settings.zoom = 0.5;
        else if (zoom + delta >= 2.0)
            settings.zoom = 2.0;
        else
            settings.zoom = zoom + delta;
        
        var theme = { "zoom": settings.zoom };
        postMessage("theme_set", { "theme": theme });
        postMessage("theme_update_scale");
        var z = Math.round(settings.zoom * 100).toString() + "%";
        baner.show(z);
    }
    
    shortcuts: [
        Shortcut {
            key: "Backspace"
            onTriggered: {
                navigateBack()
            }
        },
        Shortcut {
            key: "Space"
            onTriggered: spaceHandler(false)
        },
        Shortcut {
            key: "Shift+Space"
            onTriggered: spaceHandler(true)
        },
        Shortcut {
            key: "Delete"
            onTriggered: {
                navigateBack()
            }
        }
    ]

    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    actionBarVisibility: ChromeVisibility.Overlay
    
    function messageReceivedHandler(message) {
        if (message.type === "inited") {
            updateZoom(0);
        }
    }
    
    function postMessage(message, data) {
        var objStr = JSON.stringify({ "type": message, "data": data }).replace(/[\'']/g, "\\'");
        var script = "ZimpediaMessageListener.onMessage('"+objStr+"');";
        console.log("postMessage: " + script);
        view.evaluateJavaScript(script);
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                navigateBack();
            }
        }
    }

    actions: [
        ActionItem {
            title: qsTr("Add to bookmarks")
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///add_bookmark.png"
            enabled: root.title !== ""
            onTriggered: {
                console.log("title: " + root.title + ", url: " + root.history[root.history.length-1]);
                bookmarks.addBookmark(root.title, root.history[root.history.length-1], zimServer.favicon);
            }
            shortcuts: Shortcut {
                key: "a"
            }
        },
        /*InvokeActionItem {
            title: qsTr("Share")
            query {
                mimeType: "text/plain"
                invokeActionId: "bb.action.SHARE"
            }
            onTriggered: {
                data = root.onlineUrl;
            }
        },*/
        ActionItem {
            title: qsTr("Open in browser")
            imageSource: "asset:///browser.png"
            onTriggered: {
                var url = root.history[root.history.length-1];
                nav.openBrowser(url);
            }
            shortcuts: Shortcut {
                key: "b"
            }
        },
        ActionItem {
            title: qsTr("Increase font")
            enabled: root.scalingPossible || root.readerMode
            imageSource: "asset:///fontup.png"
            onTriggered: {
                root.updateZoom(0.1)
            }
            shortcuts: Shortcut {
                key: "f"
            }
        },
        ActionItem {
            title: qsTr("Decrease font")
            enabled: root.scalingPossible || root.readerMode
            imageSource: "asset:///fontdown.png"
            onTriggered: {
                root.updateZoom(-0.1)
            }
            shortcuts: Shortcut {
                key: "Shift+f"
            }
        }
    ]

    Container {
        layout: DockLayout {}
        background: Color.Black

        ScrollView {
            id: scrollView
            
            scrollViewProperties {
                scrollMode: ScrollMode.Both
                pinchToZoomEnabled: true
            }

            shortcuts: [
                Shortcut {
                    key: "Space"
                    onTriggered: spaceHandler(false)
                },
                Shortcut {
                    key: "Shift+Space"
                    onTriggered: spaceHandler(true)
                }
            ]

            scrollRole: ScrollRole.Main
            builtInShortcutsEnabled: true

            WebView {
                id: view
                
                property bool script1Done: false
                property bool script2Done: false

                settings.viewport: {
                    "width": "device-width",
                    "initial-scale": Math.round(settings.zoom * 10 ) / 10
                }
                
                function scrollToTop() {
                    scrollView.zoomToPoint(0, 0, minContentScale, ScrollAnimation.None);
                }
                
                onUrlChanged: {
                    //console.log("onUrlChanged: " + url);
                    script1Done = false;
                    script2Done = false;
                }
                
                onLoadProgressChanged: {
                    progressIndicator.progress = loadProgress / 100.0;
                    
                    if (loadProgress > 40 && !script1Done) {
                        script1Done = true;
                        view.evaluateJavaScript(utils.readAsset("js/Zimpedia.js"));
                        view.evaluateJavaScript(utils.readAsset("js/Console.js"));
                        view.evaluateJavaScript(utils.readAsset("js/MessageListener.js"));
                        view.evaluateJavaScript(utils.readAsset("js/Theme.js"));
                    }
                    if (loadProgress > 90 && !script2Done) {
                        script2Done = true;
                        view.evaluateJavaScript(utils.readAsset("js/init.js"));
                    }
                }
                
                onMinContentScaleChanged: {
                    scrollView.scrollViewProperties.minContentScale = minContentScale;
                    scrollView.zoomToPoint(0, 0, minContentScale, ScrollAnimation.None)
                }
                
                onMaxContentScaleChanged: {
                    scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                }
                  
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Started) {
                        progressIndicator.show(qsTr("Loading page content..."));
                    } else if (loadRequest.status == WebLoadStatus.Succeeded) {
                        console.log("loadRequest.status == WebLoadStatus.Succeeded")
                        progressIndicator.hide();
                    } else if (loadRequest.status == WebLoadStatus.Failed) {
                        progressIndicator.hide();
                        notification.show(qsTr("Failed to load page content"));
                    }
                }

                onNavigationRequested: {
                    if (request.navigationType === WebNavigationType.LinkClicked || 
                        request.navigationType === WebNavigationType.OpenWindow) {
                        var url = request.url.toString();
                        root.navigate(url);
                        request.action = WebNavigationRequestAction.Ignore;
                    }
                }

                onMessageReceived: {
                    console.log("onMessageReceived data:", message.data)
                    root.messageReceivedHandler(JSON.parse(message.data))
                }
                
                onCreationCompleted: {
                    storage.clear();
                    storage.clearCookies();
                }
            }
        }
        
        TempBaner {
            id: baner
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }

        Container {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Left

            ProgressPanel {
                id: progressIndicator
                open: false
                onCancelClicked: {
                    view.stop();
                }
            }
        }
    }
}
