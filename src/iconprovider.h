/* Copyright (C) 2017-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QPixmap>
#include <QQuickImageProvider>
#include <QSize>
#include <QString>

class IconProvider : public QQuickImageProvider {
   public:
    IconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size,
                          const QSize &requestedSize) override;

   private:
    QString themeDir;
    QString cacheDir;

    QPixmap makeZimPixmap(const QString &name, QSize *size,
                          const QSize &requestedSize) const;
    QPixmap makeThemePixmap(const QString &name, const QString &color,
                            QSize *size, const QSize &requestedSize) const;
};

#endif  // ICONPROVIDER_H
