CORE_DIR = core

INCLUDEPATH += $$CORE_DIR

SOURCES += \
    $$CORE_DIR/main.cpp \
    $$CORE_DIR/zimserver.cpp \
    $$CORE_DIR/articlemodel.cpp \
    $$CORE_DIR/listmodel.cpp \
    $$CORE_DIR/settings.cpp \
    $$CORE_DIR/filemodel.cpp \
    $$CORE_DIR/utils.cpp \
    $$CORE_DIR/bookmarkmodel.cpp \
#    $$CORE_DIR/filefinder.cpp \
    $$CORE_DIR/zimmetadatareader.cpp \
#    $$CORE_DIR/bookmarks.cpp \
    $$CORE_DIR/itemmodel.cpp

HEADERS += \
    $$CORE_DIR/zimserver.h \
    $$CORE_DIR/articlemodel.h \
    $$CORE_DIR/listmodel.h \
    $$CORE_DIR/settings.h \
    $$CORE_DIR/filemodel.h \
    $$CORE_DIR/utils.h \
    $$CORE_DIR/bookmarkmodel.h \
#    $$CORE_DIR/filefinder.h \
#    $$CORE_DIR/zimmetadata.h \
    $$CORE_DIR/zimmetadatareader.h \
#    $$CORE_DIR/bookmarks.h \
    $$CORE_DIR/info.h \
    $$CORE_DIR/itemmodel.h

sailfish {
    HEADERS += \
        $$CORE_DIR/iconprovider.h

    SOURCES += \
        $$CORE_DIR/iconprovider.cpp
}
