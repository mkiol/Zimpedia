TARGET = harbour-zimpedia

CONFIG += sailfishapp

DEFINES += SAILFISH

# QHttpServer
include(qhttpserver/qhttpserver.pri)
# ZimLib
include(zimlib/zimlib.pri)

SOURCES += src/main.cpp \
    src/zimserver.cpp \
    src/articlemodel.cpp \
    src/listmodel.cpp \
    src/settings.cpp \
    src/filemodel.cpp
    
HEADERS += \
    src/zimserver.h \
    src/articlemodel.h \
    src/listmodel.h \
    src/settings.h \
    src/filemodel.h

OTHER_FILES += \
    qml/CoverPage.qml \
    qml/AboutPage.qml \
    qml/SettingsPage.qml \
    translations/*.ts \
    harbour-zimpedia.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

#CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-zimpedia-en.ts \
                translations/harbour-zimpedia-pl.ts \
                translations/harbour-zimpedia-sv.ts \
                translations/harbour-zimpedia-cs_CZ.ts \
                translations/harbour-zimpedia-es.ts \
                translations/harbour-zimpedia-de.ts

translations.files = translations
translations.path = /usr/share/$${TARGET}
res.files = res
res.path = /usr/share/$${TARGET}
INSTALLS += translations res

DISTFILES += \
    qml/main.qml \
    qml/Spacer.qml \
    qml/PaddedLabel.qml \
    qml/SearchPage.qml \
    qml/FilesPage.qml \
    qml/PageMenu.qml \
    rpm/harbour-zimpedia.changes.in \
    rpm/harbour-zimpedia.spec \
    rpm/harbour-zimpedia.yaml \
    qml/Bubble.qml

PKGCONFIG += zlib
