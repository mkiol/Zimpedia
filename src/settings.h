/* Copyright (C) 2016-2022 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>

class Settings : public QSettings {
    Q_OBJECT

    Q_PROPERTY(
        int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(float zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(
        QString zimFile READ getZimFile WRITE setZimFile NOTIFY zimFileChanged)
    Q_PROPERTY(
        int browser READ getBrowser WRITE setBrowser NOTIFY browserChanged)
    Q_PROPERTY(SearchMode searchMode READ getSearchMode WRITE setSearchMode
                   NOTIFY searchModeChanged)

   public:
    enum SearchMode {
        TitleSearch = 0,
        FullTextSearch = 1,
    };
    Q_ENUM(SearchMode)

    static Settings* instance();

    void setFontSize(int value);
    int getFontSize() const;
    void setZimFile(const QString& value);
    void setZoom(float value);
    float getZoom() const;
    void setBrowser(int value);
    int getBrowser() const;
    void setSearchMode(SearchMode value);
    SearchMode getSearchMode() const;
    QString getZimFile() const;

   signals:
    void portChanged();
    void fontSizeChanged();
    void zimFileChanged();
    void browserChanged();
    void zoomChanged();
    void searchModeChanged();

   private:
    inline static const QString settingsFilename =
        QStringLiteral("settings.conf");
    static Settings* m_instance;
    Settings();
    static QString settingsFilepath();
};

#endif  // SETTINGS_H
