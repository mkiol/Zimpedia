/* Copyright (C) 2017 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ZIMMETADATAREADER_H
#define ZIMMETADATAREADER_H

#include <QObject>
#include <QString>

#include "filemodel.h"

class ZimMetaDataReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString name READ getName NOTIFY dataChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY dataChanged)
    Q_PROPERTY(QString creator READ getCreator NOTIFY dataChanged)
    Q_PROPERTY(QString date READ getDate NOTIFY dataChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY dataChanged)
    Q_PROPERTY(QString language READ getLanguage NOTIFY dataChanged)
    Q_PROPERTY(QString filename READ getFilename NOTIFY dataChanged)
    // Q_PROPERTY (QString source READ getSource NOTIFY dataChanged)
    Q_PROPERTY(QString publisher READ getPublisher NOTIFY dataChanged)
    Q_PROPERTY(QString tags READ getTags NOTIFY dataChanged)
    Q_PROPERTY(bool ftindex READ getFtindex NOTIFY dataChanged)
    // Q_PROPERTY (QString checksum READ getChecksum NOTIFY dataChanged)
    // Q_PROPERTY (QString version READ getVersion NOTIFY dataChanged)
    Q_PROPERTY(int articleCount READ getArticleCount NOTIFY dataChanged)
    Q_PROPERTY(qint64 size READ getSize NOTIFY dataChanged)
   public:
    explicit ZimMetaDataReader(QObject *parent = 0);

   signals:
    void dataChanged();
    void pathChanged();

   public slots:
    QString getPath() const;
    void setPath(const QString &path);
    QString getName() const;
    QString getTitle() const;
    QString getCreator() const;
    QString getDate() const;
    QString getDescription() const;
    QString getLanguage() const;
    QString getFilename() const;
    QString getPublisher() const;
    // QString getSource() const;
    QString getTags() const;
    bool getFtindex() const;
    // QString getChecksum() const;
    // QString getVersion() const;
    qint64 getSize() const;
    int getArticleCount() const;

   private:
    ZimMetaData m_data;
};

#endif  // ZIMMETADATAREADER_H
