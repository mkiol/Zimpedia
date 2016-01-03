/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#define _FILE_OFFSET_BITS 64

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QScopedPointer>
#include <QtQml>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>
#include <QFileInfo>
#include <sailfishapp.h>

#include "zimserver.h"
#include "articlemodel.h"
#include "settings.h"
#include "filemodel.h"

static const char *APP_NAME = "Zimpedia";
static const char *AUTHOR = "Michal Kosciesza <michal@mkiol.net>";
static const char *PAGE = "https://github.com/mkiol/Zimpedia";
static const char *LICENSE = "http://mozilla.org/MPL/2.0/";
static const char *VERSION = "1.0";

int main(int argc, char *argv[])
{
    qmlRegisterType<FileModel>("harbour.zimpedia.FileModel", 1, 0, "FileModel");
    qRegisterMetaType<QFileInfo>("QFileInfo");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QTranslator translator;
    QString locale = QLocale::system().name();
    //locale="pl";
    if(!translator.load("harbour-zimpedia-" + locale, SailfishApp::pathTo("translations").toLocalFile())) {
        qDebug() << "Couldn't load translation for locale " + locale + " from " + SailfishApp::pathTo("translations").toLocalFile();
    }
    app->installTranslator(&translator);

    view->rootContext()->setContextProperty("APP_NAME", APP_NAME);
    view->rootContext()->setContextProperty("VERSION", VERSION);
    view->rootContext()->setContextProperty("AUTHOR", AUTHOR);
    view->rootContext()->setContextProperty("PAGE", PAGE);
    view->rootContext()->setContextProperty("LICENSE", LICENSE);

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    Settings* s = Settings::instance();
    ZimServer* zimServer = new ZimServer();
    ArticleModel* articleModel = new ArticleModel();
    s->articleModel = articleModel;

    view->rootContext()->setContextProperty("settings", s);
    view->rootContext()->setContextProperty("zimServer", zimServer);
    view->rootContext()->setContextProperty("articleModel", articleModel);

    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->show();

    return app->exec();
}
