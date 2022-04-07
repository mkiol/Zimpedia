/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QVariant>

#include "info.h"

Settings* Settings::m_instance = nullptr;

Settings::Settings() : QSettings{settingsFilepath(), QSettings::NativeFormat} {
    qDebug() << "app:" << Zimpedia::ORG << Zimpedia::APP_ID;
    qDebug() << "config location:"
             << QStandardPaths::writableLocation(
                    QStandardPaths::ConfigLocation);
    qDebug() << "data location:"
             << QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation);
    qDebug() << "cache location:"
             << QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    qDebug() << "settings file:" << fileName();
}

QString Settings::settingsFilepath() {
    QDir confDir{
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)};
    confDir.mkpath(QCoreApplication::organizationName() + QDir::separator() +
                   QCoreApplication::applicationName());
    return confDir.absolutePath() + QDir::separator() +
           QCoreApplication::organizationName() + QDir::separator() +
           QCoreApplication::applicationName() + QDir::separator() +
           settingsFilename;
}

Settings* Settings::instance() {
    if (!Settings::m_instance) Settings::m_instance = new Settings{};
    return Settings::m_instance;
}

void Settings::setFontSize(int value) {
    if (getFontSize() != value) {
        setValue("fontsize", value);
        emit fontSizeChanged();
    }
}

int Settings::getFontSize() const { return value("fontsize", 1).toInt(); }

float Settings::getZoom() const {
    float size = value("zoom", 1.0).toFloat();
    return size <= 0.5 ? 0.5 : size >= 2.0 ? 2.0 : size;
}

void Settings::setZoom(float value) {
    // Min value is 0.5 & max value is 2.0
    if (value <= 0.5)
        value = 0.5;
    else if (value >= 2.0)
        value = 2.0;

    if (getZoom() != value) {
        setValue("zoom", value);
        emit zoomChanged();
    }
}

void Settings::setZimFile(const QString& value) {
    if (getZimFile() != value) {
        setValue("zimfile", value);
        emit zimFileChanged();
    }
}

QString Settings::getZimFile() const { return value("zimfile", "").toString(); }

void Settings::setBrowser(int value) {
    if (getBrowser() != value) {
        setValue("browser", value);
        emit browserChanged();
    }
}

int Settings::getBrowser() const { return value("browser", 0).toInt(); }

void Settings::setSearchMode(Settings::SearchMode value) {
    if (getSearchMode() != value) {
        setValue("searchmode", static_cast<int>(value));
        emit searchModeChanged();
    }
}

Settings::SearchMode Settings::getSearchMode() const {
    return static_cast<Settings::SearchMode>(value("searchmode", 0).toInt());
}
