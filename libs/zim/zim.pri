ZIM_ROOT = $$PWD
INCLUDEPATH += $${ZIM_ROOT}/include
PKGCONFIG += zlib

LIBS += -L$${ZIM_ROOT}/build/$${ARCH} -l:libzim.so.8 -l:libxapian.so.30 \
        -l:libzstd.so.1 -l:libuuid.so.1 -l:libicudata.so.68 \
        -l:libicui18n.so.68 -l:libicuuc.so.68 -l:liblzma.so.5

lib.files = $${ZIM_ROOT}/build/$${ARCH}/*
lib.path = /usr/share/$${TARGET}/lib
INSTALLS += lib
