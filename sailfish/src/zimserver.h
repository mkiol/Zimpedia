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
#include <QThread>

#include <zim/file.h>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

class ZimServer : public QThread
{
    Q_OBJECT

    Q_PROPERTY (bool loaded READ getLoaded NOTIFY loadedChanged)
    Q_PROPERTY (bool listening READ getListening NOTIFY listeningChanged)

public:
    explicit ZimServer(QObject *parent = 0);
    Q_INVOKABLE bool loadZimFile();
    Q_INVOKABLE void findTitle(const QString &title);
    Q_INVOKABLE QString serverUrl();
    Q_INVOKABLE void getArticleAsync(const QString &zimUrl);

    bool getLoaded();
    bool getListening();

signals:
    void error();
    void loadedChanged();
    void listeningChanged();
    void searchReady();
    void articleReady(QString article);

private slots:
    void requestHandler(QHttpRequest *req, QHttpResponse *resp);
    void finishedHandler();

private:
    QHttpServer * server;
    zim::File * zimfile;
    bool isListening;
    QString urlToAsyncGet;

    void run();
    QString getLocalUrl(const QString & zimUrl);
    bool getResContent(const QString &filename, QByteArray &data);
    QString getContentType(const QString & file);
    void filter(QString & data);
    bool getArticle(const QString zimUrl, QByteArray &data, QString &mimeType);
};

#endif // ZIMSERVER_H
