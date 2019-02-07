TARGET = harbour-zimpedia

CONFIG += c++11 sailfishapp dbus json libzim

PKGCONFIG += mlite5

linux-g++-32: CONFIG += x86
linux-g++: CONFIG += arm

DEFINES += SAILFISH

PROJECTDIR = $$PWD

INCLUDEPATH += /usr/include/c++/7

CONFIG += sailfish
DEFINES += SAILFISH

libzim {
    DEFINES += LIBZIM
    include($$PROJECTDIR/libs/libzim/libzim.pri)
}
zimlib {
    DEFINES += ZIMLIB
    include($$PROJECTDIR/libs/zimlib/zimlib.pri)
}

include($$PROJECTDIR/libs/qhttpserver/qhttpserver.pri)
include(core/zimpedia_core.pri)

DISTFILES += \
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
    qml/BookmarkFileChoose.qml

OTHER_FILES += \
    translations/*.ts \
    rpm/$${TARGET}.yaml \
    rpm/$${TARGET}.changes.in \
    rpm/$${TARGET}.spec

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

TRANSLATION_SOURCE_DIRS += $$PROJECTDIR/core
CONFIG += sailfishapp_i18n_include_obsolete
TRANSLATIONS += translations/Zimpedia_en.ts \
                translations/Zimpedia_pl.ts \
                translations/Zimpedia_sv.ts \
                translations/Zimpedia_cs.ts \
                translations/Zimpedia_es.ts \
                translations/Zimpedia_de.ts \
                translations/Zimpedia_fr.ts \
                translations/Zimpedia_nl.ts \
                translations/Zimpedia_hu.ts \
                translations/Zimpedia_nl_BE.ts \
                translations/Zimpedia_zh_TW.ts \
                translations/Zimpedia_zh_CN.ts \
                translations/Zimpedia_ru.ts
include(sailfishapp_i18n.pri)

images.files = images/*
images.path = /usr/share/$${TARGET}/images
INSTALLS += images

res.files = res
res.path = /usr/share/$${TARGET}
INSTALLS += res

DEPENDPATH += $$INCLUDEPATH
