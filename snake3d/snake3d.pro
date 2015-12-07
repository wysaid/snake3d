#-------------------------------------------------
#
# Project created by wysaid 2014-06-05
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake3d
TEMPLATE = app

#INCLUDEPATH += "."

DEFINES += _DEBUG

RESOURCES += wymainwindow.qrc

SOURCES += main.cpp\
    wymainwindow.cpp \
    WYGround.cpp\
    htPlatform_QT.cpp \
    htShaderFunctions.cpp \
    WYSky.cpp \
    WYSceneWindow.cpp \
    WYSnake.cpp

HEADERS  += wymainwindow.h \
    WYScenewindow.h \
    htPlatform_QT.h \
    htGLFunctions.h \
    WYGround.h \
    htMat.h\
    htVec.h\
    htStaticAssert.h\
    WYStages.h\
    htShaderFunctions.h \
    WYSky.h\
    WYSnake.h

FORMS    += wymainwindow.ui


unix {

}

win32 {
#    DEFINES += _USE_CXX11_STATIC_ASSERT_=1
}
