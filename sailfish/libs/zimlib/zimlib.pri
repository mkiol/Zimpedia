INCLUDEPATH += $$PROJECTDIR/libs/zimlib/include

HEADERS += $$PROJECTDIR/libs/zimlib/include/zim/*.h

LIBS += -lz -llzma -lbz2
x86: LIBS += -L$$PROJECTDIR/libs/zimlib/build/ -l:zimlib-static-i486.a
arm: LIBS += -L$$PROJECTDIR/libs/zimlib/build/ -l:zimlib-static-armv7hl.a
