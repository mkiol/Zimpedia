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
#include <QVariantList>
#include <memory>
#include <optional>
#include <string>

#include "filemodel.h"

struct SearchResult {
    QString title;
    QUrl url;
    QString uuid;
    int score = 0;
};

struct ArticleResult {
    QByteArray content;
    QString mime;
};

class ZimServer : public QThread {
    Q_OBJECT

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(int filesCount READ filesCount NOTIFY loadedChanged)
    Q_PROPERTY(QVariantList files READ files NOTIFY filesChanged)
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
    [[nodiscard]] static std::pair<QString, QString> parseUrl(const QUrl &url);

    Q_INVOKABLE bool loadZim();
    Q_INVOKABLE inline void unLoadZim(const QString &uuid) {
        unLoadZimUuidEmit(uuid);
    }
    Q_INVOKABLE bool isServerUrl(const QUrl &url) const;
    Q_INVOKABLE QUrl urlToMainPage(const QString &uuid) const;
    Q_INVOKABLE void articleAsync(const QUrl &url);
    Q_INVOKABLE void openUrl(const QUrl &url, const QString &title);
    Q_INVOKABLE QString titleFromUrl(const QUrl &url) const;
    QList<SearchResult> search(QString phrase) const;

   signals:
    void error();
    void loadedChanged();
    void filesChanged();
    void searchReady();
    void articleReady(const QString &article);
    void urlReady(const QUrl &url, const QString &title);
    void busyChanged();

   private:
    enum class WorkType { none, article, loadZim };
    static const int maxSearch = 20;  // max size of search result
    static ZimServer *m_instance;

    QHttpServer m_server;
    QHash<QString, std::shared_ptr<zim::Archive>> m_archive;  // uuid => archive
    QHash<QString, ZimMetaData> m_meta;                       // uuid => meta
    std::pair<QString, QString> m_articleToAsyncGet;  // article path, uuid
    QString m_lastUuid;
    bool m_loaded = false;
    bool m_busy = false;
    bool m_delayedLoadZim = false;
    QStringList m_uuidsToLoad;
    WorkType m_workType = WorkType::none;

    explicit ZimServer(QObject *parent = nullptr);
    inline auto busy() const { return m_busy; }
    inline auto loaded() const { return m_loaded; }
    inline auto filesCount() const { return m_archive.size(); }
    void run() override;
    static QUrl localUrl(const QString &path, const QString &uuid);
    static std::optional<QByteArray> resContent(const QString &filename);
    static QString contentType(const QString &file);
    void filter(const QString &uuid, QString *data);
    std::optional<ArticleResult> article(const QString &path,
                                         const QString &uuid);
    bool loadZimUuids(QStringList &&uuids);
    bool loadZimUuid(const QString &uuids);
    bool unLoadZimUuid(const QString &uuid);
    bool loadZimUuidEmit(const QString &uuid);
    bool unLoadZimUuidEmit(const QString &uuid);
    [[nodiscard]] static QString fixPath(QString path);
    void setBusy(bool busy);
    void setLoaded(bool loaded);
    void searchFullText(const QString &phrase, int maxSize,
                        QList<SearchResult> *result) const;
    static void searchTitle(const zim::Archive &archive, const QString &uuid,
                            const QString &phrase, int maxSize,
                            QList<SearchResult> *result);
    void searchTitle(const QString &phrase, int maxSize,
                     QList<SearchResult> *result) const;
    inline auto uuidLoaded(const QString &uuid) const {
        return m_archive.contains(uuid);
    }
    void handleFilemodelChanged();
    void articleAsyncWork();
    void loadZimAsyncWork();
    QVariantList files() const;
    void handleHttpRequest(QHttpRequest *req, QHttpResponse *resp);
};

#endif  // ZIMSERVER_H
