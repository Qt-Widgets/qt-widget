TEMPLATE = app

TRANSLATIONS    +=  lang_zh.ts

include (../shared.pri)
include(modbusMaster/master.pri)
include(modbusSlave/slave.pri)
include(ChartShow/ChartShow.pri)
include(diagramscene/diagramscene.pri)



QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\" /NODEFAULTLIB:"libcmt.lib"

VERSION =2.0.0.0

# 0x0004 mean zh
# for detail https://msdn.microsoft.com/en-us/library/dd318693%28vs.85%29.aspx
RC_LANG = 0x0409
RC_ICONS += inno.ico
RC_ICONS += file.ico

QMAKE_TARGET_COMPANY = LASER
QMAKE_TARGET_PRODUCT = Laser Cutting System
QMAKE_TARGET_DESCRIPTION = HLaser
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2018 Hongjianli

TARGET = HLaser



requires(qtConfig(combobox))

HEADERS += \
    Src/Gui/settings/deviceWidget/hdelegatecombbox.h \
    Src/Gui/settings/deviceWidget/hdevicewidget.h \
    Src/Gui/settings/deviceWidget/hwsidgetdelegate.h \
    Src/Gui/settings/deviceWidget/tablemodel.h \
    Src/Gui/settings/hdialogparameterlibrary.h \
    Src/Gui/settings/hdialogsystemadjust.h \
    Src/Gui/settings/hparalibcontroller.h \
    Src/Gui/settings/hsystemadjustcontroller.h \
    Src/Gui/tools/hfindfocusdialog.h \
    Src/Motion/IO/hiodevice.h \
    Src/Motion/IO/hmodbus.h \
    Src/Motion/camera/baslercamera.h \
    Src/Motion/camera/havtcamera.h \
    Src/Motion/camera/hcamera.h \
    Src/Motion/camera/hkvisioncamera.h \
    Src/Motion/hcameracontroller.h \
    Src/Motion/hfinddevice.h \
    Src/Motion/hiodevicecontroller.h \
    Src/Motion/hscannercontroller.h \
    Src/Motion/motionStage/A3200Api.h \
    Src/Motion/motionStage/GtMotion.h \
    Src/Motion/motionStage/MotionCompiler.h \
    Src/Motion/motionStage/hmotionstage.h \
    Src/Motion/scanner/rtc4func.h \
    Src/Motion/scanner/hscanner.h \
    Src/colorswatch.h \
    Src/data/hconfigwidget.h \
    Src/data/hdialogproperty.h \
    Src/data/hsettings.h \
    Src/main-menu/custompositiondialog.h \
    Src/mainwindow.h \
    Src/painter/GraphicsItems/constantIems/hgraphicsbackground.h \
    Src/painter/GraphicsItems/constantIems/hgraphicsdrawcacheitem.h \
    Src/painter/GraphicsItems/constantIems/hgraphicsmouse.h \
    Src/painter/GraphicsItems/constantIems/hgraphicsselectrectitem.h \
    Src/painter/GraphicsItems/constantIems/hgraphicsvisionshow.h \
    Src/painter/GraphicsItems/editableItems/hgraphicsarcitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicsellipseitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicsitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicsitemgroup.h \
    Src/painter/GraphicsItems/editableItems/hgraphicslayer.h \
    Src/painter/GraphicsItems/editableItems/hgraphicslineitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicspathitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicspixmapitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicspolygonitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicsrectitem.h \
    Src/painter/GraphicsItems/editableItems/hgraphicssimpletextitem.h \
    Src/painter/hcommands.h \
    Src/painter/hevent.h \
    Src/painter/hgraphicsscene.h \
    Src/painter/hgraphicsview.h \
    Src/painter/math/RArc.h \
    Src/painter/math/RBox.h \
    Src/painter/math/RCircle.h \
    Src/painter/math/RColor.h \
    Src/painter/math/RDirected.h \
    Src/painter/math/RDxfImporter.h \
    Src/painter/math/RDxfServices.h \
    Src/painter/math/REllipse.h \
    Src/painter/math/REllipseProxy.h \
    Src/painter/math/RExplodable.h \
    Src/painter/math/RLine.h \
    Src/painter/math/RLinetypePattern.h \
    Src/painter/math/RLineweight.h \
    Src/painter/math/RMath.h \
    Src/painter/math/RMatrix.h \
    Src/painter/math/RPainterPath.h \
    Src/painter/math/RPoint.h \
    Src/painter/math/RPolyline.h \
    Src/painter/math/RPolylineProxy.h \
    Src/painter/math/RRay.h \
    Src/painter/math/RRefPoint.h \
    Src/painter/math/RS.h \
    Src/painter/math/RShape.h \
    Src/painter/math/RShapeProxy.h \
    Src/painter/math/RSpline.h \
    Src/painter/math/RSplineProxy.h \
    Src/painter/math/RTextLabel.h \
    Src/painter/math/RTriangle.h \
    Src/painter/math/RVector.h \
    Src/painter/math/RXLine.h \
    Src/painter/math/core_global.h \
    Src/painter/math/rcolorcodes.h \
    Src/toolBar/hmaintoolbarwid.h \
    Src/toolbar.h \
    Src/Editor/codeedit.h \
    Src/User/dialog_newuser.h \
    Src/User/dialog_supercheck.h \
    Src/User/dialogaccountui.h \
    Src/User/myaes.h \
    Src/Gui/itemlist.h \
    Src/dxflib/dl_attributes.h \
    Src/dxflib/dl_codes.h \
    Src/dxflib/dl_creationadapter.h \
    Src/dxflib/dl_creationinterface.h \
    Src/dxflib/dl_dxf.h \
    Src/dxflib/dl_entities.h \
    Src/dxflib/dl_exception.h \
    Src/dxflib/dl_extrusion.h \
    Src/dxflib/dl_global.h \
    Src/dxflib/dl_writer.h \
    Src/dxflib/dl_writer_ascii.h \
    Src/dxflib/m_dxfio.h \
    Src/window/delegate.h \
    Src/window/dialoglayerlopts.h \
    Src/window/editdata.h \
    Src/window/editorwidget.h \
    Src/window/programdata.h \
    Src/window/treeitem.h \
    Src/window/treemodel.h \
    Src/window/htreeheaderview.h \
    Src/window/programhandler.h \
    Src/window/itemlistmenu.h \
    Src/main-menu/hmainmenubar.h \
    Src/painter/paintermainwidget.h \
    Src/data/hsqlcommon.h \
    Src/Gui/hmotionui.h \
    Src/painter/hpaintertoolbar.h \
    Src/Gui/h_processwidget.h \
    Src/Camera/h_camerashow.h \
    Src/Gui/h_cameracontrollerwidget.h \
    Src/User/h_authority.h \
    Src/h_globalapplication.h \
    Src/Gui/htreeview/h_treeview.h \
    Src/Motion/h_motioncontroller.h \
    Src/data/inisettings.h \
    Src/data/hcommondata.h \
    Src/data/mythreadworker.h \
    Src/device/hserialport.h \
    Src/Gui/tools/hstartextapp.h \
    interface/HAxisAdjustInterface.h \
    interface/himginterface.h \
    interface/hinterfacecontroller.h


