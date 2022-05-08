/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.1
import Sailfish.Silica 1.0
import Sailfish.WebView 1.0

WebViewPage {
    id: root
    objectName: "webview"

    property url url

    property int _nightMode: 0
    property bool _nightModePossible: false
    property bool _zoomPossible: false
    property string _title: ""
    readonly property variant _settings: settings
    readonly property int _toolbarHideTime: 4000

    function updateTitle(url) {
        if (zimServer.isServerUrl(url)) {
            _title = zimServer.titleFromUrl(url)
        } else {
            _title = ""
        }
    }

    function navigateBack() {
        if (view.canGoBack) {
            root._nightModePossible = false
            root._zoomPossible = false
            view.goBack()
        } else {
            pageStack.pop()
        }
    }

    function init_js() {
        var script =
                utils.readAsset("scripts/night_mode.js") + "\n" +
                utils.readAsset("scripts/zoom.js") + "\n" +
                "var res = {night_mode: false,zoom: false}\n" +
                //"try {\n" +
                "res.night_mode = _night_mode_init()\n" +
                "res.zoom = _zoom_init()\n" +
                //"} catch {}\n" +
                "return res\n"
        view.runJavaScript(script, function(res) {
            console.log("js init done:", JSON.stringify(res))
            root._nightModePossible = res.night_mode
            root._zoomPossible = res.zoom
            if (root._nightModePossible) setNightMode(root._nightMode)
            if (root._zoomPossible) setZoom(settings.zoomFontSize())
            controlbar.show()
        }, errorCallback)
    }

    function errorCallback(error) {
        console.log("error:", error)
    }

    function updateZoom(delta) {
        if (!root._zoomPossible) return
        settings.zoom = settings.zoom + delta
        setZoom(settings.zoomFontSize())
        baner.show(Math.round(settings.zoom * 100).toString() + "%")
    }

    function setZoom(zoom) {
        if (!root._zoomPossible) return
        var script = "return window._zoom_set('" + zoom + "')\n";
        view.runJavaScript(script, function(res) {
            console.log("zoom set done:", zoom, res)
        }, errorCallback)
    }

    function setNightMode(type) {
        if (!root._nightModePossible) return
        var script = "return window._night_mode_set(" + type + ")\n"

        view.runJavaScript(script, function(res) {
            console.log("night switch done:", type, res)
            if (res) root._nightMode = type
        }, errorCallback)
    }

    function switchNightMode() {
        setNightMode(root._nightMode == 0 ? 1 : root._nightMode == 1 ? 2 : 0)
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (view.url.toString().length === 0) view.url = root.url
            controlbar.show()
        } else {
            controlbar.hide()
        }
    }

    showNavigationIndicator: false

    WebView {
        id: view

        anchors.fill: parent
        canShowSelectionMarkers: true
        onUrlChanged: {
            updateTitle(url)
        }

        onLoadedChanged: {
            if (loaded) {
                root.init_js()
            }
        }
    }

    TempBaner {
        id: baner
        anchors.centerIn: root
    }

    IconBar {
        id: controlbar
        theme: "black"
        flickable: view

        IconBarItem {
            text: qsTr("Back")
            theme: parent.theme
            icon: "image://theme/icon-m-back"
            onClicked: root.navigateBack()
        }

        IconBarItem {
            text: qsTr("Toggle Night View")
            theme: parent.theme
            icon: root._nightMode === 1 ? "image://icons/icon-m-night1" :
                  root._nightMode === 2 ? "image://icons/icon-m-night2" :
                                          "image://icons/icon-m-night0"
            enabled: root._nightModePossible
            visible: true
            onClicked: {
                root.switchNightMode()
            }
        }

        IconBarItem {
            text: qsTr("Add to bookmarks")
            theme: parent.theme
            icon: "image://theme/icon-m-favorite-selected"
            enabled: root._title !== ""
            visible: true
            onClicked: {
                bookmarkModel.addBookmark(root._title, view.url)
            }
        }

        IconBarItem {
            text: qsTr("Browser")
            theme: parent.theme
            icon: "image://icons/icon-m-browser"
            onClicked: Qt.openUrlExternally(view.url)
        }

        IconBarItem {
            text: qsTr("Decrease font")
            theme: parent.theme
            icon: "image://icons/icon-m-fontdown"
            enabled: root._zoomPossible
            visible: true
            onClicked: {
                root.updateZoom(-0.1)
            }
        }

        IconBarItem {
            text: qsTr("Increase font")
            theme: parent.theme
            icon: "image://icons/icon-m-fontup"
            enabled: root._zoomPossible
            visible: true
            onClicked: {
                root.updateZoom(0.1)
            }
        }

        IconBarItem {
            text: qsTr("Hide toolbar")
            theme: parent.theme
            icon: "image://theme/icon-m-dismiss"
            onClicked: {
                controlbar.hide()
            }
        }
    }
}
