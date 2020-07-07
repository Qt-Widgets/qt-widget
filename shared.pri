
QT += core \
    widgets\
    gui \
    sql \
    concurrent\
    svg \
    opengl \
    network \
    printsupport

include(common/common.pri)

#win32-msvc2010 {
#    CONFIG+=win32-msvc
#}

#win32-msvc2015 {
#    contains(QMAKE_TARGET.arch, x86_64) {
#        # fix wrong library path under MSVC 2015 64bit:
#        LIBS += -L"C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x64"
#    }
#}

win32-msvc2017 {
    contains(QMAKE_TARGET.arch, x86_64) {
        # fix wrong library path under MSVC 2017 64bit:
        LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\winv6.3\km\x64"
    }
}






#QMAKE_CXXFLAGS +=  -Wno-unused-parameter

qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl) {
        DEFINES += QT_OPENGL_SUPPORT
        QT += opengl
        LIBS+=-lopengl32 -lglu32
}

CONFIG += c++11\
          qt\
          windows\
          thread


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += WIN32

contains( CONFIG, thread ) {
　　　　DEFINES += QT_THREAD_SUPPORT
}




#CONFIG(release，debug|release) {
##QMAKE_CXXFLAGS_RELEASE += -O        # Release -O
##QMAKE_CXXFLAGS_RELEASE += -O1       # Release -O1
##QMAKE_CXXFLAGS_RELEASE += -O2       # Release -O2
#QMAKE_CXXFLAGS_RELEASE  += -O3        # Release -O3
#}



#QMAKE_CXXFLAGS_RELEASE = -O2 -MD -GL

#QMAKE_CXXFLAGS_DEBUG = -Zi -MDd


#contains(QMAKE_COMPILER, msvc) {
#    QMAKE_CXXFLAGS += /MP /GL /O2 /Oi
#}


QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG



LIBS+=-lDbgHelp
LIBS +=User32.LIB


CONFIG(debug, debug|release) {
    !win32 {
        QMAKE_LFLAGS += -rdynamic
    }
}

INCLUDEPATH += $$PWD/OpenCv/opencv4.1.1/opencv/build/include\
               $$PWD/Camera/basler/include\
               $$PWD/Camera/hkvision/Includes\
               $$PWD/Camera/Vimba_2.1/VimbaC/Include\
               $$PWD/Camera/Vimba_2.1/VimbaCPP/Include\
               $$PWD/Camera/Vimba_2.1/VimbaImageTransform/Include\
               $$PWD/Camera/AVT/include\
               $$PWD/A3200/Include\
               $$PWD/GTS/64bit/single_card/Include\
               $$PWD/GTS/test\
               $$PWD/RTC4/Include\
               $$PWD/3rdParty\


CONFIG(debug, debug|release) {
      LIBS+="$$PWD/OpenCv/opencv4.1.1/win32/lib/opencv_world411d.lib"
}
else {
      LIBS+="$$PWD/OpenCv/opencv4.1.1/win32/lib/opencv_world411.lib"
}


# 64bit
#LIBS+="$$PWD/GTS/64bit/single_card/Lib/gts.lib"\
#      "$$PWD/A3200/Lib64/A3200C64.lib"\
#      "$$PWD/Camera/basler/lib/x64/GCBase_MD_VC141_v3_1_Basler_pylon_v5_1.lib"\
#      "$$PWD/Camera/basler/lib/x64/GenApi_MD_VC141_v3_1_Basler_pylon_v5_1.lib"\
#      "$$PWD/Camera/basler/lib/x64/PylonBase_v5_1.lib"\
#      "$$PWD/Camera/basler/lib/x64/PylonUtility_v5_1.lib"\
#      "$$PWD/Camera/hkvision/Libraries/win64/MvCameraControl.lib"\
#      "$$PWD/RTC4/lib/RTC4DLLx64.lib"\

LIBS+="$$PWD/GTS/32bit/single_card/Lib/gts.lib"\
      "$$PWD/GTS/test/GucoiDll.lib"\
      "$$PWD/A3200/Lib/A3200C.lib"\
      "$$PWD/Camera/basler/lib/Win32/GCBase_MD_VC141_v3_1_Basler_pylon_v5_1.lib"\
      "$$PWD/Camera/basler/lib/Win32/GenApi_MD_VC141_v3_1_Basler_pylon_v5_1.lib"\
      "$$PWD/Camera/basler/lib/Win32/PylonBase_v5_1.lib"\
      "$$PWD/Camera/basler/lib/Win32/PylonUtility_v5_1.lib"\
      "$$PWD/Camera/hkvision/Libraries/win32/MvCameraControl.lib"\
      "$$PWD/Camera/Vimba_2.1/VimbaC/Lib/Win32/VimbaC.lib"\
      "$$PWD/Camera/Vimba_2.1/VimbaCPP/Lib/Win32/VimbaCPP.lib"\
      "$$PWD/Camera/Vimba_2.1/VimbaImageTransform/Lib/Win32/VimbaImageTransform.lib"\
      "$$PWD/Camera/AVT/lib/FGCamera.lib"\
      "$$PWD/RTC4/lib/RTC4DLL.lib"\