SOURCES += \
    Src/Gui/settings/deviceWidget/hdelegatecombbox.cpp \
    Src/Gui/settings/deviceWidget/hdevicewidget.cpp \
    Src/Gui/settings/deviceWidget/hwsidgetdelegate.cpp \
    Src/Gui/settings/deviceWidget/tablemodel.cpp \
    Src/Gui/settings/hdialogparameterlibrary.cpp \
    Src/Gui/settings/hdialogsystemadjust.cpp \
    Src/Gui/settings/hparalibcontroller.cpp \
    Src/Gui/settings/hsystemadjustcontroller.cpp \
    Src/Gui/tools/hfindfocusdialog.cpp \
    Src/Motion/IO/hiodevice.cpp \
    Src/Motion/IO/hmodbus.cpp \
    Src/Motion/camera/baslercamera.cpp \
    Src/Motion/camera/havtcamera.cpp \
    Src/Motion/camera/hcamera.cpp \
    Src/Motion/camera/hkvisioncamera.cpp \
    Src/Motion/hcameracontroller.cpp \
    Src/Motion/hfinddevice.cpp \
    Src/Motion/hiodevicecontroller.cpp \
    Src/Motion/hscannercontroller.cpp \
    Src/Motion/motionStage/A3200Api.cpp \
    Src/Motion/motionStage/GtMotion.cpp \
    Src/Motion/motionStage/MotionCompiler.cpp \
    Src/Motion/motionStage/hmotionstage.cpp \
    Src/Motion/scanner/rtc4func.cpp \
    Src/Motion/scanner/hscanner.cpp \
    Src/colorswatch.cpp \
    Src/data/hconfigwidget.cpp \
    Src/data/hdialogproperty.cpp \
    Src/data/hsettings.cpp \
    Src/main-menu/custompositiondialog.cpp \
    Src/main.cpp \
    Src/mainwindow.cpp \
    Src/painter/GraphicsItems/constantIems/hgraphicsbackground.cpp \
    Src/painter/GraphicsItems/constantIems/hgraphicsdrawcacheitem.cpp \
    Src/painter/GraphicsItems/constantIems/hgraphicsmouse.cpp \
    Src/painter/GraphicsItems/constantIems/hgraphicsselectrectitem.cpp \
    Src/painter/GraphicsItems/constantIems/hgraphicsvisionshow.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicsarcitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicsellipseitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicsitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicsitemgroup.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicslayer.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicslineitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicspathitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicspixmapitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicspolygonitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicsrectitem.cpp \
    Src/painter/GraphicsItems/editableItems/hgraphicssimpletextitem.cpp \
    Src/painter/hcommands.cpp \
    Src/painter/hevent.cpp \
    Src/painter/hgraphicsscene.cpp \
    Src/painter/hgraphicsview.cpp \
    Src/painter/math/RArc.cpp \
    Src/painter/math/RBox.cpp \
    Src/painter/math/RCircle.cpp \
    Src/painter/math/RColor.cpp \
    Src/painter/math/RDirected.cpp \
    Src/painter/math/RDxfImporter.cpp \
    Src/painter/math/RDxfServices.cpp \
    Src/painter/math/REllipse.cpp \
    Src/painter/math/RLine.cpp \
    Src/painter/math/RLinetypePattern.cpp \
    Src/painter/math/RLineweight.cpp \
    Src/painter/math/RMath.cpp \
    Src/painter/math/RMatrix.cpp \
    Src/painter/math/RPainterPath.cpp \
    Src/painter/math/RPoint.cpp \
    Src/painter/math/RPolyline.cpp \
    Src/painter/math/RRay.cpp \
    Src/painter/math/RRefPoint.cpp \
    Src/painter/math/RS.cpp \
    Src/painter/math/RShape.cpp \
    Src/painter/math/RSpline.cpp \
    Src/painter/math/RTextLabel.cpp \
    Src/painter/math/RTriangle.cpp \
    Src/painter/math/RVector.cpp \
    Src/painter/math/RXLine.cpp \
    Src/toolBar/hmaintoolbarwid.cpp \
    Src/toolbar.cpp \
    Src/Editor/codeedit.cpp \
    Src/User/dialog_newuser.cpp \
    Src/User/dialog_supercheck.cpp \
    Src/User/dialogaccountui.cpp \
    Src/User/myaes.cpp \
    Src/Gui/itemlist.cpp \
    Src/dxflib/dl_dxf.cpp \
    Src/dxflib/dl_writer_ascii.cpp \
    Src/dxflib/m_dxfio.cpp \
    Src/window/delegate.cpp \
    Src/window/dialoglayerlopts.cpp \
    Src/window/editdata.cpp \
    Src/window/editorwidget.cpp \
    Src/window/programdata.cpp \
    Src/window/treeitem.cpp \
    Src/window/treemodel.cpp \
    Src/window/htreeheaderview.cpp \
    Src/window/programhandler.cpp \
    Src/window/itemlistmenu.cpp \
    Src/main-menu/hmainmenubar.cpp \
    Src/painter/paintermainwidget.cpp \
    Src/data/hsqlcommon.cpp \
    Src/Gui/hmotionui.cpp \
    Src/painter/hpaintertoolbar.cpp \
    Src/Gui/h_processwidget.cpp \
    Src/Camera/h_camerashow.cpp \
    Src/Gui/h_cameracontrollerwidget.cpp \
    Src/User/h_authority.cpp \
    Src/h_globalapplication.cpp \
    Src/Gui/htreeview/h_treeview.cpp \
    Src/Motion/h_motioncontroller.cpp \
    Src/data/inisettings.cpp \
    Src/data/hcommondata.cpp \
    Src/data/mythreadworker.cpp \
    Src/device/hserialport.cpp \
    Src/Gui/tools/hstartextapp.cpp \
    interface/hinterfacecontroller.cpp

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

