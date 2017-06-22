INCLUDEPATH += ./qjson
DEPENDPATH  += ./qjson

HEADERS += \
    json_parser.hh \
    json_scanner.h \
    location.hh \
    parser_p.h \
    parser.h \
    parserrunnable.h \
    position.hh \
    qjson_debug.h \   
    qjson_export.h \    
    qobjecthelper.h \
    serializer.h \
    serializerrunnable.h \
    stack.hh

SOURCES += \
    json_parser.cc \
    json_scanner.cpp \
    json_scanner.cc \
    parser.cpp \
    parserrunnable.cpp \
    qobjecthelper.cpp \
    serializer.cpp \
    serializerrunnable.cpp

OTHER_FILES += json_parser.yy \
               json_scanner.yy
