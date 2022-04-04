/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define _FILE_OFFSET_BITS 64

#include <sailfishapp.h>

#include <QDebug>
#include <QFileInfo>
#include <QGuiApplication>
#include <QLocale>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QTranslator>
#include <QtQml>

#include "articlemodel.h"
#include "bookmarkmodel.h"
#include "filemodel.h"
#include "iconprovider.h"
#include "info.h"
#include "settings.h"
#include "utils.h"
#include "zimmetadatareader.h"
#include "zimserver.h"

void registerTypes() {
    qmlRegisterType<ZimMetaDataReader>("harbour.zimpedia.ZimMetaDataReader", 1,
                                       0, "ZimMetaDataReader");
    qmlRegisterUncreatableType<Settings>(
        "harbour.zimpedia.Settings", 1, 0, "Settings",
        QStringLiteral("Settings is a singleton"));
    qRegisterMetaType<QFileInfo>("QFileInfo");
    qRegisterMetaType<ZimMetaData>("ZimMetaData");
}

void installTranslator(QGuiApplication* app) {
    auto translator = new QTranslator{};
    auto transDir =
        SailfishApp::pathTo(QStringLiteral("translations")).toLocalFile();
    if (!translator->load(QLocale{}, QStringLiteral("Zimpedia"),
                          QStringLiteral("_"), transDir,
                          QStringLiteral(".qm"))) {
        qDebug() << "Cannot load translation:" << QLocale::system().name()
                 << transDir;
        if (!translator->load(QStringLiteral("Zimpedia_en"), transDir)) {
            qDebug() << "Cannot load default translation";
            delete translator;
            return;
        }
    }

    app->installTranslator(translator);
}

Q_DECL_EXPORT int main(int argc, char** argv) {
    auto app = SailfishApp::application(argc, argv);
    auto view = SailfishApp::createView();
    auto context = view->rootContext();
    auto engine = view->engine();

    app->setApplicationName(Zimpedia::APP_ID);
    app->setOrganizationName(Zimpedia::ORG);
    app->setApplicationDisplayName(Zimpedia::APP_NAME);
    app->setApplicationVersion(Zimpedia::APP_VERSION);

    registerTypes();

    context->setContextProperty(QStringLiteral("APP_NAME"), Zimpedia::APP_NAME);
    context->setContextProperty(QStringLiteral("APP_ID"), Zimpedia::APP_ID);
    context->setContextProperty(QStringLiteral("APP_VERSION"),
                                Zimpedia::APP_VERSION);
    context->setContextProperty(QStringLiteral("COPYRIGHT_YEAR"),
                                Zimpedia::COPYRIGHT_YEAR);
    context->setContextProperty(QStringLiteral("AUTHOR"), Zimpedia::AUTHOR);
    context->setContextProperty(QStringLiteral("SUPPORT_EMAIL"),
                                Zimpedia::SUPPORT_EMAIL);
    context->setContextProperty(QStringLiteral("PAGE"), Zimpedia::PAGE);
    context->setContextProperty(QStringLiteral("LICENSE"), Zimpedia::LICENSE);
    context->setContextProperty(QStringLiteral("LICENSE_URL"),
                                Zimpedia::LICENSE_URL);

    installTranslator(app);

    auto settings = Settings::instance();
    auto fileModel = FileModel::instance();
    auto bookmarkModel = BookmarkModel::instance();
    auto articleModel = ArticleModel::instance();
    auto zimServer = ZimServer::instance();
    Utils utils;

    engine->addImageProvider(QStringLiteral("icons"), new IconProvider{});

    context->setContextProperty(QStringLiteral("settings"), settings);
    context->setContextProperty(QStringLiteral("zimServer"), zimServer);
    context->setContextProperty(QStringLiteral("fileModel"), fileModel);
    context->setContextProperty(QStringLiteral("articleModel"), articleModel);
    context->setContextProperty(QStringLiteral("utils"), &utils);
    context->setContextProperty(QStringLiteral("bookmarkModel"), bookmarkModel);

    fileModel->updateModel();

    QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(),
                     SLOT(quit()));

    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/main.qml")));
    view->show();
    return app->exec();
}
