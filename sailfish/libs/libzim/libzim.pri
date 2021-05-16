LIBZIM_ROOT = $${LIB_DIR}/libzim
INCLUDEPATH += $${LIBZIM_ROOT}/include
PKGCONFIG += zlib liblzma

LIBS += -L$${LIBZIM_ROOT}/build/$${ARCH} -l:libzim.so.6 -l:libxapian.so.30 \
        -l:libzstd.so.1 -l:libuuid.so.1 -l:liblzma.so.5 -l:libicudata.so.66 \
        -l:libicui18n.so.66 -l:libicuuc.so.66

lib.files = $${LIBZIM_ROOT}/build/$${ARCH}/*
lib.path = /usr/share/$${TARGET}/lib
INSTALLS += lib

HEADERS += $${LIBZIM_ROOT}/include/zim/*.h
