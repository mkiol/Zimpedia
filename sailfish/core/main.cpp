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
#include <QTimer>
#include "abstractitemmodel.hpp"
#include "webimageview.h"
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

#include "info.h"
#include "zimserver.h"
#include "articlemodel.h"
#include "settings.h"
#include "filemodel.h"
#include "utils.h"
#include "filefinder.h"
#include "zimmetadatareader.h"
#include "bookmarkmodel.h"
#include "bookmarks.h"

Q_DECL_EXPORT int main(int argc, char **argv)
{
#ifdef BB10
    qmlRegisterType<QAbstractItemModel>();
    qmlRegisterType <AbstractItemModel> ("com.kdab.components", 1, 0, "AbstractItemModel");
    qmlRegisterType<WebImageView> ("org.labsquare", 1, 0, "WebImageView");
    qmlRegisterType<FileModel>("net.mkiol.zimpedia.FileModel", 1, 0, "FileModel");
    qmlRegisterType<ZimMetaDataReader>("net.mkiol.zimpedia.ZimMetaDataReader", 1, 0, "ZimMetaDataReader");
    qmlRegisterType<BookmarkModel>("net.mkiol.zimpedia.BookmarkModel", 1, 0, "BookmarkModel");
    qmlRegisterType<QTimer>("net.mkiol.zimpedia.QTimer", 1, 0, "QTimer");
#elif SAILFISH
    qmlRegisterType<FileModel>("harbour.zimpedia.FileModel", 1, 0, "FileModel");
    qmlRegisterType<ZimMetaDataReader>("harbour.zimpedia.ZimMetaDataReader", 1, 0, "ZimMetaDataReader");
    qmlRegisterType<BookmarkModel>("harbour.zimpedia.BookmarkModel", 1, 0, "BookmarkModel");
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

    QTranslator translator;
    QString locale = QLocale::system().name();
    //qDebug() << locale;
#ifdef BB10
    if (translator.load("Zimpedia_" + locale, "app/native/qm")) {
        app.installTranslator(&translator);
    } else {
        qWarning() << "Couldn't load translation for locale " + locale + " from app/native/qm";
    }
#elif SAILFISH
    if(translator.load("Zimpedia_" + locale, SailfishApp::pathTo("translations").toLocalFile())) {
        app->installTranslator(&translator);
    } else {
        qWarning() << "Couldn't load translation for locale " + locale + " from " + SailfishApp::pathTo("translations").toLocalFile();
    }
#endif

    context->setContextProperty("APP_NAME", Zimpedia::APP_NAME);
    context->setContextProperty("APP_VERSION", Zimpedia::APP_VERSION);
    context->setContextProperty("COPYRIGHT_YEAR", Zimpedia::COPYRIGHT_YEAR);
    context->setContextProperty("AUTHOR", Zimpedia::AUTHOR);
    context->setContextProperty("SUPPORT_EMAIL", Zimpedia::SUPPORT_EMAIL);
    context->setContextProperty("PAGE", Zimpedia::PAGE);
    context->setContextProperty("LICENSE", Zimpedia::LICENSE);
    context->setContextProperty("LICENSE_URL", Zimpedia::LICENSE_URL);

    Settings* s = Settings::instance();
    Bookmarks* b = Bookmarks::instance();
    ZimServer zimServer;
    ArticleModel articleModel;
    Utils utils;
    s->articleModel = &articleModel;

    context->setContextProperty("settings", s);
    context->setContextProperty("zimServer", &zimServer);
    context->setContextProperty("articleModel", &articleModel);
    context->setContextProperty("utils", &utils);
    context->setContextProperty("bookmarks", b);

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
