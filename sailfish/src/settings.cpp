/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QVariant>

#include "settings.h"

Settings* Settings::inst = 0;

Settings::Settings(QObject *parent) : QObject(parent), settings()
{
}

Settings* Settings::instance()
{
    if (Settings::inst == 0) {
        Settings::inst = new Settings();
    }

    return Settings::inst;
}

void Settings::setPort(int value)
{
    if (getPort() != value) {
        settings.setValue("port", value);
        emit portChanged();
    }
}

int Settings::getPort()
{
    return settings.value("port", 9091).toInt();
}

void Settings::setFontSize(int value)
{
    if (getFontSize() != value) {
        settings.setValue("fontsize", value);
        emit fontSizeChanged();
    }
}

int Settings::getFontSize()
{
    return settings.value("fontsize", 1).toInt();
}

float Settings::getZoom()
{
    float size = settings.value("zoom", 1.0).toFloat();
    return size <= 0.5 ? 0.5 : size >= 2.0 ? 2.0 : size;
}

void Settings::setZoom(float value)
{
    // Min value is 0.5 & max value is 2.0
    if (value <= 0.5)
        value = 0.5;
    else if (value >= 2.0)
        value = 2.0;

    if (getZoom() != value) {
        settings.setValue("zoom", value);
        emit zoomChanged();
    }
}

void Settings::setZimFile(const QString & value)
{
    if (getZimFile() != value) {
        settings.setValue("zimfile", value);
        emit zimFileChanged();
    }
}

QString Settings::getZimFile()
{
    return settings.value("zimfile", "").toString();
}

void Settings::setBrowser(int value)
{
    if (getBrowser() != value) {
        settings.setValue("browser", value);
        emit browserChanged();
    }
}

int Settings::getBrowser()
{
    return settings.value("browser", 0).toInt();
}
