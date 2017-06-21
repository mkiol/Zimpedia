APP_NAME = Zimpedia

CONFIG += qt warn_on cascades10
QT += network 
LIBS += -lbbplatform -lbbsystem -lbbdevice -lbb

DEFINES += BB10

include(config.pri)
include(qjson/json.pri)
include(qhttpserver/qhttpserver.pri)
include(zimlib/zimlib.pri)
