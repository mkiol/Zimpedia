/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

import QtQuick 2.0

Item {
    id: root

    property bool showPlaceholder: false
    property alias text: placeholder.text
    property alias source: icon.source
    property alias status: icon.status
    property bool ok: icon.status === Image.Ready

    Image {
        // feed icon
        id: icon
        anchors.fill: parent
    }

    IconPlaceholder {
        // placeholder
        id: placeholder
        visible: root.showPlaceholder && !root.ok
        anchors.fill: parent
    }
}
