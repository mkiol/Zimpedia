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
#include <QStringList>
#include <QUrl>

class Settings : public QSettings {
    Q_OBJECT

    Q_PROPERTY(
        int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QStringList zimFiles READ zimFiles WRITE setZimFiles NOTIFY
                   zimFilesChanged)
    Q_PROPERTY(int browser READ browser WRITE setBrowser NOTIFY browserChanged)
    Q_PROPERTY(SearchMode searchMode READ searchMode WRITE setSearchMode NOTIFY
                   searchModeChanged)
   public:
    enum SearchMode {
        TitleSearch = 0,
        FullTextSearch = 1,
    };
    Q_ENUM(SearchMode)

    static Settings* instance();

    void setFontSize(int value);
    int fontSize() const;
    void setZimFiles(QStringList value);
    void addZimFile(const QString& value);
    void removeZimFile(const QString& value);
    void setZoom(float value);
    float zoom() const;
    Q_INVOKABLE QString zoomViewport() const;
    Q_INVOKABLE QString zoomFontSize() const;
    void setBrowser(int value);
    int browser() const;
    void setSearchMode(SearchMode value);
    SearchMode searchMode() const;
    QStringList zimFiles() const;  // uuids
    Q_INVOKABLE QUrl appIcon() const;

   signals:
    void portChanged();
    void fontSizeChanged();
    void zimFilesChanged();
    void browserChanged();
    void zoomChanged();
    void searchModeChanged();

   private:
    inline static const QString settingsFilename =
        QStringLiteral("settings.conf");
    static constexpr const float maxZoom = 2.0;
    static constexpr const float minZoom = 0.5;
    static Settings* m_instance;
    Settings();
    static QString settingsFilepath();
};

#endif  // SETTINGS_H