RESOURCES += \
    icons.qrc \
    Mainwindow.qrc





####################### Halcon18 ###################################

#INCLUDEPATH += $$PWD/Halcon18/include\
#               $$PWD/Halcon18/include/halconcpp\
#               $$PWD/Halcon18/include/hlib\
#               $$PWD/Halcon18/include/hdevengine

#LIBS+="$$PWD/Halcon18/lib/x64-win64/halcon.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcpp.lib"

#      "$$PWD/Halcon18/lib/x64-win64/halconc.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcppxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconcxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/halconxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/hdevenginecpp.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/hdevenginecppxl.lib"\
#      "$$PWD/Halcon18/lib/x64-win64/libiomp5md.lib"\


####################################################################

#############################GSL####################################
#INCLUDEPATH += $$PWD/GSL/include\

#LIBS+="$$PWD/GSL/lib/gsl-2_5.lib"


####################################################################


###############################################
win32 {
    LIBS += -lRpcrt4 -lAdvapi32
    LIBS += -luser32 -lgdi32
}

CONFIG(debug, debug|release) {
LIBS += "$$PWD/opennurbs/debug/opennurbs.lib"\
        "$$PWD/opennurbs/debug/zlib.lib"\
}
else {
LIBS += "$$PWD/opennurbs/release/opennurbs.lib"\
        "$$PWD/opennurbs/release/zlib.lib"\
}


