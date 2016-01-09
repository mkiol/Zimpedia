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
    
    property bool advancedVisible: false

    /*titleBar: TitleBar {
        title: qsTr("Settings")
    }*/
    
    titleBar: TitleBar {
        acceptAction: ActionItem {
            title: qsTr("Save")
            onTriggered: {
                accept();
            }
        }
        
        dismissAction: ActionItem {
            title: qsTr("Cancel")
            onTriggered: {
                nav.pop();
            }
        }
        
        title: qsTr("Settings")
        appearance: TitleBarAppearance.Plain
    }

    function validatePort(port) {
        return parseInt(port).toString() == port && parseInt(port) > 1023 && parseInt(port) <= 65535;
    }

    function accept() {

        if (! validatePort(portField.text)) {
            notification.show(qsTr("Port number is invalid! Allowed port range is 1024-65535!"));
            return;
        }

        settings.port = portField.text;
        settings.fontSize = fontSizeBox.selectedOption.value;

        nav.pop();
    }

    /*actions: ActionItem {
        title: qsTr("Save")
        imageSource: "asset:///ic_save.png"
        ActionBar.placement: ActionBarPlacement.OnBar

        onTriggered: {
            accept();
        }
    }*/

    Container {

        ScrollView {
            Container {
                Header {
                    title: qsTr("UI")
                }
                
                Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(2)
                    bottomPadding: ui.du(2)
                    
                    DropDown {
                        id: fontSizeBox
                        title: qsTr("Font size")
                        options: [
                            Option {
                                selected: settings.fontSize == 1
                                value: 1
                                text: qsTr("Normal")
                            },
                            Option {
                                selected: settings.fontSize == 2
                                value: 2
                                text: qsTr("Large")
                            },
                            Option {
                                selected: settings.fontSize == 3
                                value: 3
                                text: qsTr("Extra large")
                            }
                        ]
                    }
                }
                
                Container {
                    topPadding: ui.du(1)
                }
                
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: advancedVisible ? qsTr("Hide advanced options") : qsTr("Show advanced options")
                    onClicked: advancedVisible = !advancedVisible;
                }
                
                Container {
                    topPadding: ui.du(1)
                }
                
                Header {
                    visible: root.advancedVisible
                    title: qsTr("Advanced")
                }

                Container {
                    visible: root.advancedVisible
                    
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(2)
                    bottomPadding: ui.du(2)

                    Label {
                        text: qsTr("Define a listening port number. Changes will take effect after app restart.")
                        multiline: true
                    }

                    Container {
                        topPadding: ui.du(1)
                    }

                    TextField {
                        id: portField
                        hintText: qsTr("Enter port number here!")
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        validator: Validator {
                            mode: ValidationMode.FocusLost
                            errorMessage: qsTr("Port number is invalid! Allowed port range is 1024-65535!")
                            onValidate: {
                                if (validatePort(portField.text))
                                    state = ValidationState.Valid;
                                else
                                    state = ValidationState.Invalid;
                            }
                        }

                        onCreationCompleted: {
                            text = settings.port;
                        }
                    }
                }
            }
        }
    }
}
