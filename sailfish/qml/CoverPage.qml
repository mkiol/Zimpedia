/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    CoverPlaceholder {
        text: zimServer.loaded ? zimServer.title + " (" + zimServer.language + ")" : APP_NAME
        icon.source: zimServer.loaded ? zimServer.favicon : "image://icons/icon-a-zimpedia"
        icon.width: Theme.iconSizeLarge
        icon.height: Theme.iconSizeLarge
    }
}
