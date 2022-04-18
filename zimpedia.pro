TARGET = harbour-zimpedia

CONFIG += c++1z sailfishapp dbus json
PKGCONFIG += mlite5 qt5embedwidget
DEFINES += QT_NO_URL_CAST_FROM_STRING

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

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
TRANSLATIONS_TS_DIR = translations

include($${LIB_DIR}/zim/zim.pri)
include($${LIB_DIR}/qhttpserver/qhttpserver.pri)

OTHER_FILES += \
    qml/*.qml \
    rpm/*.*

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
    $${SRC_DIR}/iconprovider.cpp \
    $${SRC_DIR}/log.cpp

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
    $${SRC_DIR}/iconprovider.h \
    $${SRC_DIR}/log.h \
    $${ZIM_ROOT}/include/zim/*.h


SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

CONFIG += sailfishapp_i18n_include_obsolete
TRANSLATIONS += $${TRANSLATIONS_TS_DIR}/Zimpedia_en.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_pl.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_sv.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_cs.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_es.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_de.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_fr.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_nl.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_hu.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_nl_BE.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_zh_TW.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_zh_CN.ts \
                $${TRANSLATIONS_TS_DIR}/Zimpedia_ru.ts
include(sailfishapp_i18n.pri)

# install

install_images.files = images/*
install_images.path = /usr/share/$${TARGET}/images
INSTALLS += install_images

install_res.files = res
install_res.path = /usr/share/$${TARGET}
INSTALLS += install_res

DEPENDPATH += $${INCLUDEPATH}
