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

QColor Utils::colorFromUuid(const QString &uuid) const {
    auto len = uuid.size() / 3;
    auto r = static_cast<int>(100 + (qHash(uuid.mid(0, len)) % 155));
    auto g = static_cast<int>(100 + (qHash(uuid.mid(len, len)) % 155));
    auto b = static_cast<int>(100 + (qHash(uuid.mid(2 * len, len)) % 155));
    return {r, g, b};
}

QString Utils::humanSize(qint64 bytes) const {
    QString number;

    if (bytes < 0x400) {
        number = QLocale::system().toString(bytes);
        number.append(" B");
        return number;
    }

    if (bytes < 0x100000) {
        qint64 result = (bytes + (0x400 / 2)) / 0x400;
        if (result < 0x400) {
            number = QLocale::system().toString(result);
            number.append(" KB");
            return number;
        }

        result = (bytes + (0x100000 / 2)) / 0x100000;
        number = QLocale::system().toString(result);
        number.append(" MB");
        return number;
    }

    if (bytes < 0x40000000) {
        qint64 result = (bytes + (0x100000 / 2)) / 0x100000;
        if (result < 0x100000) {
            number = QLocale::system().toString(result);
            number.append(" MB");
            return number;
        }

        result = (bytes + (0x40000000 / 2)) / 0x40000000;
        number = QLocale::system().toString(result);
        number.append(" GB");
        return number;
    }

    if (bytes < 0x10000000000) {
        qint64 result = (bytes + (0x40000000 / 2)) / 0x40000000;
        if (result < 0x40000000) {
            number = QLocale::system().toString(result);
            number.append(" GB");
            return number;
        }

        result = (bytes + (0x10000000000 / 2)) / 0x10000000000;
        number = QLocale::system().toString(result);
        number.append(" TB");
        return number;
    }

    qint64 result = (bytes + (0x10000000000 / 2)) / 0x10000000000;
    number = QLocale::system().toString(result);
    number.append(" TB");
    return number;
}
