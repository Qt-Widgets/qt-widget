INCLUDEPATH += $$PWD
QT += serialbus serialport widgets
requires(qtConfig(combobox))

#TARGET = modbusmaster
#TEMPLATE = app
CONFIG += c++11

SOURCES +=\
       $$PWD/settingsdialog.cpp \
       $$PWD/writeregistermodel.cpp \
       $$PWD/hmodbusmainwindow.cpp

HEADERS  += \
        $$PWD/settingsdialog.h \
        $$PWD/writeregistermodel.h \
    $$PWD/hmodbusmainwindow.h

FORMS    += \
        $$PWD/settingsdialog.ui \
    $$PWD/hmodbusmainwindow.ui

RESOURCES += \
    $$PWD/master.qrc

#target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
#INSTALLS += target
