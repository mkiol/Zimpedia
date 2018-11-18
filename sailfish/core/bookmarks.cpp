/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifdef BB10
#include <QtGui/QDesktopServices>
#include <QVariantList>
#include <QVariantMap>
#include <QVariant>
#include "qjson/serializer.h"
#include "qjson/parser.h"
#endif
#ifdef SAILFISH
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#endif

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QByteArray>
#include <algorithm>

#include "bookmarks.h"
#include "filefinder.h"
#include "zimmetadata.h"
#include "settings.h"

const QString Bookmarks::bookmarkFilename = "bookmarks.json";

Bookmarks* Bookmarks::inst = 0;

Bookmarks::Bookmarks(QObject *parent) : QObject(parent), busy(false)
{
    QObject::connect(FileFinder::instance(), SIGNAL(busyChanged()),
            this, SLOT(finderBusyHandler()));
}

void Bookmarks::init()
{
    FileFinder::instance()->init();
}

void Bookmarks::finderBusyHandler()
{
    FileFinder* f = qobject_cast<FileFinder*>(sender());
    if (f->getBusy() != this->busy) {
        this->busy = f->getBusy();
        emit busyChanged();
    }
}

bool Bookmarks::getBusy()
{
    return busy;
}

Bookmarks* Bookmarks::instance()
{
    if (Bookmarks::inst == 0) {
        Bookmarks::inst = new Bookmarks();
    }

    return Bookmarks::inst;
}

#ifdef BB10
QVariantList Bookmarks::readBookmarks()
{
    QString filepath = QDesktopServices::storageLocation(
            QDesktopServices::DataLocation) + "/" + Bookmarks::bookmarkFilename;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can not open" << filepath << "file!";
        return QVariantList();
    }

    QJson::Parser parser;
    bool ok;
    QByteArray data = file.readAll();
    QVariant doc = parser.parse(data, &ok);
    if (!ok) {
        qWarning() << "Can not parse bookmarks file!";
        file.close();
        return QVariantList();
    }

    if (doc.type() != QVariant::List) {
        qWarning() << "Bookmarks file is not JSON array!";
        file.close();
        return QVariantList();
    }

    file.close();
    return doc.toList();
}

bool Bookmarks::writeBookmarks(const QVariantList &json)
{
    QString filepath = QDesktopServices::storageLocation(
            QDesktopServices::DataLocation) + "/" + Bookmarks::bookmarkFilename;
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Can not open" << filepath << "file!";
        return false;
    }

    QJson::Serializer serializer;
    serializer.setIndentMode(QJson::IndentFull);
    bool ok;
    serializer.serialize(json, &file, &ok);
    if (!ok) {
      qWarning() << "Can not serialize JSON:" << serializer.errorMessage();
      file.close();
      return false;
    }
    file.close();
    return true;
}
#elif SAILFISH
QJsonArray Bookmarks::readBookmarks()
{
    QString filepath = QStandardPaths::writableLocation(
                QStandardPaths::DataLocation) + "/" + Bookmarks::bookmarkFilename;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can not open" << filepath << "file!";
        return QJsonArray();
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Can not parse bookmarks file!" << parseError.errorString();
        file.close();
        return QJsonArray();
    }

    if (!doc.isArray()) {
        qWarning() << "Bookmarks file is not JSON array!";
        file.close();
        return QJsonArray();
    }

    file.close();
    return doc.array();
}

bool Bookmarks::writeBookmarks(const QJsonArray &json)
{
    QString filepath = QStandardPaths::writableLocation(
                QStandardPaths::DataLocation) + "/" + Bookmarks::bookmarkFilename;
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Can not open" << filepath << "file!";
        return false;
    }

    QTextStream outStream(&file);
    QJsonDocument doc(json);
    outStream.setCodec("UTF-8");
    outStream.setGenerateByteOrderMark(true);
    outStream << doc.toJson();

    file.close();
    return true;
}
#endif

