/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ZIMSERVER_H
#define ZIMSERVER_H

#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qhttpserver.h>
#include <zim/file.h>

#include <QList>
#include <QObject>
#include <QThread>
#include <string>

#include "filemodel.h"

struct SearchResult {
    QString title;
    QString url;
};

class ZimServer : public QThread {
    Q_OBJECT

    Q_PROPERTY(bool loaded READ getLoaded NOTIFY loadedChanged)
    Q_PROPERTY(bool listening READ getListening NOTIFY listeningChanged)
    Q_PROPERTY(bool hasMainPage READ getHasMainPage NOTIFY zimChanged)
    Q_PROPERTY(bool busy READ getBusy NOTIFY busyChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY zimChanged)
    Q_PROPERTY(QString language READ getLanguage NOTIFY zimChanged)
    Q_PROPERTY(QString uuid READ getUuid NOTIFY zimChanged)
    Q_PROPERTY(QString favicon READ getFavicon NOTIFY zimChanged)

   public:
    static ZimServer *instance(QObject *parent = nullptr);

    static std::string stringQtoStd(const QString &s);
    static QString stringStdToQ(const std::string &s);
    static bool getArticle(zim::File *zimfile, const QString &zimUrl,
                           QByteArray &data, QString &mimeType);

    Q_INVOKABLE bool loadZimFile();
    Q_INVOKABLE QString serverUrl();
    Q_INVOKABLE void getArticleAsync(const QString &zimUrl);
    Q_INVOKABLE void openUrl(const QString &url, const QString &title);
    Q_INVOKABLE QString getTitleFromUrl(const QString &url);
    QList<SearchResult> search(const QString &value);

    bool getLoaded();
    bool getListening();
    bool getHasMainPage();
    QString getTitle();
    QString getLanguage();
    QString getUuid();
    QString getFavicon();
    bool getBusy();

   signals:
    void error();
    void loadedChanged();
    void zimChanged();
    void listeningChanged();
    void searchReady();
    void articleReady(QString article);
    void urlReady(QString url, QString title);
    void busyChanged();

   private slots:
    void requestHandler(QHttpRequest *req, QHttpResponse *resp);

   private:
    static const int maxSearch = 20;  // max size of search result
    static ZimServer *m_instance;

    explicit ZimServer(QObject *parent = nullptr);

    QHttpServer *server;
    zim::File *zimfile;
    bool isListening;
    bool hasMainPage;
    bool ftindex;
    ZimMetaData metadata;
    QString urlToAsyncGet;
    bool busy;

    void run();
    QString getLocalUrl(const QString &zimUrl);
    bool getResContent(const QString &filename, QByteArray &data);
    QString getContentType(const QString &file);
    void filter(QString &data);
    bool getArticle(const QString &zimUrl, QByteArray &data, QString &mimeType);
    bool loadZimPath(const QString &path);
    bool loadZimFileByUuid(const QString &uuid);
    void fixUrl(QString &url);
};

#endif  // ZIMSERVER_H
