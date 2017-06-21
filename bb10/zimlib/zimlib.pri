ZIMLIB_BASE = ./zimlib

INCLUDEPATH += $$ZIMLIB_BASE/include
DEPENDPATH  += $$ZIMLIB_BASE/include

ZLIB_SOURCES += \
        $$ZIMLIB_BASE/src/deflatestream.cpp \
        $$ZIMLIB_BASE/src/inflatestream.cpp

BZIP2_SOURCES += \
        $$ZIMLIB_BASE/src/bunzip2stream.cpp \
        $$ZIMLIB_BASE/src/bzip2.cpp \
        $$ZIMLIB_BASE/src/bzip2stream.cpp

LZMA_SOURCES += \
        $$ZIMLIB_BASE/src/lzmastream.cpp \
        $$ZIMLIB_BASE/src/unlzmastream.cpp

SOURCES += \
        $$ZIMLIB_BASE/src/article.cpp \
        $$ZIMLIB_BASE/src/articlesearch.cpp \
        $$ZIMLIB_BASE/src/articlesource.cpp \
        $$ZIMLIB_BASE/src/cluster.cpp \
        $$ZIMLIB_BASE/src/dirent.cpp \
        $$ZIMLIB_BASE/src/envvalue.cpp \
        $$ZIMLIB_BASE/src/file.cpp \
        $$ZIMLIB_BASE/src/fileheader.cpp \
        $$ZIMLIB_BASE/src/fileimpl.cpp \
        $$ZIMLIB_BASE/src/fstream.cpp \
        $$ZIMLIB_BASE/src/indexarticle.cpp \
        $$ZIMLIB_BASE/src/md5.cc \
        $$ZIMLIB_BASE/src/md5stream.cpp \
        $$ZIMLIB_BASE/src/ptrstream.cpp \
        $$ZIMLIB_BASE/src/search.cpp \
        $$ZIMLIB_BASE/src/tee.cpp \
        $$ZIMLIB_BASE/src/template.cpp \
        $$ZIMLIB_BASE/src/unicode.cpp \
        $$ZIMLIB_BASE/src/uuid.cpp \
        $$ZIMLIB_BASE/src/zimcreator.cpp \
        $$ZIMLIB_BASE/src/zintstream.cpp \
        $$ZLIB_SOURCES \
        $$BZIP2_SOURCES \
        $$LZMA_SOURCES

BASE_HEADERS = \
        $$ZIMLIB_BASE/include/zim/article.h \
        $$ZIMLIB_BASE/include/zim/articlesearch.h \
        $$ZIMLIB_BASE/include/zim/blob.h \
        $$ZIMLIB_BASE/include/zim/cache.h \
        $$ZIMLIB_BASE/include/zim/cluster.h \
        $$ZIMLIB_BASE/include/zim/dirent.h \
        $$ZIMLIB_BASE/include/zim/endian.h \
        $$ZIMLIB_BASE/include/zim/error.h \
        $$ZIMLIB_BASE/include/zim/file.h \
        $$ZIMLIB_BASE/include/zim/fileheader.h \
        $$ZIMLIB_BASE/include/zim/fileimpl.h \
        $$ZIMLIB_BASE/include/zim/fileiterator.h \
        $$ZIMLIB_BASE/include/zim/fstream.h \
        $$ZIMLIB_BASE/include/zim/indexarticle.h \
        $$ZIMLIB_BASE/include/zim/noncopyable.h \
        $$ZIMLIB_BASE/include/zim/search.h \
        $$ZIMLIB_BASE/include/zim/smartptr.h \
        $$ZIMLIB_BASE/include/zim/refcounted.h \
        $$ZIMLIB_BASE/include/zim/template.h \
        $$ZIMLIB_BASE/include/zim/unicode.h \
        $$ZIMLIB_BASE/include/zim/uuid.h \
        $$ZIMLIB_BASE/include/zim/zim.h \
        $$ZIMLIB_BASE/include/zim/zintstream.h \
        $$ZIMLIB_BASE/include/zim/writer/articlesource.h \
        $$ZIMLIB_BASE/include/zim/writer/dirent.h \
        $$ZIMLIB_BASE/include/zim/writer/zimcreator.h

INST_HEADERS = \
        $$ZIMLIB_BASE/include/zim/bunzip2stream.h \
        $$ZIMLIB_BASE/include/zim/bzip2.h \
        $$ZIMLIB_BASE/include/zim/bzip2stream.h \
        $$ZIMLIB_BASE/include/zim/deflatestream.h \
        $$ZIMLIB_BASE/include/zim/inflatestream.h \
        $$ZIMLIB_BASE/include/zim/lzmastream.h \
        $$ZIMLIB_BASE/include/zim/unlzmastream.h

HEADERS += \
    $$BASE_HEADERS \
    $$INST_HEADERS

LIBS += -lz
LIBS += -llzma
LIBS += -lbz2
