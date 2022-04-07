ZIM_ROOT = $$PWD
INCLUDEPATH += $${ZIM_ROOT}/include
PKGCONFIG += zlib liblzma

LIBS += -L$${ZIM_ROOT}/build/$${ARCH} -l:libzim.so.7 -l:libxapian.so.30 \
        -l:libzstd.so.1 -l:libuuid.so.1 -l:libicudata.so.66 \
        -l:libicui18n.so.66 -l:libicuuc.so.66

lib.files = $${ZIM_ROOT}/build/$${ARCH}/*
lib.path = /usr/share/$${TARGET}/lib
INSTALLS += lib
