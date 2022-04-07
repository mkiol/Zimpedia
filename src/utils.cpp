/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "utils.h"

#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QStandardPaths>
#include <QStringList>

Utils::Utils(QObject *parent) : QObject(parent) { createCacheDir(); }

QString Utils::homeDir() const {
    return QStandardPaths::standardLocations(QStandardPaths::HomeLocation)
        .first();
}

bool Utils::createCacheDir() {
    const QString cacheDir =
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    if (!QFile::exists(cacheDir)) {
        if (!QDir::root().mkpath(cacheDir)) {
            qWarning() << "unable to create cache dir";
            return false;
        }
    }

    return true;
}

void Utils::copyToClipboard(const QString &text) const {
    QGuiApplication::clipboard()->setText(text);
}
