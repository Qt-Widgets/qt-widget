INCLUDEPATH += $$PWD

QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    +=  $$PWD/hflowviewmainwindow.h \
                $$PWD/diagramitem.h \
                $$PWD/diagramscene.h \
                $$PWD/arrow.h \
                $$PWD/diagramtextitem.h

SOURCES	    +=  $$PWD/hflowviewmainwindow.cpp \
                $$PWD/diagramitem.cpp \
                $$PWD/arrow.cpp \
                $$PWD/diagramtextitem.cpp \
                $$PWD/diagramscene.cpp

RESOURCES   +=	$$PWD/diagramscene.qrc






