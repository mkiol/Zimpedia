/* Copyright (C) 2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

function _night_mode_init() {
    var head = document.head
    var css1 = `
html._nightmode1_class {
background-color: white !important;
filter: contrast(68%) brightness(108%) invert(100%) !important; }
html._nightmode1_class iframe { filter: invert(100%) !important; }
html._nightmode1_class object { filter: invert(100%) !important; }
html._nightmode1_class embed { filter: invert(100%) !important; }
html._nightmode1_class video { filter: invert(100%) !important; }
html._nightmode1_class img { filter: invert(100%) !important; }`
    var style1 = document.getElementById('_nightmode1_style')
    if (style1) {
        style1.innerHTML = css1
        window._night_mode_possible = true
        return true
    }
    style1 = document.createElement('style')
    style1.id = '_nightmode1_style'
    style1.type = 'text/css'
    style1.appendChild(document.createTextNode(css1))
    if (head) head.appendChild(style1)

    var css2 = `
html._nightmode2_class {
background-color: white !important;
filter: invert(100%) !important; }
html._nightmode2_class iframe { filter: invert(100%) !important; }
html._nightmode2_class object { filter: invert(100%) !important; }
html._nightmode2_class embed { filter: invert(100%) !important; }
html._nightmode2_class video { filter: invert(100%) !important; }
html._nightmode2_class img { filter: invert(100%) !important; }`
    var style2 = document.getElementById('_nightmode2_style')
    if (style2) {
        style2.innerHTML = css2
        window._night_mode_possible = true
        return true
    }
    style2 = document.createElement('style')
    style2.id = '_nightmode2_style'
    style2.type = 'text/css'
    style2.appendChild(document.createTextNode(css2))
    if (head) head.appendChild(style2)

    window._night_mode_possible = true
    return true
}

function _night_mode_set(type) {
    if (!window._night_mode_possible) return false
    var html = document.documentElement
    if (type === 0) {
        if (html) {
            html.className = html.className.replace(/_nightmode1_class/g, '')
            html.className = html.className.replace(/_nightmode2_class/g, '')
        }
        return true
    }
    if (type === 1) {
        if (html) {
            html.className += ' _nightmode1_class'
            html.className = html.className.replace(/_nightmode2_class/g, '')
        }
        return true
    }
    if (type === 2) {
        if (html) {
            html.className += ' _nightmode2_class'
            html.className = html.className.replace(/_nightmode1_class/g, '')
        }
        return true
    }
    return false
}

window._night_mode_possible = false
window._night_mode_set = _night_mode_set
