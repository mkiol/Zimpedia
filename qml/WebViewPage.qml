/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

// Some ideas heavily inspired and partially borrowed from
// harbour-webpirate project (https://github.com/Dax89/harbour-webpirate)

import QtQuick 2.1
import Sailfish.Silica 1.0
import QtWebKit 3.0

Page {
    id: root
    objectName: "webview"

    property string url

    property variant _settings: settings
    property bool nightMode: false
    property bool nightModePossible: true
    property int toolbarHideTime: 4000
    property variant history
    property bool local: false
    property string title: ""

    function isLocal(url) {
        return url.lastIndexOf(zimServer.serverUrl()) === 0
    }

    function init() {
        history = []
        history.push(url)
        navigate(url)
    }

    function openUrlEntryInBrowser(url) {
        notification.show(qsTr("Launching an external browser..."))
        Qt.openUrlExternally(url)
    }

    function navigate(url) {
        local = isLocal(url)
        console.log("Opening " + (local ? "local" : "non-local") + " url: " + url)
        if (local) {
            title = zimServer.getTitleFromUrl(url)
            // WORKAROUND for https://github.com/mkiol/kaktus/issues/14
            zimServer.getArticleAsync(url)
        } else {
            title = ""
            view.url = url
        }
    }

    function navigateBack() {
        if (history.length > 1) {
            navigate(history[history.length-2])
            history.pop()
        } else {
            pageStack.pop()
        }
    }

    function updateZoom(delta) {
        var zoom = settings.zoom
        if (zoom + delta <= 0.5)
            settings.zoom = 0.5
        else if (zoom + delta >= 2.0)
            settings.zoom = 2.0
        else
            settings.zoom = zoom + delta

        var theme = { "zoom": settings.zoom }
        postMessage("theme_set", { "theme": theme })
        postMessage("theme_update_scale")
        baner.show(Math.round(settings.zoom * 100).toString() + "%")
    }

    function switchNightMode() {
        postMessage(root.nightMode ? "nightmode_disable" : "nightmode_enable")
    }

    function messageReceivedHandler(message) {
        if (message.type === "inited") {
            // ok
        } else if (message.type === "nightmode_enabled") {
            root.nightMode = true
        } else if (message.type === "nightmode_disabled") {
            root.nightMode = false
        }
    }

    function postMessage(message, data) {
        view.experimental.postMessage(JSON.stringify({ "type": message, "data": data }));
    }

    Connections {
        target: zimServer
        onArticleReady: {
            view.loadHtml(article, root.url)
        }
    }

    showNavigationIndicator: false

    Component.onCompleted: init()

    // Workaround for 'High Power Consumption' webkit bug
    /*Connections {
        target: Qt.application
        onActiveChanged: {
            if(!Qt.application.active) {
                if (settings.powerSaveMode && root.status === PageStatus.Active) {
                    pageStack.pop()
                    return
                }
                if (root.status !== PageStatus.Active) {
                    pageStack.pop(pageStack.previousPage(root), PageStackAction.Immediate)
                    return
                }
            }
        }
    }*/

    SilicaWebView {
        id: view

        anchors { top: parent.top; left: parent.left; right: parent.right}
        height: parent.height

        experimental.preferences.javascriptEnabled: true
        experimental.preferences.navigatorQtObjectEnabled: true
        experimental.preferredMinimumContentsWidth: 980
        experimental.overview: false
        experimental.enableResizeContent: true

        experimental.userScripts: [
            Qt.resolvedUrl("js/Zimpedia.js"),
            Qt.resolvedUrl("js/Console.js"),
            Qt.resolvedUrl("js/MessageListener.js"),
            Qt.resolvedUrl("js/NightMode.js"),
            Qt.resolvedUrl("js/Theme.js"),
            Qt.resolvedUrl("js/init.js")]

        experimental.onMessageReceived: {
            //console.log("onMessageReceived data:", message.data)
            root.messageReceivedHandler(JSON.parse(message.data))
        }

        onLoadingChanged: {
            switch (loadRequest.status) {
            case WebView.LoadStartedStatus:
                proggressPanel.text = qsTr("Loading page content...");
                proggressPanel.open = true;
                break;
            case WebView.LoadSucceededStatus:
                proggressPanel.open = false;
                break;
            case WebView.LoadFailedStatus:
                proggressPanel.open = false;
                notification.show(qsTr("Failed to load page content"));
                break;
            default:
                proggressPanel.open = false;
            }
        }

        onNavigationRequested: {
            /*console.log("onNavigationRequested: ")
            console.log(" url:",request.url)
            console.log(" navigation type:", request.navigationType)
            console.log(" navigation LinkClickedNavigation:", request.navigationType === WebView.LinkClickedNavigation)
            console.log(" navigation FormSubmittedNavigation:", request.navigationType === WebView.FormSubmittedNavigation)
            console.log(" navigation BackForwardNavigation:", request.navigationType === WebView.BackForwardNavigation)
            console.log(" navigation ReloadNavigation:", request.navigationType === WebView.ReloadNavigation)
            console.log(" navigation FormResubmittedNavigation:", request.navigationType === WebView.FormResubmittedNavigation)
            console.log(" navigation OtherNavigation:", request.navigationType === WebView.OtherNavigation)
            console.log(" action:", request.action);*/

            if (!Qt.application.active) {
                request.action = WebView.IgnoreRequest
                return
            }

            if (request.navigationType === WebView.LinkClickedNavigation) {
                var url = request.url.toString()
                if (url !== root.history[root.history.length-1])
                    root.history.push(url)
                var local = root.isLocal(request.url.toString());
                //console.log(" local:", local);
                if (local) {
                    request.action = WebView.IgnoreRequest
                    root.navigate(request.url.toString())
                    return
                } else {
                    root.local = false
                    root.title = ""
                }
            }

            request.action = WebView.AcceptRequest
        }
    }

    TempBaner {
        id: baner
        anchors.centerIn: root
    }

    IconBar {
        id: controlbar
        flickable: view
        theme: "black"
        showable: !hideToolbarTimer.running

        IconBarItem {
            text: qsTr("Back")
            theme: parent.theme
            icon: "image://theme/icon-m-back"
            onClicked: root.navigateBack()
        }

        IconBarItem {
            text: qsTr("Toggle Night View")
            theme: parent.theme
            icon: root.nightMode ? "image://icons/icon-m-night-selected" :
                                   "image://icons/icon-m-night"
            enabled: root.nightModePossible
            visible: true
            onClicked: {
                root.switchNightMode()
            }
        }

        IconBarItem {
            text: qsTr("Add to bookmarks")
            theme: parent.theme
            icon: "image://theme/icon-m-favorite-selected"
            enabled: root.title !== ""
            visible: true
            onClicked: {
                bookmarkModel.addBookmark(root.title,
                                      root.history[root.history.length-1],
                                      zimServer.favicon)
            }
        }

        IconBarItem {
            text: qsTr("Browser")
            theme: parent.theme
            icon: "image://icons/icon-m-browser"
            onClicked: {
                notification.show(qsTr("Launching an external browser..."))
                var url = encodeURI(root.history[root.history.length-1])
                console.log("Opening: " + url)
                Qt.openUrlExternally(url)
            }
        }
        IconBarItem {
            text: qsTr("Decrease font")
            theme: parent.theme
            icon: "image://icons/icon-m-fontdown"
            onClicked: {
                root.updateZoom(-0.1)
            }
        }

        IconBarItem {
            text: qsTr("Increase font")
            theme: parent.theme
            icon: "image://icons/icon-m-fontup"
            onClicked: {
                root.updateZoom(0.1)
            }
        }

        IconBarItem {
            text: qsTr("Hide toolbar")
            theme: parent.theme
            icon: "image://theme/icon-m-dismiss"
            onClicked: {
                hideToolbarTimer.start()
                controlbar.hide()
            }
        }
    }

    ProgressPanel {
        id: proggressPanel
        transparent: false
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        cancelable: true
        onCloseClicked: view.stop()
    }

    Timer {
        id: hideToolbarTimer
        interval: root.toolbarHideTime
    }
}
