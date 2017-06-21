/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Container {
    id: root
    
    property bool pressed
    property bool isMove
    property bool isDown
    signal clicked
    
    background: pressed ? ui.palette.plain : Color.Transparent
    
    onTouch: {
        pressed = event.isDown()||event.isMove() ? true : false;
        if (event.isUp()) {
            clicked();
        }
    }
}
