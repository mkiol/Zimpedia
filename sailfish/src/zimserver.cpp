/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QRegExp>
#include <string>

#include <zim/fileiterator.h>
#include <zim/fileheader.h>
#include <zim/search.h>
#include <zim/error.h>
#include <zim/blob.h>

#include "zimserver.h"
#include "settings.h"
#include "filefinder.h"

#ifdef SAILFISH
#include "sailfishapp.h"
#endif

#ifdef BB10
#define Q_NULLPTR 0
#endif

ZimServer::ZimServer(QObject *parent) :
    QThread(parent)
{
    zimfile = Q_NULLPTR;

    // Thread
    QObject::connect(this, &QThread::finished, this, &ZimServer::finishedHandler);

    // Local HTTP server
    server = new QHttpServer;
    QObject::connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(requestHandler(QHttpRequest*, QHttpResponse*)));

    Settings* s = Settings::instance();

    isListening = server->listen(s->getPort());
    if (!isListening) {
        qWarning() << "Unable to start HTTP server!";
    }

    emit listeningChanged();
}

void ZimServer::finishedHandler()
{
    //qDebug() << "finishedHandler";
}

void ZimServer::run()
{
    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        emit articleReady("");
        return;
    }

    QString zimUrl = urlToAsyncGet;

    QByteArray data;
    QString mimeType;
    if (!getArticle(zimUrl, data, mimeType)) {
        emit articleReady("");
        return;
    }

    if (mimeType == "text/html") {
        mimeType == "text/html; charset=utf-8";
        QString dd = QString::fromUtf8(data);
        filter(dd);
        emit articleReady(dd);
        //data = dd.toUtf8();
    } else {
        emit articleReady(QString(data));
    }
}

bool ZimServer::loadZimFile()
{
    Settings* s = Settings::instance();

    s->articleModel->clear();
    this->hasMainPage = false;
    this->metadata.clear();

    this->metadata.path = s->getZimFile();
    if (this->metadata.path.isEmpty()) {
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        emit loadedChanged();
        return false;
    }

    if (!QFile::exists(this->metadata.path)) {
        qWarning() << "ZIM file" << this->metadata.path << "doesn't exist!";
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        s->setZimFile("");
        emit loadedChanged();
        return false;
    }

    this->metadata.fields =
            ZimMetaData::Title |
            ZimMetaData::Checksum |
            ZimMetaData::Language |
            ZimMetaData::Favicon;
    if (!FileFinder::scanZimFile(this->metadata)) {
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        this->metadata.clear();
        emit loadedChanged();
        return false;
    }

    try
    {
        zimfile = new zim::File(this->metadata.path.toStdString());
    }
    catch (const std::exception& e)
    {
        qWarning() << "Unable to open ZIM file" << this->metadata.path << "!";
        qWarning() << "Details:" << e.what();
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        s->setZimFile("");
        this->metadata.clear();
        emit loadedChanged();
        return false;
    }

    QByteArray data; QString mimeType;
    // Main page
    if (!zimfile->getFileheader().hasMainPage()) {
        this->hasMainPage = false;
    } else {
        // Wikipedia ZIM returns true for hasMainPage but has empty main page
        this->hasMainPage = getArticle("A/mainpage", data, mimeType);
    }

    emit loadedChanged();

    return true;
}

bool ZimServer::getLoaded()
{
    return zimfile != Q_NULLPTR;
}

bool ZimServer::getListening()
{
    return isListening;
}

bool ZimServer::getResContent(const QString &filename, QByteArray &data)
{
#ifdef SAILFISH
    QFile file(SailfishApp::pathTo("res/" + filename).toLocalFile());
#elif BB10
    QFile file(QDir::currentPath()+"/app/native/res/" + filename);
#else
    return false;
#endif

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open" << filename << "for reading: " << file.errorString();
        file.close();
        return false;
    }

    data = file.readAll();
    file.close();

    return true;
}

QString ZimServer::getContentType(const QString & file)
{
    QStringList l = file.split(".");

    QString ext = "";
    if (l.length()>0)
        ext = l[l.length()-1];

    return ext == "css" ? "text/css; charset=utf-8" :
           ext == "gif" ? "image/gif" :
           ext == "png" ? "image/png" :
           ext == "js" ? "application/x-javascript; charset=utf-8" :
           "text/html; charset=utf-8";
}

