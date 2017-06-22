/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4
import org.labsquare 1.0

Container {
    id: root
    
    property bool showPlaceholder: false
    property alias text: placeholder.text
    property alias source: icon.url
    property bool ok: icon.isLoaded
    property real size
    
    preferredWidth: root.size
    preferredHeight: root.size
    minWidth: root.size
    minHeight: root.size
    
    layout: DockLayout {}
    
    WebImageView {
        id: icon
        preferredWidth: root.size
        preferredHeight: root.size
        minWidth: root.size
        minHeight: root.size
    }
    
    IconPlaceholder {
        // placeholder
        id: placeholder
        visible: root.showPlaceholder && !root.ok
        preferredWidth: root.size
        preferredHeight: root.size
        minWidth: root.size
        minHeight: root.size
    }
}