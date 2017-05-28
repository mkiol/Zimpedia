/*
  Copyright (C) 2016 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#define _FILE_OFFSET_BITS 64

#include <QLocale>
#include <QTranslator>
#include <QFileInfo>

#ifdef BB10
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QAbstractItemModel>
#include "abstractitemmodel.hpp"
using namespace bb::cascades;
#elif SAILFISH
#include <QGuiApplication>
#include <QScopedPointer>
#include <QtQml>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <sailfishapp.h>
#include "iconprovider.h"
#endif

#include "zimserver.h"
#include "articlemodel.h"
#include "settings.h"
#include "filemodel.h"
#include "utils.h"
#include "filefinder.h"
#include "zimmetadatareader.h"

static const char *APP_NAME = "Zimpedia";
static const char *AUTHOR = "Michal Kosciesza <michal@mkiol.net>";
static const char *PAGE = "https://github.com/mkiol/Zimpedia";
static const char *LICENSE = "http://mozilla.org/MPL/2.0/";
static const char *VERSION = "2.0.0";

Q_DECL_EXPORT int main(int argc, char **argv)
{
#ifdef BB10
    qmlRegisterType<QAbstractItemModel>();
    qmlRegisterType<FileModel>("net.mkiol.zimpedia.FileModel", 1, 0, "FileModel");
    qmlRegisterType <AbstractItemModel> ("com.kdab.components", 1, 0, "AbstractItemModel");
#elif SAILFISH
    qmlRegisterType<FileModel>("harbour.zimpedia.FileModel", 1, 0, "FileModel");
    qmlRegisterType<ZimMetaDataReader>("harbour.zimpedia.ZimMetaDataReader", 1, 0, "ZimMetaDataReader");
#endif
    qRegisterMetaType<QFileInfo>("QFileInfo");
    qRegisterMetaType<ZimMetaData>("ZimMetaData");

#ifdef BB10
    Application app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(VERSION);
    QmlDocument* qml = QmlDocument::create("asset:///main.qml");
    QDeclarativeContext* context = qml->documentContext();
    QDeclarativeEngine* engine = qml->defaultDeclarativeEngine();
#elif SAILFISH
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQmlContext* context = view->rootContext();
    QQmlEngine* engine = view->engine();
    engine->addImageProvider(QLatin1String("icons"), new IconProvider);
#endif

#ifdef SAILFISH
    QTranslator translator;
    QString locale = QLocale::system().name();
    //locale="de";
    if(!translator.load("harbour-zimpedia-" + locale, SailfishApp::pathTo("translations").toLocalFile())) {
        qDebug() << "Couldn't load translation for locale " + locale + " from " + SailfishApp::pathTo("translations").toLocalFile();
    }
    app->installTranslator(&translator);
#endif

    context->setContextProperty("APP_NAME", APP_NAME);
    context->setContextProperty("VERSION", VERSION);
    context->setContextProperty("AUTHOR", AUTHOR);
    context->setContextProperty("PAGE", PAGE);
    context->setContextProperty("LICENSE", LICENSE);

    Settings* s = Settings::instance();
    ZimServer zimServer;
    ArticleModel articleModel;
    Utils utils;
    s->articleModel = &articleModel;

    context->setContextProperty("settings", s);
    context->setContextProperty("zimServer", &zimServer);
    context->setContextProperty("articleModel", &articleModel);
    context->setContextProperty("utils", &utils);

    QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

#ifdef BB10
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
    return Application::exec();
#elif SAILFISH
    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->show();
    return app->exec();
#endif
}
