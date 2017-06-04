/*
  Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>

  This file is part of Zimpedia application.

  This Source Code Form is subject to the terms of
  the Mozilla Public License, v.2.0. If a copy of
  the MPL was not distributed with this file, You can
  obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef ZIMMETADATAREADER_H
#define ZIMMETADATAREADER_H

#include <QObject>
#include <QString>

#include "zimmetadata.h"

class ZimMetaDataReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY (QString name READ getName NOTIFY dataChanged)
    Q_PROPERTY (QString title READ getTitle NOTIFY dataChanged)
    Q_PROPERTY (QString creator READ getCreator NOTIFY dataChanged)
    Q_PROPERTY (QString date READ getDate NOTIFY dataChanged)
    Q_PROPERTY (QString description READ getDescription NOTIFY dataChanged)
    Q_PROPERTY (QString language READ getLanguage NOTIFY dataChanged)
    Q_PROPERTY (QString filename READ getFilename NOTIFY dataChanged)
    Q_PROPERTY (QString source READ getSource NOTIFY dataChanged)
    Q_PROPERTY (QString publisher READ getPublisher NOTIFY dataChanged)
    Q_PROPERTY (QString tags READ getTags NOTIFY dataChanged)
    Q_PROPERTY (QString checksum READ getChecksum NOTIFY dataChanged)
    //Q_PROPERTY (QString version READ getVersion NOTIFY dataChanged)
    Q_PROPERTY (int articleCount READ getArticleCount NOTIFY dataChanged)
    Q_PROPERTY (qint64 size READ getSize NOTIFY dataChanged)
public:
    explicit ZimMetaDataReader(QObject *parent = 0);

signals:
    void dataChanged();
    void pathChanged();

public slots:
    QString getPath();
    void setPath(const QString &path);
    QString getName();
    QString getTitle();
    QString getCreator();
    QString getDate();
    QString getDescription();
    QString getLanguage();
    QString getFilename();
    QString getPublisher();
    QString getSource();
    QString getTags();
    QString getChecksum();
    //QString getVersion();
    qint64 getSize();
    int getArticleCount();

private:
    ZimMetaData m_data;
};

#endif // ZIMMETADATAREADER_H
