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
#include <algorithm>

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
    if (fontSize() != value) {
        setValue("fontsize", value);
        emit fontSizeChanged();
    }
}

int Settings::fontSize() const { return value("fontsize", 1).toInt(); }

float Settings::zoom() const {
    return std::min(std::max(minZoom, value("zoom", 1.0).toFloat()), maxZoom);
}

void Settings::setZoom(float value) {
    value = std::min(std::max(minZoom, value), maxZoom);
    if (zoom() != value) {
        setValue("zoom", value);
        emit zoomChanged();
    }
}

void Settings::setZimFiles(QStringList files) {
    std::sort(files.begin(), files.end());
    if (zimFiles() != files) {
        setValue("zimfiles", files);
        emit zimFilesChanged();
    }
}

void Settings::addZimFile(const QString& value) {
    auto files = zimFiles();
    if (files.contains(value)) return;
    files.push_back(value);
    setZimFiles(files);
}

void Settings::removeZimFile(const QString& value) {
    auto files = zimFiles();
    if (!files.contains(value)) return;
    files.removeAll(value);
    setZimFiles(files);
}

QStringList Settings::zimFiles() const {
    return value("zimfiles", {}).toStringList();
}

void Settings::setBrowser(int value) {
    if (browser() != value) {
        setValue("browser", value);
        emit browserChanged();
    }
}

int Settings::browser() const { return value("browser", 0).toInt(); }

void Settings::setSearchMode(Settings::SearchMode value) {
    if (searchMode() != value) {
        setValue("searchmode", static_cast<int>(value));
        emit searchModeChanged();
    }
}

Settings::SearchMode Settings::searchMode() const {
    return static_cast<Settings::SearchMode>(value("searchmode", 0).toInt());
}

QUrl Settings::appIcon() const {
    return QUrl::fromLocalFile(
        QString(QStringLiteral("/usr/share/icons/hicolor/172x172/apps/%1.png"))
            .arg(Zimpedia::APP_BINARY_ID));
}
