#-------------------------------------------------
#
# Project created by QtCreator 2019-04-09T06:52:21
#
#-------------------------------------------------
INCLUDEPATH += $$PWD
QT       += core gui charts serialport sql
requires(qtConfig(tableview))
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets




# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    $$PWD/Gui/customtablemodel.cpp \
    $$PWD/Gui/tablewidget.cpp \
    $$PWD/SerialPort/hchartshowmasterthread.cpp \
    $$PWD/SerialPort/hserialdialog.cpp \
    $$PWD/Gui/hsamplewidget.cpp \
    $$PWD/Src/hdatacollect.cpp \
    $$PWD/Gui/hdatacollectsetting.cpp \
    $$PWD/hchartshowmainwindow.cpp

HEADERS += \
    $$PWD/Gui/customtablemodel.h \
    $$PWD/Gui/tablewidget.h \
    $$PWD/SerialPort/hchartshowmasterthread.h \
    $$PWD/SerialPort/hserialdialog.h \
    $$PWD/Gui/hsamplewidget.h \
    $$PWD/Src/hdatacollect.h \
    $$PWD/Gui/hdatacollectsetting.h \
    $$PWD/hchartshowmainwindow.h

FORMS += \
        $$PWD/mainwindow.ui



RESOURCES += \
    $$PWD/res.qrc


