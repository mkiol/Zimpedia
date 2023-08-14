/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: root
    objectName: "about"
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: root.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("About %1").arg(APP_NAME)
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                height: root.isPortrait ? Theme.itemSizeHuge : Theme.iconSizeLarge
                width: root.isPortrait ? Theme.itemSizeHuge : Theme.iconSizeLarge
                source: settings.appIcon()
            }

            InfoLabel {
                text: APP_NAME
            }

            PaddedLabel {
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.highlightColor
                text: qsTr("Version %1").arg(APP_VERSION);
            }

            Flow {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge
                Button {
                    text: qsTr("Project website")
                    onClicked: Qt.openUrlExternally(PAGE)
                }
                Button {
                    text: qsTr("Changes")
                    onClicked: pageStack.push(Qt.resolvedUrl("ChangelogPage.qml"))
                }
            }

            SectionHeader {
                text: qsTr("Authors")
            }

            PaddedLabel {
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.RichText
                text: "Copyright &copy; %1 %2"
                .arg(COPYRIGHT_YEAR)
                .arg(AUTHOR)
            }

            PaddedLabel {
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.StyledText
                text: qsTr("%1 is developed as an open source project under %2.")
                .arg(APP_NAME)
                .arg("<a href=\"" + LICENSE_URL + "\">" + LICENSE + "</a>")
            }

            SectionHeader {
                text: qsTr("Translators")
            }

            PaddedLabel {
                horizontalAlignment: Text.AlignLeft
                text: "lutinotmalin · Jeff Huang · Вячеслав Диконов · RikudouSage · npopanz · Carmen F. B · " +
                      "Åke Engelbrektson · Nathan Follens · Szabó G. · 涛 匡 · Алексей Дедун · Heimen Stoffels · Albano Battistella"
            }

            SectionHeader {
                text: qsTr("Libraries")
            }

            PaddedLabel {
                horizontalAlignment: Text.AlignLeft
                text: "QHTTPServer · LibZIM · zstd · Xapian"
            }

            Spacer {}
        }

        VerticalScrollDecorator {}
    }
}
