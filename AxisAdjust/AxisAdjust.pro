include (../shared.pri)

TEMPLATE        = lib
#QT += core \
#    widgets\
#    gui
CONFIG         += plugin
INCLUDEPATH    += ../mainwindow/interface
TARGET          = axisAdjustPlugin

CONFIG(debug, debug|release) {
    DESTDIR         = ../mainwindow/debug/plugins
}else{
    DESTDIR         = ../mainwindow/release/plugins
}

PLUGIN_FILES = plugin.json

HEADERS += \
    dialogexport.h \
    haxisadjustmainwindow.h \
    haxisadjustplugin.h

SOURCES += \
    dialogexport.cpp \
    haxisadjustmainwindow.cpp \
    haxisadjustplugin.cpp

FORMS += \
    dialogexport.ui \
    haxisadjustmainwindow.ui
