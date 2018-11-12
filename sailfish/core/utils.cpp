/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDebug>

#ifdef BB10
#include <bps/navigator.h>
#include <bb/platform/PlatformInfo>
#include <bb/system/Clipboard>
#elif SAILFISH
#include <QGuiApplication>
#include <QClipboard>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

#include "utils.h"
#include "bookmarks.h"

Utils::Utils(QObject *parent) : QObject(parent)
{
    createCacheDir();
}

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

#ifdef SAILFISH
bool Utils::createCacheDir()
{
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    if (!QFile::exists(cacheDir)) {
        if (!QDir::root().mkpath(cacheDir)) {
            qWarning() << "Unable to create cache dir!";
            return false;
        }
    }

    return true;
}
#endif

void Utils::copyToClipboard(const QString &text)
{
#ifdef BB10
    bb::system::Clipboard clipboard;
    clipboard.clear();
    clipboard.insert("text/plain", text.toUtf8());
#elif SAILFISH
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
#endif
}

#ifdef BB10
void Utils::launchBrowser(const QString &url)
{
    qDebug() << "launchBrowser" << url;
    navigator_invoke(url.toStdString().c_str(),0);
}

QString Utils::readAsset(const QString &path)
{
    QFile file("app/native/assets/" + path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open" << path << "for reading: " << file.errorString();
        file.close();
        return "";
    }

    QString data = QString(file.readAll());
    file.close();

    return data;
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

