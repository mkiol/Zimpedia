/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

#include "articlemodel.h"

class Settings: public QObject
{
    Q_OBJECT

    Q_PROPERTY (int port READ getPort WRITE setPort NOTIFY portChanged)
    Q_PROPERTY (int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY (float zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY (QString zimFile READ getZimFile WRITE setZimFile NOTIFY zimFileChanged)
    Q_PROPERTY (int browser READ getBrowser WRITE setBrowser NOTIFY browserChanged)

public:
    static Settings* instance();

    void setPort(int value);
    int getPort();
    void setFontSize(int value);
    int getFontSize();
    void setZimFile(const QString & value);
    void setZoom(float value);
    float getZoom();
    void setBrowser(int value);
    int getBrowser();
    QString getZimFile();

signals:
    void portChanged();
    void fontSizeChanged();
    void zimFileChanged();
    void browserChanged();
    void zoomChanged();

private:
    QSettings settings;
    static Settings *inst;

    explicit Settings(QObject *parent = 0);
};

#endif // SETTINGS_H
