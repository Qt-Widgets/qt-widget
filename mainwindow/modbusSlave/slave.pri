INCLUDEPATH += $$PWD/
QT += serialbus serialport widgets
requires(qtConfig(combobox))

#TARGET = modbusslave
#TEMPLATE = app
CONFIG += c++11

SOURCES +=\
    $$PWD/hmodbusslavemainwindow.cpp \
    $$PWD/settingsdialogslave.cpp

HEADERS  +=\
    $$PWD/hmodbusslavemainwindow.h \
    $$PWD/settingsdialogslave.h

FORMS    += \
    $$PWD/hmodbusslavemainwindow.ui \
    $$PWD/settingsdialogslave.ui

RESOURCES += $$PWD/slave.qrc

#target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/slave
#INSTALLS += target
