/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "utils.h"

#include <sailfishapp.h>

#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QStandardPaths>
#include <QStringList>

Utils::Utils(QObject *parent) : QObject{parent} { createCacheDir(); }

QByteArray Utils::readAssetStatic(const QString &path) {
    QFile file{SailfishApp::pathTo(path).toLocalFile()};

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "could not open file:" << path << file.errorString();
        return {};
    }

    return file.readAll();
}

QString Utils::readAsset(const QString &path) const {
    return QString::fromUtf8(readAssetStatic(path));
}

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
    int len = uuid.size() / 3;
    auto h = qHash(uuid);
    auto r = static_cast<int>(100u + ((h + qHash(uuid.mid(0, len))) % 155u));
    auto g = static_cast<int>(100u + ((h + qHash(uuid.mid(len, len))) % 155u));
    auto b =
        static_cast<int>(100u + ((h + qHash(uuid.mid(2 * len, len))) % 155u));
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
