INCLUDEPATH += ./qhttpserver
DEPENDPATH  += ./qhttpserver

HEADERS += \
    ./qhttpserver/http_parser.h \
    ./qhttpserver/qhttpconnection.h \
    ./qhttpserver/qhttprequest.h \
    ./qhttpserver/qhttpresponse.h \
    ./qhttpserver/qhttpserver.h \
    ./qhttpserver/qhttpserverapi.h \
    ./qhttpserver/qhttpserverfwd.h

SOURCES += \
    ./qhttpserver/http_parser.c \
    ./qhttpserver/qhttpconnection.cpp \
    ./qhttpserver/qhttprequest.cpp \
    ./qhttpserver/qhttpresponse.cpp \
    ./qhttpserver/qhttpserver.cpp

