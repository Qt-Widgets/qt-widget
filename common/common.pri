

INCLUDEPATH += $$PWD




PRECOMPILED_HEADER = $$PWD/stable.h

qtHaveModule(opengl)|qtConfig(opengles2)  {
    DEFINES += QT_OPENGL_SUPPORT
    QT += opengl widgets
}

SOURCES += \
    $$PWD/customTreeWid/htreeitemdata.cpp \
    $$PWD/customTreeWid/htreeitemdelegate.cpp \
    $$PWD/customTreeWid/htreemodel.cpp \
    $$PWD/customTreeWid/htreeview.cpp \
    $$PWD/h2leveltreewid.cpp \
    $$PWD/hinputbox.cpp \
    $$PWD/hlistwidget.cpp \
    $$PWD/hopenglwidget.cpp \
    $$PWD/hproperty_para.cpp \
    $$PWD/htableview.cpp \
    $$PWD/htableviewdelegate.cpp \
    $$PWD/htableviewmodel.cpp \
    $$PWD/hvariantlist.cpp \
    $$PWD/hlineedit.cpp

HEADERS += \
    $$PWD/customTreeWid/htreeitemdata.h \
    $$PWD/customTreeWid/htreeitemdelegate.h \
    $$PWD/customTreeWid/htreemodel.h \
    $$PWD/customTreeWid/htreeview.h \
    $$PWD/h2leveltreewid.h \
    $$PWD/hinputbox.h \
    $$PWD/hlistwidget.h \
    $$PWD/hmath.h \
    $$PWD/hopenglwidget.h \
    $$PWD/hcommon.h \
    $$PWD/hmathfunctions.h \
    $$PWD/hproperty_para.h \
    $$PWD/htableview.h \
    $$PWD/htableviewdelegate.h \
    $$PWD/htableviewmodel.h \
    $$PWD/hvariantlist.h \
    $$PWD/hlineedit.h

