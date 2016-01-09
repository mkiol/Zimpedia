/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef ZIMSERVER_H
#define ZIMSERVER_H

#include <QObject>

#include <zim/file.h>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

class ZimServer : public QObject
{
    Q_OBJECT

    Q_PROPERTY (bool loaded READ getLoaded NOTIFY loadedChanged)
    Q_PROPERTY (bool listening READ getListening NOTIFY listeningChanged)

public:
    explicit ZimServer(QObject *parent = 0);
    Q_INVOKABLE bool loadZimFile();
    Q_INVOKABLE void findTitle(const QString & title);
    Q_INVOKABLE void setScreenSize(int width);

    bool getLoaded();
    bool getListening();

signals:
    void error();
    void loadedChanged();
    void listeningChanged();
    void searchReady();

public slots:
    void requestHandler(QHttpRequest *req, QHttpResponse *resp);

private:
    QHttpServer * server;
    zim::File * zimfile;
    bool isListening;
    int width;

    QString getLocalUrl(const QString & zimUrl);
    bool getResContent(const QString &filename, QByteArray &data);
    QString getContentType(const QString & file);
    void filter(QString & data);
};

#endif // ZIMSERVER_H