##############################################

# install
target.path = $SolutionDir
INSTALLS += target


DISTFILES +=

FORMS += \
    Src/Gui/settings/hdialogparameterlibrary.ui \
    Src/Gui/settings/hdialogsystemadjust.ui \
    Src/Gui/tools/hfindfocusdialog.ui \
    Src/data/hdialogproperty.ui \
    Src/main-menu/custompositiondialog.ui \
    Src/toolBar/hmaintoolbarwid.ui \
    Src/window/dialoglayerlopts.ui \
    UIforms/dialog_newuser.ui \
    UIforms/dialog_supercheck.ui \
    UIforms/dialogaccountui.ui \
    Src/Gui/itemlist.ui


#################################################for release########################################
#    CONFIG(debug, debug|release) {
#      SRC_PATH=$$shadowed($$PWD)/debug/$${TARGET}.exe
#      EXTRA_BINFILES += $$PWD/OpenCv/opencv4.0.1/x64/Debug/dll
#    }
#    else{
#      SRC_PATH=$$shadowed($$PWD)/release/$${TARGET}.exe
#      EXTRA_BINFILES += $$PWD/OpenCv/opencv4.0.1/x64/Release/dll
#    }

#    BIN_PATH=$$shadowed($$PWD)/bin/
#    BIN_PATH ~= s,/,\\,g
#    SRC_PATH ~= s,/,\\,g

#    QMAKE_POST_LINK +=$$quote(cmd /c xcopy $${SRC_PATH} $${BIN_PATH} /y $$escape_expand(\n\t))



#    EXTRA_BINFILES += \
#    $$PWD/Camera/basler/Runtime/x64 \
#    $$PWD/A3200/Bin64 \
#    $$PWD/GTS/64bit/single_card/Bin \
#    $$PWD/Halcon18/bin \
#    $$PWD/gsl/bin \

#    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#    EXTRA_BINFILES_WIN ~= s,/,\\,g

