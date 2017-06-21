/*
 Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.4

Container {
    id: root
    
    property string text

    //Source: http://werxltd.com/wp/2010/05/13/javascript-implementation-of-javas-string-hashcode-method/
    function hash(text, index) {
        var hash = 0, i, chr, len;
        if (text.length === 0 || text.length < index) return hash;
        for (i = index, len = text.length; i < len; i++) {
            chr   = text.charCodeAt(i);
            hash  = ((hash << 5) - hash) + chr;
        }
        return Math.abs(hash)%255;
    }
    
    layout: DockLayout {}
    
    function init() {
        var r = text.length>0 ? hash(text,0)/255 : 1;
        var g = text.length>1 ? hash(text,1)/255 : 1;
        var b = text.length>2 ? hash(text,2)/255 : 1;
        var colorBg = Color.create(r,g,b);
        var colorFg = (r+g+b)>1.5 ? Color.Black : Color.White;
        background = colorBg;
        label.textStyle.color = colorFg;
    }
    
    onTextChanged: init()
    
    Label {
        id: label
        text: root.text.substring(0,1).toUpperCase()
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        textStyle.base: SystemDefaults.TextStyles.BodyText
    }
}