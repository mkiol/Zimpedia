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
public:
    static FileFinder* instance();
    static bool scanZimFile(ZimMetaData &metaData);

    QMap<QString, ZimMetaData> files;
    bool busy;

    void init();

protected:
    void run();

signals:
    void fileFound(const ZimMetaData &metaData);
    void busyChanged();

private slots:
    void startedHandler();
    void finishedHandler();

private:
    static FileFinder* inst;

    explicit FileFinder(QObject *parent = 0);
    void findFiles(const QString &dirName);
};

#endif // FILEFINDER_H
