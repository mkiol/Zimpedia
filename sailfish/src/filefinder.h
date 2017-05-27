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

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#endif

#include "zimmetadata.h"

class FileFinder : public QThread
{
    Q_OBJECT

public:
    explicit FileFinder(QObject *parent = 0);
    static bool scanZimFile(ZimMetaData &metaData);

protected:
    void run();

signals:
    void fileFound(const ZimMetaData &metaData);

private:
    void findFiles(const QString &dirName);
};

#endif // FILEFINDER_H
