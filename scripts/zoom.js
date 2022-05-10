/* Copyright (C) 2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

function _zoom_init() {
    window._zoom_possible = true
    return true
}

function _zoom_set(zoom) {
    if (!window._zoom_possible) return false

    var style = document.documentElement.getElementsByClassName('_zoom_class').item(0)
    var css = 'html *:not(h1) { font-size: ' + zoom + ' !important; }'
    if (style) {
        style.innerHTML = css
    } else {
        style = document.createElement('style')
        style.className = '_zoom_class'
        style.type = 'text/css'
        style.appendChild(document.createTextNode(css))
        var head = document.head
        if (head) head.appendChild(style)
    }
    return true
}

window._zoom_possible = false
window._zoom_set = _zoom_set
