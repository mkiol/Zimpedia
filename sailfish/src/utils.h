/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>

class Utils : public QObject
{
    Q_OBJECT

public:
    explicit Utils(QObject *parent = 0);

    Q_INVOKABLE const QString homeDir();
#ifdef BB10
    Q_INVOKABLE bool checkOSVersion(int major, int minor, int patch = 0, int build = 0);
    Q_INVOKABLE void launchBrowser(const QString &url);
    Q_INVOKABLE QString readAsset(const QString &path);
    Q_INVOKABLE void copyToClipboard(const QString &text);
#endif

};

#endif // UTILS_H
