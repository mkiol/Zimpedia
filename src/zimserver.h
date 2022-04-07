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
#include <zim/archive.h>

#include <QList>
#include <QObject>
#include <QThread>
#include <optional>
#include <string>

#include "filemodel.h"

struct SearchResult {
    QString title;
    QUrl url;
};

struct ArticleResult {
    QByteArray content;
    QString mime;
};

class ZimServer : public QThread {
    Q_OBJECT

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool hasMainPage READ hasMainPage NOTIFY archiveChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString title READ title NOTIFY archiveChanged)
    Q_PROPERTY(QString language READ language NOTIFY archiveChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY archiveChanged)
    Q_PROPERTY(QString favicon READ favicon NOTIFY archiveChanged)

   public:
    static const int port = 9091;
    static ZimServer *instance(QObject *parent = nullptr);

    inline static std::string stringQtoStd(const QString &s) {
        return s.toStdString();
    }

    inline static QString stringStdToQ(const std::string &s) {
        return QString::fromStdString(s);
    }

    static std::optional<ArticleResult> article(const zim::Archive &archive,
                                                const QString &path);

    Q_INVOKABLE bool loadZimFile();
    Q_INVOKABLE QUrl serverUrl() const;
    Q_INVOKABLE void articleAsync(const QString &url);
    Q_INVOKABLE void openUrl(const QString &url, const QString &title);
    Q_INVOKABLE QString titleFromUrl(const QUrl &url);
    QList<SearchResult> search(QString phrase) const;

   signals:
    void error();
    void loadedChanged();
    void archiveChanged();
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

    QHttpServer m_server;
    std::optional<zim::Archive> m_archive;

    bool isListening;
    bool ftindex;
    ZimMetaData m_meta;
    QString urlToAsyncGet;
    bool m_busy = false;
    bool m_hasMainPage = false;

    inline bool busy() const { return m_busy; }
    inline bool loaded() const { return m_archive.has_value(); }
    inline bool hasMainPage() const { return m_hasMainPage; }
    inline QString title() const { return m_meta.title; }
    inline QString language() const { return m_meta.language; }
    inline QString uuid() const { return m_meta.checksum; }
    inline QString favicon() const { return m_meta.favicon; }
    void run() override;
    QUrl localUrl(const QString &path) const;
    static std::optional<QByteArray> resContent(const QString &filename);
    static QString contentType(const QString &file);
    void filter(QString *data);
    std::optional<ArticleResult> article(const QString &path);
    bool loadZimPath(const QString &path);
    bool loadZimFileByUuid(const QString &uuid);
    [[nodiscard]] static QString fixPath(QString path);
    void setBusy(bool busy);
    void searchFullText(const QString &phrase, int maxSize,
                        QList<SearchResult> *result) const;
    void searchTitle(const QString &phrase, int maxSize,
                     QList<SearchResult> *result) const;
};

#endif  // ZIMSERVER_H
