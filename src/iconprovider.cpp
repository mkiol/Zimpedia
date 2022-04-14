/* Copyright (C) 2017-2020 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "iconprovider.h"

#include <sailfishapp.h>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QStandardPaths>
#include <mlite5/MGConfItem>

static inline QString pathTo(const QString &dir) {
    return SailfishApp::pathTo(dir).toString(QUrl::RemoveScheme);
}

IconProvider::IconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap) {
    auto ratio =
        MGConfItem(QStringLiteral("/desktop/sailfish/silica/theme_pixel_ratio"))
            .value()
            .toDouble();
    qDebug() << "device pixel ratio:" << ratio;
    if (ratio == 1.0) {
        themeDir = pathTo(QStringLiteral("images/z1.0"));
    } else if (ratio == 1.25) {
        themeDir = pathTo(QStringLiteral("images/z1.25"));
    } else if (ratio == 1.5) {
        themeDir = pathTo(QStringLiteral("images/z1.5"));
    } else if (ratio == 1.65 || ratio == 1.75 || ratio == 1.8) {
        themeDir = pathTo(QStringLiteral("images/z1.75"));
    } else if (ratio == 2.0) {
        themeDir = pathTo(QStringLiteral("images/z2.0"));
    } else {
        qWarning() << "Unknown pixel ratio so, defaulting to 1.0";
        themeDir = pathTo(QStringLiteral("images/z1.0"));
    }

    if (!QDir{themeDir}.exists()) {
        qWarning() << "theme dir does not exist:" << themeDir << ratio;
        themeDir = pathTo(QStringLiteral("images/z1.0"));
    }

    cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QPixmap IconProvider::makeZimPixmap(const QString &name, QSize *size,
                                    const QSize &requestedSize) const {
    auto filepath = QString{"%1/%2.png"}.arg(cacheDir, name);
    if (!QFile::exists(filepath)) {
        return QPixmap{};
    }

    QPixmap pixmap{filepath, "PNG"};
    if (pixmap.isNull()) pixmap.load(filepath, "WEBP");

    //    QPixmap sourcePixmap{filepath};
    //    QPixmap pixmap{sourcePixmap.size()};
    //    QPainter painter{&pixmap};
    //    painter.fillRect(pixmap.rect(), QColor{Qt::white});
    //    painter.drawPixmap(pixmap.rect(), sourcePixmap, sourcePixmap.rect());
    //    painter.end();

    if (size) *size = pixmap.size();

    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        return pixmap.scaled(requestedSize.width(), requestedSize.height());
    }

    return pixmap;
}

QPixmap IconProvider::makeThemePixmap(const QString &name, const QString &color,
                                      QSize *size,
                                      const QSize &requestedSize) const {
    auto filepath = QString{"%1/%2.png"}.arg(themeDir, name);
    if (!QFile::exists(filepath)) {
        filepath = themeDir + "/icon-m-item.png";
    }

    QPixmap pixmap{filepath};

    if (QColor::isValidColor(color)) {
        QPainter painter{&pixmap};
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
        painter.end();
    }

    if (size) *size = pixmap.size();

    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        return pixmap.scaled(requestedSize.width(), requestedSize.height());
    }

    return pixmap;
}

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size,
                                    const QSize &requestedSize) {
    if (id.startsWith("zim-")) return makeZimPixmap(id, size, requestedSize);

    auto parts = id.split('?');
    return makeThemePixmap(parts.at(0), parts.size() > 1 ? parts.at(1) : "",
                           size, requestedSize);
}
