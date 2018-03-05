TARGET = harbour-zimpedia

CONFIG += c++11 sailfishapp dbus json
PKGCONFIG += mlite5

DEFINES += SAILFISH

INCLUDEPATH += /usr/include/c++/6

# QHttpServer
include(qhttpserver/qhttpserver.pri)
# ZimLib
include(zimlib/zimlib.pri)

SOURCES += src/main.cpp \
    src/zimserver.cpp \
    src/articlemodel.cpp \
    src/listmodel.cpp \
    src/settings.cpp \
    src/filemodel.cpp \
    src/utils.cpp \
    src/iconprovider.cpp \
    src/bookmarkmodel.cpp \
    src/filefinder.cpp \
    src/zimmetadatareader.cpp \
    src/bookmarks.cpp
    
HEADERS += \
    src/zimserver.h \
    src/articlemodel.h \
    src/listmodel.h \
    src/settings.h \
    src/filemodel.h \
    src/utils.h \
    src/iconprovider.h \
    src/bookmarkmodel.h \
    src/filefinder.h \
    src/zimmetadata.h \
    src/zimmetadatareader.h \
    src/bookmarks.h

OTHER_FILES += \
    qml/CoverPage.qml \
    qml/AboutPage.qml \
    qml/SettingsPage.qml \
    qml/main.qml \
    qml/Spacer.qml \
    qml/PaddedLabel.qml \
    qml/SearchPage.qml \
    qml/FilesPage.qml \
    qml/Bubble.qml \
    qml/ChangelogPage.qml \
    qml/LogItem.qml \
    qml/WebViewPage.qml \
    qml/IconBar.qml \
    qml/IconBarItem.qml \
    qml/TempBaner.qml \
    qml/ProgressPanel.qml \
    qml/IconSlider.qml \
    qml/Notification.qml \
    qml/Icon.qml \
    qml/IconPlaceholder.qml \
    qml/ZimInfoPage.qml \
    qml/tools.js \
    qml/AttValue.qml \
    qml/BookmarksPage.qml \
    qml/BookmarkEditPage.qml \
    translations/*.ts \
    rpm/harbour-zimpedia.changes.in \
    rpm/harbour-zimpedia.spec \
    rpm/harbour-zimpedia.yaml \
    harbour-zimpedia.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 150x150 256x256

TRANSLATIONS += translations/Zimpedia_en.ts \
                translations/Zimpedia_pl.ts \
                translations/Zimpedia_sv.ts \
                translations/Zimpedia_cs.ts \
                translations/Zimpedia_es.ts \
                translations/Zimpedia_de.ts \
                translations/Zimpedia_fr.ts \
                translations/Zimpedia_zh_TW.ts

translations.files = translations
translations.path = /usr/share/$${TARGET}
res.files = res
res.path = /usr/share/$${TARGET}
images.files = images/*
images.path = /usr/share/$${TARGET}/images
INSTALLS += translations res images

DISTFILES += \
    qml/BookmarkFileChoose.qml
