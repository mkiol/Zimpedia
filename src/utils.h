/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>

class Utils : public QObject {
    Q_OBJECT

   public:
    explicit Utils(QObject *parent = nullptr);

    Q_INVOKABLE QString homeDir() const;
    Q_INVOKABLE void copyToClipboard(const QString &text) const;
    static bool createCacheDir();
};

#endif  // UTILS_H