#    for(FILE,EXTRA_BINFILES_WIN){
#    QMAKE_POST_LINK +=$$quote(cmd /c xcopy $${FILE}  $${BIN_PATH} /E /Y$$escape_expand(\n\t))
#    #QMAKE_POST_LINK +=$$quote(cmd /c robocopy $${FILE}  $${BIN_PATH} /E /MT:32$$escape_expand(\n\t))
#    }

#    DEPLOY_COMMAND = $$replace(QMAKE_QMAKE,qmake.exe,windeployqt.exe)
#    DEPLOY_COMMAND ~= s,/,\\,g

#    QMAKE_POST_LINK += $$quote($${DEPLOY_COMMAND}  $${BIN_PATH}$${TARGET}.exe$$escape_expand(\n\t))
####################################################################################################




#PROJECT_DIRECTORY = $$PWD
#COMMON_DIRECTORY = $$PWD
#THIRDPARTY_PATH = $$PWD

#CONFIG(debug, debug|release) {

#    contains(QMAKE_COMPILER, gcc) {

#        if($$ARCHITECTURE_64_BIT) {

#            DESTDIR = $${COMMON_DIRECTORY}/build/debug-64bit-gcc

#            TARGET = ($$TARGET)-debug-64bit-gcc

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-debug-64bit-gcc

#             #    EXTRA_BINFILES += \
#             #        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstrtp.so \
#             #        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstvideo4linux2.so
#             #    for(FILE,EXTRA_BINFILES){
#             #        QMAKE_POST_LINK += $$quote(cp $${FILE} $${DESTDIR}$$escape_expand(\n\t))
#             #    }

#        } else {

#            DESTDIR = $${COMMON_DIRECTORY}/build/debug-32bit-gcc

#            TARGET = $${TARGET}-debug-32bit-gcc

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-debug-32bit-gcc
#             #    EXTRA_BINFILES += \
#             #        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstrtp.so \
#             #        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstvideo4linux2.so
#             #    for(FILE,EXTRA_BINFILES){
#             #        QMAKE_POST_LINK += $$quote(cp $${FILE} $${DESTDIR}$$escape_expand(\n\t))
#             #    }

#        }

#    }



#    contains(QMAKE_COMPILER, msvc) {

#        contains(QMAKE_TARGET.arch,x86_64) {

#            DESTDIR = $${COMMON_DIRECTORY}/build/debug-64bit-msvc

#            TARGET = $${TARGET}-debug-64bit-msvc

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-debug-64bit-msvc

#            EXTRA_BINFILES += \
#            $${THIRDPARTY_PATH}/Camera/basler/Runtime/x64 \
#            $${THIRDPARTY_PATH}/OpenCv/x64/Debug/dll \
#            $${THIRDPARTY_PATH}/A3200/Bin/x64 \
#            $${THIRDPARTY_PATH}/GTS/x64/Bin \

#            EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#            EXTRA_BINFILES_WIN ~= s,/,\\,g
#            DESTDIR_WIN = $${DESTDIR}
#            DESTDIR_WIN ~= s,/,\\,g
#            for(FILE,EXTRA_BINFILES_WIN){

#                QMAKE_POST_LINK +=$$quote(robocopy $${FILE}  $${DESTDIR_WIN} /E /MT:8$$escape_expand(\n\t))
#                #QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#               }

#        } else {

#            DESTDIR = $${COMMON_DIRECTORY}/build/debug-32bit-msvc

#            TARGET = $${TARGET}-debug-32bit-msvc

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-debug-32bit-msvc

#            EXTRA_BINFILES += \
#            $${THIRDPARTY_PATH}/Camera/basler/Runtime/Win32 \
#            $${THIRDPARTY_PATH}/OpenCv/x86/Debug/dll \
#            $${THIRDPARTY_PATH}/A3200/Bin/x86 \
#            $${THIRDPARTY_PATH}/GTS/x86/Bin \

