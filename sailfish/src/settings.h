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
    Q_PROPERTY (QString zimFile READ getZimFile WRITE setZimFile NOTIFY zimFileChanged)

public:
    static Settings* instance();

    void setPort(int value);
    int getPort();

    void setZimFile(const QString & value);
    QString getZimFile();

    ArticleModel* articleModel;

signals:
    void portChanged();
    void zimFileChanged();

private:
    QSettings settings;
    static Settings *inst;

    explicit Settings(QObject *parent = 0);
};

#endif // SETTINGS_H
