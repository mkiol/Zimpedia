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
#include <QRegExp>
#include <string>

#include <zim/fileiterator.h>
#include <zim/blob.h>

#include "zimserver.h"
#include "settings.h"

#ifdef SAILFISH
#include "sailfishapp.h"
#endif

#ifdef BB10
#define Q_NULLPTR 0
#endif

ZimServer::ZimServer(QObject *parent) :
    QObject(parent)
{
    zimfile = Q_NULLPTR;

    // Local HTTP server
    server = new QHttpServer;
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(requestHandler(QHttpRequest*, QHttpResponse*)));

    Settings* s = Settings::instance();

    isListening = server->listen(s->getPort());
    if (!isListening) {
        qWarning() << "Unable to start HTTP server!";
    }

    emit listeningChanged();
}

bool ZimServer::loadZimFile()
{
    Settings* s = Settings::instance();

    s->articleModel->clear();

    QString filePath = s->getZimFile();
    if (filePath.isEmpty()) {
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        emit loadedChanged();
        return false;
    }
    if (!QFile::exists(filePath)) {
        qWarning() << "ZIM file" << filePath << "doesn't exist!";
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        s->setZimFile("");
        emit loadedChanged();
        return false;
    }

    try
    {
        zimfile = new zim::File(filePath.toStdString());
    }
    catch (const std::exception& e)
    {
        qWarning() << "Unable to open ZIM file" << filePath << "!";
        qWarning() << "Details:" << e.what();
        if (zimfile != Q_NULLPTR) {
            delete zimfile;
            zimfile = Q_NULLPTR;
        }
        s->setZimFile("");
        emit loadedChanged();
        return false;
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

void ZimServer::setScreenSize(int width)
{
    //qDebug() << "setScreenSize" << width;
    this->width = width;
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

void ZimServer::requestHandler(QHttpRequest *req, QHttpResponse *resp)
{
    //qDebug() << "requestHandler, URL:" << req->url().path();
    //qDebug() << "requestHandler, URL:" << QString::fromStdString(zim::urldecode(req->url().path().toStdString()));

    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(500);
        resp->end();
        return;
    }

    QStringList pathlist = req->url().path().split("/");
    if (pathlist.size() < 3) {
        qWarning() << "Invalid Url!";
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end();
        return;
    }

    // Internal Zimpedia resources, namespace = R
    if (pathlist.at(1) == "R" || pathlist.at(1) == "r") {
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
    }

    QString zimUrl = QStringList(pathlist.mid(1,pathlist.size()-1)).join("/");

    //qDebug() << "zimUrl:" << zimUrl;
    //qDebug() << "query:" << req->url().query();

    /*QString resUrl = "";
    if (req->url().query().contains("&modules=jquery%2Cmediawiki&only=scripts"))
        resUrl = "/r/jquery.js";
    if (!resUrl.isEmpty()) {
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Location", resUrl);
        resp->setHeader("Connection", "close");
        resp->writeHead(302);
        resp->end();
        return;
    }*/

    zim::File::const_iterator it = zimfile->find(zim::urldecode(zimUrl.toStdString()));

    if (it == zimfile->end()) {
      qWarning() << "Article not found!";
      resp->setHeader("Content-Length", "0");
      resp->setHeader("Connection", "close");
      resp->writeHead(404);
      resp->end();
      return;
    }

    /*qDebug() << "Url:" << QString::fromStdString(it->getLongUrl())
             << "Title:" << QString::fromStdString(it->getTitle())
             << "Redirection:" << it->isRedirect();*/

    /*if (it->isRedirect()) {
      QString redirectionUrl = QString(QUrl("/" +QString::fromStdString(it->getRedirectArticle().getLongUrl())).toEncoded());
      qDebug() << "Redirection to" << redirectionUrl;
      resp->setHeader("Content-Length", "0");
      resp->setHeader("Location", redirectionUrl);
      resp->setHeader("Connection", "close");
      resp->writeHead(302);
      resp->end("");
      return;
    }*/

    if (it->isDeleted()) {
      qWarning() << "Article deleted!";
      resp->setHeader("Content-Length", "0");
      resp->setHeader("Connection", "close");
      resp->writeHead(404);
      resp->end();
      return;
    }

    zim::Article article = it->isRedirect() ? it->getRedirectArticle() : *it;
    zim::Blob zimblob = article.getData();

    QByteArray data;
    QString mimeType = QString::fromStdString(article.getMimeType());
    //qDebug() << mimeType;
    if (mimeType == "text/html") {
        mimeType == "text/html; charset=utf-8";
        QString dd = QString::fromUtf8(zimblob.data(), zimblob.size());
        filter(dd);
        data = dd.toUtf8();
    } else {
        data = QByteArray(zimblob.data(), zimblob.size());
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
    return QString("http://localhost:%1/%2").arg(s->getPort()).arg(zimUrl);
}

void ZimServer::filter(QString &data)
{
    QString scale;
    QString width;

    Settings* s = Settings::instance();
    switch (s->getFontSize()) {
    case 1:
        return;
        //scale = "1"; width = "device-width"; break;
#ifdef BB10
    case 2:
        scale = "1.3"; width = QString::number(this->width/2.6)+"px"; break;
    case 3:
        scale = "1.6"; width = QString::number(this->width/3.2)+"px"; break;
#elif SAILFISH
    case 2:
        scale = "1.3"; width = QString::number(this->width/2)+"px"; break;
    case 3:
        scale = "1.6"; width = QString::number(this->width/3)+"px"; break;
#endif
    default:
        scale = "1"; width = "device-width"; break;
    }

    QRegExp rxMetaViewport("<meta\\s[^>]*name\\s*=(\"viewport\"|'viewport')[^>]*>", Qt::CaseInsensitive);
    data.replace(rxMetaViewport,QString("<meta name=\"viewport\" content=\"width=%1px, initial-scale=%2\">").arg(width).arg(scale));
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
        s->articleModel->appendRow(new ArticleItem(QString::number(i), QString::fromUtf8(it->getTitle().data()), getLocalUrl(QString::fromStdString(it->getLongUrl()))));
        ++i;
    }

    emit searchReady();
}