bool ZimServer::getHasMainPage()
{
    return this->hasMainPage;
}

QString ZimServer::getTitle()
{
    return this->metadata.title;
}

QString ZimServer::getLanguage()
{
    return this->metadata.language;
}

QString ZimServer::getUuid()
{
    return this->metadata.checksum;
}

QString ZimServer::getFavicon()
{
    return this->metadata.favicon;
}

bool ZimServer::getArticle(const QString zimUrl, QByteArray &data, QString &mimeType)
{
    return ZimServer::getArticle(this->zimfile, zimUrl, data, mimeType);
}

bool ZimServer::getArticle(zim::File *zimfile, const QString zimUrl, QByteArray &data, QString &mimeType)
{
    //qDebug() << "getArticle static, zimUrl:" << zimUrl;

    zim::Article article;

    if (zimUrl == "A/mainpage") {
        if (!zimfile->getFileheader().hasMainPage()) {
            qWarning() << "Main page not found!";
            return false;
        }

        zim::Fileheader header = zimfile->getFileheader();

        try {
            article = zimfile->getArticle(header.getMainPage());
        } catch (zim::ZimFileFormatError e) {
            qWarning() << e.what();
            qWarning() << "Main page not found!";
            return false;
        }

    } else {
        zim::File::const_iterator it = zimfile->find(zim::urldecode(zimUrl.toStdString()));

        if (it == zimfile->end()) {
          qWarning() << "Article not found!";
          return false;
        }

        if (it->isDeleted()) {
          qWarning() << "Article deleted!";
          return false;
        }

        article = it->isRedirect() ? it->getRedirectArticle() : *it;
    }

    zim::Blob zimblob = article.getData();

    if (zimblob.size() == 0) {
        qWarning() << "Article empty!";
        return false;
    }

    data = QByteArray(zimblob.data(), zimblob.size());

    try {
        mimeType = QString::fromStdString(article.getMimeType());
    } catch (std::runtime_error e) {
        qWarning() << e.what();
        qWarning() << "Unable to get mimeType!";
    }

    return true;
}

void ZimServer::getArticleAsync(const QString &zimUrl)
{
    //qDebug() << "getArticleAsync, zimUrl:" << zimUrl;
    QStringList parts = zimUrl.split("/");
    if (parts.length()<2) {
        qWarning() << "Bad url!";
        return;
    }
    urlToAsyncGet = parts.mid(parts.length()-2).join("/");
    this->start(QThread::IdlePriority);
}

void ZimServer::requestHandler(QHttpRequest *req, QHttpResponse *resp)
{
    //qDebug() << "requestHandler";
    //qDebug() << "req->url():" << req->url().path();

    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(500);
        resp->end();
        return;
    }

    QStringList pathlist = req->url().path().split("/");
    QString zimUrl;
    if (pathlist.last() == "mainpage") {
        zimUrl = "A/mainpage";
    } else {
        if (pathlist.length() < 2) {
            qWarning() << "Invalid Url!";
            resp->setHeader("Content-Length", "0");
            resp->setHeader("Connection", "close");
            resp->writeHead(404);
            resp->end();
            return;
        }

        // Internal Zimpedia resources, namespace = R
        /*if (pathlist.at(1) == "R" || pathlist.at(1) == "r") {
            qWarning() << "Resource namespace request!";
            QString filename = pathlist.at(2);
            QByteArray body;
            if (getResContent(filename, body)) {
                QString contentType = getContentType(filename);
                resp->setHeader("Content-Length", QString::number(body.size()));
                resp->setHeader("Content-Type", contentType);
                resp->setHeader("Connection", "close");
                resp->writeHead(200);
                resp->end(body);
                return;
            } else {
                qWarning() << "Unable to get resource file:" << filename;
                resp->setHeader("Content-Length", "0");
                resp->setHeader("Connection", "close");
                resp->writeHead(404);
                resp->end();
                return;
            }
        }*/

        zimUrl = pathlist.mid(pathlist.length()-2).join("/");
    }

    QByteArray data;
    QString mimeType;
    if (!getArticle(zimUrl, data, mimeType)) {
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end();
        return;
    }

    if (mimeType == "text/html") {
        mimeType == "text/html; charset=utf-8";
        QString dd = QString::fromUtf8(data);
        filter(dd);
        data = dd.toUtf8();
    }

    resp->setHeader("Content-Length", QString::number(data.size()));
    resp->setHeader("Content-Type", mimeType);
    resp->setHeader("Connection", "close");
    resp->writeHead(200);
    resp->end(data);
}