#            EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#            EXTRA_BINFILES_WIN ~= s,/,\\,g
#            DESTDIR_WIN = $${DESTDIR}
#            DESTDIR_WIN ~= s,/,\\,g
#            for(FILE,EXTRA_BINFILES_WIN){
#                QMAKE_POST_LINK +=$$quote(robocopy $${FILE}  $${DESTDIR_WIN} /E /MT:8$$escape_expand(\n\t))
#                #QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#               }

#        }

#    }



#} else {


#    contains(QMAKE_COMPILER, gcc) {

#        if($$ARCHITECTURE_64_BIT) {

#            DESTDIR = $${COMMON_DIRECTORY}/build/release-64bit-gcc

#            TARGET = $${TARGET}

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-release-64bit-gcc

#        } else {

#            DESTDIR = $${COMMON_DIRECTORY}/build/release-32bit-gcc

#            TARGET = $${TARGET}-32bit

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-release-32bit-gcc

#        }

#    }



#    contains(QMAKE_COMPILER, msvc) {

#         contains(QMAKE_TARGET.arch,x86_64) {

#            DESTDIR = $${COMMON_DIRECTORY}/build/release-64bit-msvc

#            TARGET = $${TARGET}

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-release-64bit-msvc


#            EXTRA_BINFILES += \
#            $${THIRDPARTY_PATH}/Camera/basler/Runtime/x64 \
#            $${THIRDPARTY_PATH}/OpenCv/x64/Release/dll \
#            $${THIRDPARTY_PATH}/A3200/Bin/x64 \
#            $${THIRDPARTY_PATH}/GTS/x64/Bin \

#            EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#            EXTRA_BINFILES_WIN ~= s,/,\\,g
#            DESTDIR_WIN = $${DESTDIR}
#            DESTDIR_WIN ~= s,/,\\,g
#            for(FILE,EXTRA_BINFILES_WIN){
#                QMAKE_POST_LINK +=$$quote(robocopy $${FILE}  $${DESTDIR_WIN} /E /MT:8 $$escape_expand(\n\t))
#                #QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#               }

#        } else {

#            DESTDIR = $${COMMON_DIRECTORY}/build/release-32bit-msvc

#            TARGET = $${TARGET}-32bit

#            OBJECTS_DIR = $${PROJECT_DIRECTORY}/generated/obj-release-32bit-msvc

#            EXTRA_BINFILES += \
#            $${THIRDPARTY_PATH}/Camera/basler/Runtime/Win32 \
#            $${THIRDPARTY_PATH}/OpenCv/x86/Release/dll \
#            $${THIRDPARTY_PATH}/A3200/Bin/x86 \
#            $${THIRDPARTY_PATH}/GTS/x86/Bin \

#            EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#            EXTRA_BINFILES_WIN ~= s,/,\\,g
#            DESTDIR_WIN = $${DESTDIR}
#            DESTDIR_WIN ~= s,/,\\,g
#            for(FILE,EXTRA_BINFILES_WIN){
#                QMAKE_POST_LINK +=$$quote(robocopy $${FILE}  $${DESTDIR_WIN} /E /MT:8$$escape_expand(\n\t))
#                #QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#               }


#        }

#    }

#    DEFINES += NDEBUG

#}



#S = $${DIR_SEPARATOR}



#D = $$DESTDIR

#D = $$replace(D, $$escape_expand(\\), $$S)

#D = $$replace(D, /, $$S)

#E = $$escape_expand(\n\t)


#win32 {

#    QT += winextras



#    DEPLOY_COMMAND = $$replace($$QMAKE,qmake.exe,windeployqt)

#    DEPLOY_TARGET = $$shell_quote($$shell_path($${D}/$${TARGET}.exe))

#    QMAKE_POST_LINK += $${DEPLOY_COMMAND} --no-translations $${DEPLOY_TARGET} $${E}



#     contains(QMAKE_TARGET.arch,x86_64) {

#        message("x86_64 build")



#    } else {

#        message("x86 build")

#        contains(QMAKE_COMPILER, gcc) {

#                QMAKE_LFLAGS += -Wl,--large-address-aware

#        }

#        contains(QMAKE_COMPILER, msvc) {

#                QMAKE_LFLAGS += /LARGEADDRESSAWARE

#        }

#    }

#}
