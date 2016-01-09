/*
 Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>
 
 This file is part of Zimpedia application.
 
 This Source Code Form is subject to the terms of
 the Mozilla Public License, v.2.0. If a copy of
 the MPL was not distributed with this file, You can
 obtain one at http://mozilla.org/MPL/2.0/.
 */

import bb.cascades 1.3
import bb.system 1.3

Container {
    id: root

    function show(text) {
        toast.body = text;
        toast.show();
    }

    attachedObjects: [
        SystemToast {
            id: toast
            body: ""
        }
    ]
}
