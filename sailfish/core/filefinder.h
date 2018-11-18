/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef FILEFINDER_H
#define FILEFINDER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QMap>

#include "zimmetadata.h"

class FileFinder : public QThread
{
    Q_OBJECT
    Q_PROPERTY (bool busy READ getBusy NOTIFY busyChanged)

public:
    static FileFinder* instance(QObject *parent = nullptr);
    static bool scanZimFile(ZimMetaData &metaData);

    QMap<QString, ZimMetaData> files;

    bool getBusy();
    void init();

signals:
    void busyChanged();

private:
    static const int max_size = 50; // maximum size of title
    static FileFinder* inst;

    explicit FileFinder(QObject *parent = nullptr);
    void run();
    void findFiles(const QString &dirName);
};

#endif // FILEFINDER_H
