/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QStringList>

#ifdef BB10
#include <bps/navigator.h>
#include <bb/platform/PlatformInfo>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

#include "utils.h"
#include "bookmarks.h"

Utils::Utils(QObject *parent) : QObject(parent)
{}

const QString Utils::homeDir()
{
#ifdef BB10
    // shared folder
    const QString homeLocation = QDir::currentPath() + "/shared";
#elif SAILFISH
    const QStringList homeLocationList = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    const QString homeLocation = homeLocationList.first();
#endif
    return homeLocation;
}

#ifdef BB10
void Utils::launchBrowser(const QString &url)
{
    navigator_invoke(url.toStdString().c_str(),0);
}

// Source: http://hecgeek.blogspot.com/2014/10/blackberry-10-multiple-os-versions-from.html
bool Utils::checkOSVersion(int major, int minor, int patch, int build)
{
    bb::platform::PlatformInfo platformInfo;
    const QString osVersion = platformInfo.osVersion();
    const QStringList parts = osVersion.split('.');
    const int partCount = parts.size();

    if(partCount < 4) {
        // Invalid OS version format, assume check failed
        return false;
    }

    // Compare the base OS version using the same method as the macros
    // in bbndk.h, which are duplicated here for maximum compatibility.
    int platformEncoded = (((parts[0].toInt())*1000000)+((parts[1].toInt())*1000)+(parts[2].toInt()));
    int checkEncoded = (((major)*1000000)+((minor)*1000)+(patch));

    if(platformEncoded < checkEncoded) {
        return false;
    }
    else if(platformEncoded > checkEncoded) {
        return true;
    }
    else {
        // The platform and check OS versions are equal, so compare the build version
        int platformBuild = parts[3].toInt();
        return platformBuild >= build;
    }
}
#endif

