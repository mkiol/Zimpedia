TARGET = harbour-zimpedia
CONFIG += c++1z sailfishapp dbus json
PKGCONFIG += mlite5

contains(QT_ARCH, i386){
   CONFIG += x86
   DEFINES += X86
   ARCH = x86
} else {
   contains(QT_ARCH, x86_64) {
       CONFIG += amd64
       DEFINES += X86
       ARCH = amd64
   } else {
       contains(QT_ARCH, arm){
           CONFIG += arm
           DEFINES += ARM
           ARCH = arm
       } else {
            contains(QT_ARCH, arm64) {
                CONFIG += arm64
                DEFINES += ARM
                ARCH = arm64
            }
       }
   }
}

ROOT_DIR = $${PWD}
SRC_DIR = $${ROOT_DIR}/src
LIB_DIR = $${ROOT_DIR}/libs

CONFIG += sailfish
DEFINES += SAILFISH

include($${LIB_DIR}/libzim/libzim.pri)
include($${LIB_DIR}/qhttpserver/qhttpserver.pri)

SOURCES += \
    $${SRC_DIR}/main.cpp \
    $${SRC_DIR}/zimserver.cpp \
    $${SRC_DIR}/articlemodel.cpp \
    $${SRC_DIR}/listmodel.cpp \
    $${SRC_DIR}/settings.cpp \
    $${SRC_DIR}/filemodel.cpp \
    $${SRC_DIR}/utils.cpp \
    $${SRC_DIR}/bookmarkmodel.cpp \
    $${SRC_DIR}/zimmetadatareader.cpp \
    $${SRC_DIR}/itemmodel.cpp \
    $${SRC_DIR}/iconprovider.cpp

HEADERS += \
    $${SRC_DIR}/zimserver.h \
    $${SRC_DIR}/articlemodel.h \
    $${SRC_DIR}/listmodel.h \
    $${SRC_DIR}/settings.h \
    $${SRC_DIR}/filemodel.h \
    $${SRC_DIR}/utils.h \
    $${SRC_DIR}/bookmarkmodel.h \
    $${SRC_DIR}/zimmetadatareader.h \
    $${SRC_DIR}/info.h \
    $${SRC_DIR}/itemmodel.h \
    $${SRC_DIR}/iconprovider.h

DISTFILES += \
    qml/*.qml \
    rpm/*.*

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

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

DEPENDPATH += $${INCLUDEPATH}
