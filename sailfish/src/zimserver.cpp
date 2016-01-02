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

#include <zim/fileiterator.h>
#include <zim/blob.h>

#include "zimserver.h"
#include "settings.h"

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

void ZimServer::requestHandler(QHttpRequest *req, QHttpResponse *resp)
{
    qDebug() << "requestHandler, URL:" << req->url().toString();

    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(500);
        resp->end("");
        return;
    }

    QStringList pathlist = req->url().path().split("/");
    if (pathlist.size() < 3) {
        qWarning() << "Invalid Url!";
        resp->setHeader("Content-Length", "0");
        resp->setHeader("Connection", "close");
        resp->writeHead(404);
        resp->end("");
        return;
    }

    QString zimUrl = QStringList(pathlist.mid(1,pathlist.size()-1)).join("/");

    zim::File::const_iterator it = zimfile->find(zimUrl.toStdString());

    if (it == zimfile->end()) {
      qWarning() << "Article not found!";
      resp->setHeader("Content-Length", "0");
      resp->setHeader("Connection", "close");
      resp->writeHead(404);
      resp->end("");
      return;
    }

    if (it->isRedirect()) {
      resp->setHeader("Content-Length", "0");
      resp->setHeader("Location", QString::fromStdString(it->getRedirectArticle().getUrl()).toLatin1());
      resp->setHeader("Connection", "close");
      resp->writeHead(302);
      resp->end("");
      return;
    }

    zim::Blob zimblob = it->getData();
    QByteArray data(zimblob.data(), zimblob.size());

    resp->setHeader("Content-Length", QString::number(data.size()));
    resp->setHeader("Content-Type", QString::fromStdString(it->getMimeType()).toLatin1());
    resp->setHeader("Connection", "close");
    resp->writeHead(200);
    resp->end(data);
}

QString ZimServer::getLocalUrl(const QString &zimUrl)
{
    Settings* s = Settings::instance();
    return QString("http://localhost:%1/%2").arg(s->getPort()).arg(zimUrl);
}

void ZimServer::findTitle(const QString &title)
{
    if (!getLoaded()) {
        qWarning() << "ZIM file not loaded!";
        return;
    }

    Settings* s = Settings::instance();

    s->articleModel->clear();

    if (title.trimmed().isEmpty())
        return;

    zim::File::const_iterator it = zimfile->findByTitle('A', title.toStdString());

    for (int i = 0; it != zimfile->end() && i < 10; ++it, ++i) {
        s->articleModel->appendRow(new ArticleItem(QString::number(i), QString::fromStdString(it->getTitle()), getLocalUrl(QString::fromStdString(it->getLongUrl()))));
        ++i;
    }

}
