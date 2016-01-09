INCLUDEPATH += ./zimlib/include ./zimlib/symbian
DEPENDPATH  += ./zimlib/include ./zimlib/symbian

HEADERS += \
    ./zimlib/include/zim/article.h \
    ./zimlib/include/zim/articlesearch.h \
    ./zimlib/include/zim/blob.h \
    ./zimlib/include/zim/bunzip2stream.h \
    ./zimlib/include/zim/bzip2.h \
    ./zimlib/include/zim/bzip2stream.h \
    ./zimlib/include/zim/cache.h \
    ./zimlib/include/zim/cluster.h \
    ./zimlib/include/zim/deflatestream.h \
    ./zimlib/include/zim/dirent.h \
    ./zimlib/include/zim/endian.h \
    ./zimlib/include/zim/error.h \
    ./zimlib/include/zim/file.h \
    ./zimlib/include/zim/fileheader.h \
    ./zimlib/include/zim/fileimpl.h \
    ./zimlib/include/zim/fileiterator.h \
    ./zimlib/include/zim/fstream.h \
    ./zimlib/include/zim/indexarticle.h \
    ./zimlib/include/zim/inflatestream.h \
    ./zimlib/include/zim/lzmastream.h \
    ./zimlib/include/zim/noncopyable.h \
    ./zimlib/include/zim/refcounted.h \
    ./zimlib/include/zim/search.h \
    ./zimlib/include/zim/smartptr.h \
    ./zimlib/include/zim/template.h \
    ./zimlib/include/zim/unicode.h \
    ./zimlib/include/zim/unlzmastream.h \
    ./zimlib/include/zim/uuid.h \
    ./zimlib/include/zim/zim.h \
    ./zimlib/include/zim/zintstream.h \
    ./zimlib/symbian/config.h \
    ./zimlib/src/arg.h \
    ./zimlib/src/envvalue.h \
    ./zimlib/src/log.h \
    ./zimlib/src/md5.h \
    ./zimlib/src/md5stream.h \
    ./zimlib/src/ptrstream.h \
    ./zimlib/src/tee.h

SOURCES += \
    ./zimlib/src/article.cpp \
    ./zimlib/src/articlesearch.cpp \
    ./zimlib/src/articlesource.cpp \
    ./zimlib/src/bunzip2stream.cpp \
    ./zimlib/src/bzip2.cpp \
    ./zimlib/src/bzip2stream.cpp \
    ./zimlib/src/cluster.cpp \
    ./zimlib/src/deflatestream.cpp \
    ./zimlib/src/dirent.cpp \
    ./zimlib/src/envvalue.cpp \
    ./zimlib/src/file.cpp \
    ./zimlib/src/fileheader.cpp \
    ./zimlib/src/fileimpl.cpp \
    ./zimlib/src/fstream.cpp \
    ./zimlib/src/indexarticle.cpp \
    ./zimlib/src/inflatestream.cpp \
    ./zimlib/src/lzmastream.cpp \
    ./zimlib/src/md5stream.cpp \
    ./zimlib/src/ptrstream.cpp \
    ./zimlib/src/search.cpp \
    ./zimlib/src/tee.cpp \
    ./zimlib/src/template.cpp \
    ./zimlib/src/unicode.cpp \
    ./zimlib/src/unlzmastream.cpp \
    ./zimlib/src/uuid.cpp \
    ./zimlib/src/zimcreator.cpp \
    ./zimlib/src/zintstream.cpp \
    ./zimlib/src/md5.cc

LIBS += -lz
LIBS += -llzma
LIBS += -lbz2
