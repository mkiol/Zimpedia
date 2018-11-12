INCLUDEPATH += $$PROJECTDIR/libs/libzim/include

HEADERS += $$PROJECTDIR/libs/libzim/include/zim/*.h

LIBS += -lz -llzma -lbz2
x86: LIBS += -L$$PROJECTDIR/libs/libzim/build/ -lzim-static-i486
arm: LIBS += -L$$PROJECTDIR/libs/libzim/build/ -lzim-static-armv7hl