QList<Bookmarks::Bookmark> Bookmarks::getBookmarks()
{
    FileFinder* f = FileFinder::instance();
    QList<Bookmarks::Bookmark> list;

#ifdef BB10
    QVariantList jsonlist = readBookmarks();
    QVariantList::iterator end = jsonlist.end();
    for (QVariantList::iterator it = jsonlist.begin(); it != end; ++it) {
        QVariantMap obj = (*it).toMap();
#elif SAILFISH
    QJsonArray jsonlist = readBookmarks();
    for (auto i : jsonlist) {
        QJsonObject obj = i.toObject();
#endif
        QString url = obj["url"].toString();
        QStringList parts = url.split("/");
        if (parts.length() < 6) {
            qWarning() << "Url is invalid!";
            continue;
        }

        QString zimuuid = parts.at(3);
        if (zimuuid.startsWith("uuid:")) {
            zimuuid = zimuuid.right(zimuuid.length()-5);
        } else {
            qWarning() << "Url UUID is invalid or missing!";
            continue;
        }

        ZimMetaData metaData = f->files.value(zimuuid);

        Bookmark b;
        b.title = obj["title"].toString();
        b.url = url;

        if (metaData.isEmpty()) {
#ifdef BB10
            b.favicon = obj.contains("favicon") && obj["favicon"].type() == QVariant::String ? obj["favicon"].toString() : "";
#elif SAILFISH
            b.favicon = obj.contains("favicon") ? obj["favicon"].toString() : "";
#endif
            b.valid = false;
        } else {
#ifdef BB10
            b.favicon = obj.contains("favicon") && obj["favicon"].type() == QVariant::String ? obj["favicon"].toString() : metaData.favicon;
#elif SAILFISH
            b.favicon = obj.contains("favicon") ? obj["favicon"].toString() : metaData.favicon;
#endif
            b.zimTitle = metaData.title;
            b.zimLanguage = metaData.language;
            b.zimUuid = zimuuid;
            b.valid = true;
        }

        list.append(b);
    }

    std::sort(list.begin(), list.end());

    return list;
}

void Bookmarks::addBookmark(const QString &title,
                            const QString &url,
                            const QString &favicon)
{
#ifdef BB10
    //qDebug() << "addBookmark";
    //qDebug() << "title:" << title;
    //qDebug() << "url:" << url;
    QVariantList array = readBookmarks();
    QVariantList::iterator end = array.end();
    for (QVariantList::iterator it = array.begin(); it != end; ++it) {
        QVariantMap obj = (*it).toMap();
        if (obj.contains("url") && obj["url"].type() == QVariant::String && obj["url"].toString() == url) {
#elif SAILFISH
    QJsonArray array = readBookmarks();
    for (auto b : array) {
        auto obj = b.toObject();
        if (obj.contains("url") && obj.value("url").toString() == url) {
#endif
            qWarning() << "Bookmark url already exists!";
            emit bookmarkExists();
            return;
        }
    }

#ifdef BB10
    QVariantMap obj;
#elif SAILFISH
    QJsonObject obj;
#endif
    obj.insert("title", title);
    obj.insert("url", url);
    obj.insert("favicon", favicon);
    array.append(obj);

    if (writeBookmarks(array)) {
        emit bookmarkAdded();
    }
}

void Bookmarks::updateBookmark(const QString &oldUrl,
                               const QString &title,
                               const QString &url,
                               const QString &favicon)
{
#ifdef BB10
    QVariantList array = readBookmarks();
    QVariantList::iterator it = array.begin();
    QVariantList::iterator end = array.end();
    while (it != end) {
        QVariantMap obj = (*it).toMap();
        if (obj.contains("url") && obj["url"].type() == QVariant::String && obj["url"].toString() == oldUrl) {
            array.erase(it);
            QVariantMap nobj;
#elif SAILFISH
    QJsonArray array = readBookmarks();
    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") && obj.value("url").toString() == oldUrl) {
            array.erase(it);
            QJsonObject nobj;
#endif
            nobj.insert("title", title);
            nobj.insert("url", url);
            nobj.insert("favicon", favicon);
            array.insert(it, nobj);

            if (writeBookmarks(array))
                emit bookmarkUpdated();
            return;
        }
        ++it;
    }

    qWarning() << "Bookmark url doesn't exist!";
}

void Bookmarks::deleteBookmark(const QString &url)
{
#ifdef BB10
    QVariantList array = readBookmarks();
    QVariantList::iterator it = array.begin();
    QVariantList::iterator end = array.end();
    while (it != end) {
        QVariantMap obj = (*it).toMap();
        if (obj.contains("url") && obj["url"].type() == QVariant::String && obj["url"].toString() == url) {
#elif SAILFISH
    QJsonArray array = readBookmarks();
    auto it = array.begin();
    auto end = array.end();
    while (it != end) {
        auto obj = (*it).toObject();
        if (obj.contains("url") && obj.value("url").toString() == url) {
#endif
            array.erase(it);
            if (writeBookmarks(array))
                emit bookmarkDeleted();
            return;
        }
        ++it;
    }
}

const QString Bookmarks::changeUuid(const QString &url, const QString &newUuid)
{
    Settings* s = Settings::instance();
    return newUuid.isEmpty() ? url : QString("http://localhost:%1/%2/%3")
                                                .arg(s->getPort())
                                                .arg("uuid:" + newUuid)
                                                .arg(articleUrl(url));
}

bool Bookmarks::validateUrl(const QString &url)
{
    QStringList parts = url.split("/");
    if (parts.length() < 6) {
        qWarning() << "Url is invalid!";
        return false;
    }
    QString uuid = parts.at(3);
    if (uuid.startsWith("uuid:")) {
        return true;
    }
    return false;
}

const QString Bookmarks::articleUrl(const QString &url)
{
    QString newUrl = "";

    QStringList parts = url.split("/");
    if (parts.length() < 2) {
        qWarning() << "Url is invalid!";
    } else {
        newUrl = QStringList(parts.mid(parts.length()-2)).join("/");
    }

    return newUrl;
}
