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
    property bool _nightModePossible: true
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
            view.goBack()
        } else {
            pageStack.pop()
        }
    }

    function errorCallback(error) {
        console.log("error:", error)
    }

    function updateZoom(delta) {
        settings.zoom = settings.zoom + delta
        setZoom(settings.zoomFontSize())
        baner.show(Math.round(settings.zoom * 100).toString() + "%")
    }

    function initZoom() {
        setZoom(settings.zoomFontSize())
    }

    function setZoom(zoom) {
        var script =
                "var css = 'html *{ font-size: " + zoom + "; }'\n" +
                "var style = document.getElementById('_zoom_style')\n" +
                "if (style) { style.innerHTML = css; return }\n" +
                "style = document.createElement('style')\n" +
                "style.id = '_zoom_style'\n" +
                "style.type = 'text/css'\n" +
                "style.appendChild(document.createTextNode(css))\n" +
                "document.getElementsByTagName('head').item(0).appendChild(style)\n"
        view.runJavaScript(script, function(res) {
            console.log("zoom set done:", zoom)
        }, errorCallback)
    }

    function initNightMode() {
        var script =
                "var css1 = 'html._nightmode1_class { filter: contrast(68%) brightness(108%) invert(100%); }\\n' +\n" +
                "'html._nightmode1_class iframe { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode1_class object { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode1_class embed { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode1_class video { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode1_class img { filter: invert(100%); }'\n" +
                "var style1 = document.getElementById('_nightmode1_style')\n" +
                "if (style1) { style1.innerHTML = css1; return}\n" +
                "style1 = document.createElement('style')\n" +
                "style1.id = '_nightmode1_style'\n" +
                "style1.type = 'text/css'\n" +
                "style1.appendChild(document.createTextNode(css1))\n" +
                "document.getElementsByTagName('head').item(0).appendChild(style1)\n" +
                "var css2 = 'html._nightmode2_class { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode2_class iframe { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode2_class object { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode2_class embed { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode2_class video { filter: invert(100%); }\\n' +\n" +
                "'html._nightmode2_class img { filter: invert(100%); }'\n" +
                "var style2 = document.getElementById('_nightmode2_style')\n" +
                "if (style2) { style2.innerHTML = css2; return}\n" +
                "style2 = document.createElement('style')\n" +
                "style2.id = '_nightmode2_style'\n" +
                "style2.type = 'text/css'\n" +
                "style2.appendChild(document.createTextNode(css2))\n" +
                "document.getElementsByTagName('head').item(0).appendChild(style2)\n"
        view.runJavaScript(script, function(res) {
            console.log("night mode init done")
            root._nightModePossible = true
            setNightMode(root._nightMode)
        }, errorCallback)
    }

    function setNightMode(type) {
        var script
        if (type === 0) {
            script =
                "var html = document.getElementsByTagName('html')[0]\n" +
                "html.className = html.className.replace(' _nightmode1_class', '')\n" +
                "html.className = html.className.replace(' _nightmode2_class', '')\n"
        } else if (type === 1)  {
            script =
                "var html = document.getElementsByTagName('html')[0]\n" +
                "html.className += ' _nightmode1_class'\n"
        } else {
            script =
                "var html = document.getElementsByTagName('html')[0]\n" +
                "html.className += ' _nightmode2_class'\n"
        }

        view.runJavaScript(script, function(res) {
            console.log("night switch done:", type)
            root._nightMode = type
        }, errorCallback)
    }

    function switchNightMode() {
        setNightMode(root._nightMode == 0 ? 1 : root._nightMode == 1 ? 2 : 0)
    }

    Component.onCompleted: view.url = root.url
    showNavigationIndicator: false

    WebView {
        id: view

        canShowSelectionMarkers: true
        onUrlChanged: {
            updateTitle(url)
        }
        anchors { top: parent.top; left: parent.left; right: parent.right}
        height: parent.height

        onLoadedChanged: {
            if (loaded) {
                root.initZoom()
                root.initNightMode()
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

    Timer {
        id: hideToolbarTimer
        interval: root._toolbarHideTime
    }
}
