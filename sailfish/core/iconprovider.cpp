/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifdef SAILFISH
#include <sailfishapp.h>
#include <mlite5/MGConfItem>
#endif
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QDir>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

#include "iconprovider.h"

IconProvider::IconProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
#ifdef SAILFISH
    // Getting pixel ratio
    double ratio = MGConfItem("/desktop/sailfish/silica/theme_pixel_ratio").value().toDouble();
    //qDebug() << "ratio:" << ratio;
    if (ratio == 0) {
        qWarning() << "Pixel ratio is 0, defaulting to 1.0.";
        themeDir = SailfishApp::pathTo("images/z1.0").toString(QUrl::RemoveScheme);
    } else if (ratio == 1.0) {
        themeDir = SailfishApp::pathTo("images/z1.0").toString(QUrl::RemoveScheme);
    } else if (ratio == 1.25) {
        themeDir = SailfishApp::pathTo("images/z1.25").toString(QUrl::RemoveScheme);
    } else if (ratio == 1.5) {
        themeDir = SailfishApp::pathTo("images/z1.5").toString(QUrl::RemoveScheme);
    } else if (ratio == 1.75 || ratio == 1.8) {
        themeDir = SailfishApp::pathTo("images/z1.75").toString(QUrl::RemoveScheme);
    } else if (ratio == 2.0) {
        themeDir = SailfishApp::pathTo("images/z2.0").toString(QUrl::RemoveScheme);
    }

    if (!QDir(themeDir).exists()) {
        qWarning() << "Theme " + themeDir + " for ratio " + ratio + " doesn't exist!";
        themeDir = SailfishApp::pathTo("images/z1.0").toString(QUrl::RemoveScheme);
    }
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#else
    cacheDir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#endif
}

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    //qDebug() << id;

    QStringList parts = id.split('?');
    QString filepath;

    if (id.startsWith("zim-")) {
        filepath = cacheDir + "/" + parts.at(0) + ".png";
        if (!QFile::exists(filepath)) {
            return QPixmap();
        }

        QPixmap sourcePixmap(filepath);
        QPixmap newPixmap(sourcePixmap.size());
        QPainter painter(&newPixmap);
        painter.fillRect(newPixmap.rect(), QColor(Qt::white));
        painter.drawPixmap(newPixmap.rect(), sourcePixmap, sourcePixmap.rect());
        painter.end();

        if (size)
            *size  = newPixmap.size();
        if (requestedSize.width() > 0 && requestedSize.height() > 0)
            return newPixmap.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
        else
            return newPixmap;

    } else {
        filepath = themeDir + "/" + parts.at(0) + ".png";
        if (!QFile::exists(filepath)) {
            // Icon file is not exist -> fallback to default icon
            filepath = themeDir + "/icon-m-item.png";
        }

        QPixmap sourcePixmap(filepath);

        if (size)
            *size  = sourcePixmap.size();

        if (parts.length() > 1)
            if (QColor::isValidColor(parts.at(1))) {
                QPainter painter(&sourcePixmap);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.fillRect(sourcePixmap.rect(), parts.at(1));
                painter.end();
            }

        if (requestedSize.width() > 0 && requestedSize.height() > 0)
            return sourcePixmap.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
        else
            return sourcePixmap;
    }
}
