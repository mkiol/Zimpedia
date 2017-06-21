/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

function bytesToSize(bytes) {
   var sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
   if (bytes === 0) return '0 B';
   var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
   return Math.round((bytes / Math.pow(1024, i))*100)/100 + ' ' + sizes[i];
}

function friendlyPath(path, home) {
    home = home + "/";
    if (path.lastIndexOf(home) === 0) {
        path = path.replace(home, "~/");
    }
    
    if (path.lastIndexOf("~/misc/android/external_sd") === 0) {
        return path.replace(/~\/misc\/android\/external_sd\//i, "SD Card:/");
    }
	
    return path;
}

function friendlyDir(path, home) {
    var dir = path.split("/"); dir.pop(); dir = dir.join("/");
    return friendlyPath(dir, home);
}
