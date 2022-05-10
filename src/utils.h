/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QColor>
#include <QObject>
#include <QString>

class Utils : public QObject {
    Q_OBJECT

   public:
    explicit Utils(QObject *parent = nullptr);
    static bool createCacheDir();
    static QByteArray readAssetStatic(const QString &path);
    Q_INVOKABLE QString readAsset(const QString &path) const;
    Q_INVOKABLE QString homeDir() const;
    Q_INVOKABLE void copyToClipboard(const QString &text) const;
    Q_INVOKABLE QColor colorFromUuid(const QString &uuid) const;
    Q_INVOKABLE QString humanSize(qint64 bytes) const;
};

#endif  // UTILS_H