QString ZimServer::getLocalUrl(const QString &zimUrl)
{
    Settings* s = Settings::instance();
    return QString("http://localhost:%1/%2/%3").arg(s->getPort()).arg(this->metadata.checksum).arg(zimUrl);
}

void ZimServer::findTitle(const QString &title)
{
    //qDebug() << "findTitle:" << title;

    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        return;
    }

    Settings* s = Settings::instance();

    s->articleModel->clear();

    if (title.trimmed().isEmpty()) {
        emit searchReady();
        return;
    }

    std::string std_title(title.toUtf8().constData());
    zim::File::const_iterator it = zimfile->findByTitle('A', std_title);

    for (int i = 0; it != zimfile->end() && i < 10; ++it, ++i) {
        //qDebug() << QString::fromStdString(it->getLongUrl()) << QString::fromUtf8(it->getLongUrl().data());
        s->articleModel->appendRow(new ArticleItem(QString::number(i),
                                                   QString::fromUtf8(it->getTitle().data()),
                                                   getLocalUrl(QString::fromStdString(it->getLongUrl())))
                                   );
        ++i;
    }

    emit searchReady();
}

QString ZimServer::serverUrl()
{
    Settings* s = Settings::instance();
    return QString("http://localhost:%1/%2/").arg(s->getPort()).arg(this->metadata.checksum);
}

void ZimServer::filter(QString &data)
{
    int pos;
    auto fixUrl = [](QString& url) {
        if (url.startsWith("..")) {
            url = url.right(url.length()-2);
        }
        if (url.startsWith("/")) {
            url = url.right(url.length()-1);
        }
    };

    // <link href=\"../-/s/css_modules/ext.kartographer.link.css\" rel=\"stylesheet\" type=\"text/css\">
    QRegExp rxStyle("<link\\s[^>]*href\\s*=\\s*\"(\\S*)\"[^>]*>", Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0) {
        pos = rxStyle.indexIn(data, pos);
        if (pos >= 0) {
            QString url = rxStyle.cap(1); fixUrl(url);
            QByteArray emData; QString mimeType;
            if (getArticle(url, emData, mimeType)) {
                data.replace(pos, rxStyle.cap(0).length(), "<style type=\"text/css\">" + emData + "</style>");
            } else {
                qWarning() << "Can't get embeded style content!";
            }

            ++pos;
        }
    }

    // <script src=\"../-/j/js_modules/startup.js\"></script>
    QRegExp rxScript("<script\\s[^>]*src\\s*=\\s*\"(\\S*)\"[^>]*></script>", Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0) {
        pos = rxScript.indexIn(data, pos);
        if (pos >= 0) {
            QString url = rxScript.cap(1); fixUrl(url);
            QByteArray emData; QString mimeType;
            if (getArticle(url, emData, mimeType)) {
                data.replace(pos, rxScript.cap(0).length(), "<script type=\"text/javascript\">" + emData + "</script>");
            } else {
                qWarning() << "Can't get embeded script content!";
            }

            ++pos;
        }
    }

    // <img src=\"../I/m/AloraMountain.JPG\" data-file-width=\"3648\" data-file-height=\"2736\"
    // data-file-type=\"bitmap\" height=\"150\" width=\"200\" id=\"mwBQ\">
    QRegExp rxImg("<img\\s[^>]*src\\s*=\\s*\"(\\S*)\"[^>]*>", Qt::CaseInsensitive);
    pos = 0;
    while (pos >= 0) {
        pos = rxImg.indexIn(data, pos);
        if (pos >= 0) {
            QString url = rxImg.cap(1); fixUrl(url);
            QByteArray emData; QString mimeType;
            if (getArticle(url, emData, mimeType)) {
                QString imgTag = rxImg.cap(0);
                imgTag.replace(rxImg.cap(1),"data:"+mimeType+";base64,"+emData.toBase64());
                data.replace(pos, rxImg.cap(0).length(), imgTag);
            } else {
                qWarning() << "Can't get embeded image content!";
            }

            ++pos;
        }
    }
}
