INCLUDEPATH += $$PROJECTDIR/libs/libzim/include

LIBS += -lz -llzma -lbz2 -lrt -licuuc -licudata
x86 {
    LIBS += -L$$PROJECTDIR/libs/libzim/build/i486/ -l:libzim.so.4
    LIBS += -L$$PROJECTDIR/libs/libzim/build/i486/ -l:libxapian.so.30
}
arm {
    LIBS += -L$$PROJECTDIR/libs/libzim/build/armv7hl/ -l:libzim.so.4
    LIBS += -L$$PROJECTDIR/libs/libzim/build/armv7hl/ -l:libxapian.so.30
}

x86: lib.files = $$PROJECTDIR/libs/libzim/build/i486/*
arm: lib.files = $$PROJECTDIR/libs/libzim/build/armv7hl/*
lib.path = /usr/share/$${TARGET}/lib
INSTALLS += lib

HEADERS += $$PROJECTDIR/libs/libzim/include/zim/*.h

OTHER_FILES += \
    $$PROJECTDIR/libs/libzim/src/*.cpp \
    $$PROJECTDIR/libs/libzim/src/*.h \
    $$PROJECTDIR/libs/libzim/src/writer/*.h \
    $$PROJECTDIR/libs/libzim/src/writer/*.cpp \
    $$PROJECTDIR/libs/libzim/include/zim/*.h
