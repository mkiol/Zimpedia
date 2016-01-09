APP_NAME = Zimpedia

CONFIG += qt warn_on cascades10
QT += network 
LIBS += -lbbplatform -lbbsystem -lbbdevice

DEFINES += BB10

include(config.pri)

include(qhttpserver/qhttpserver.pri)
include(zimlib/zimlib.pri)
