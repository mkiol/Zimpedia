/*
 * Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 * 
 * This file is part of Zimpedia application.
 * 
 * This Source Code Form is subject to the terms of
 * the Mozilla Public License, v.2.0. If a copy of
 * the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3

Page {
    id: root
    objectName: "settings"

    property bool dark: Application.themeSupport.theme.colorTheme.style === VisualStyle.Dark

    titleBar: TitleBar {
        title: qsTr("Settings")
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

    Container {
        ScrollView {
            Container {
                Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(2)
                    bottomPadding: ui.du(2)

                    DropWithDescription {
                        title: qsTr("Default viewer")
                        description: qsTr("Wiki pages can be opened in the built-in web viewer or in an external browser.")
                        options: [
                            Option {
                                selected: settings.browser === value
                                value: 0
                                imageSource: dark ? "asset:///webview.png" : "asset:///webviewd.png"
                                text: qsTr("Built-in viewer")
                            },
                            Option {
                                selected: settings.browser === value
                                value: 1
                                imageSource: dark ? "asset:///browser.png" : "asset:///browserd.png"
                                text: qsTr("Browser")
                            }
                        ]
                        onSelectedOptionChanged: {
                            settings.browser = selectedOption.value;
                        }
                    }

                    Container {
                        topPadding: ui.du(2)
                        Container {
                            layout: DockLayout {
                            }
                            preferredWidth: display.pixelSize.width
                            Label {
                                horizontalAlignment: HorizontalAlignment.Left
                                text: qsTr("Viewer font size level")
                            }
                            Label {
                                horizontalAlignment: HorizontalAlignment.Right
                                text: Math.floor(fontSizeSlider.value) + "%"
                                onTouch: {
                                    fontSizeSlider.value = 100.0;
                                }
                            }
                        }

                        Container {
                            topPadding: ui.du(1)
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }

                            ImageView {
                                imageSource: dark ? "asset:///fontdown.png" : "asset:///fontdownd.png"
                                minWidth: 96
                            }

                            Slider {
                                id: fontSizeSlider
                                property bool created: false
                                fromValue: 50.0
                                toValue: 200.0
                                onValueChanged: {
                                    if (created)
                                        settings.zoom = Math.floor(value) / 100;
                                }
                                onCreationCompleted: {
                                    created = true
                                    value = Math.floor(settings.zoom * 100)
                                }
                            }

                            ImageView {
                                imageSource: dark ? "asset:///fontup.png" : "asset:///fontupd.png"
                                minWidth: 96
                            }
                        }
                    }
                }
            }
        }
    }
}
