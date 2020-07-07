#! [0]

include (../shared.pri)

TEMPLATE        = lib
#QT += core \
#    widgets\
#    gui
CONFIG         += plugin
INCLUDEPATH    += ../mainwindow/interface
TARGET          = imgplugin

CONFIG(debug, debug|release) {
    DESTDIR         = ../mainwindow/debug/plugins
}else{
    DESTDIR         = ../mainwindow/release/plugins
}

#! [0]

PLUGIN_FILES = plugin.json

## install
#target.path = ../mainwindow/
#INSTALLS += target

#CONFIG += install_ok  # Do not cargo-cult this!



HEADERS += \
    dialogcamerasetting.h \
    h_halcon_widget.h \
    hdialogproperty.h \
    himgplugin.h \
    himgprocessmwindow.h

SOURCES += \
    dialogcamerasetting.cpp \
    h_halcon_widget.cpp \
    hdialogproperty.cpp \
    himgplugin.cpp \
    himgprocessmwindow.cpp

FORMS += \
    dialogcamerasetting.ui \
    hdialogproperty.ui \
    himgprocessmwindow.ui

INCLUDEPATH += $$PWD/Halcon18/includeRename\
               $$PWD/Halcon18/includeRename/halconcpp\
               $$PWD/Halcon18/includeRename/hlib\
               $$PWD/Halcon18/includeRename/hdevengine

#LIBS+="$$PWD/Halcon18/lib/x64-win64/halcon.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcpp.lib"

#      "$$PWD/Halcon18/lib/x64-win64/halconc.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcppxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/hdevenginecpp.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/hdevenginecppxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/libiomp5md.lib"


LIBS+="$$PWD/Halcon18/lib/rename32/halcon.lib"\
      "$$PWD/Halcon18/lib/rename32/halconcpp.lib"
