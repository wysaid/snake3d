#-------------------------------------------------
#
# Project created by wysaid 2014-06-05
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SDKDemo_QTCreator
TEMPLATE = app

#INCLUDEPATH += "."

#DEFINES += LIBHTGPUSDK4QT_LIB=1 DEBUG

RESOURCES += wymainwindow.qrc

SOURCES += main.cpp\
    wymainwindow.cpp \
    scenewindow.cpp \
    htCommonDefine.cpp \
    htShaderFunctions.cpp \
    htGLFunctions.cpp

HEADERS  += wymainwindow.h \
    scenewindow.h \
    htPlatform_QT.h \
    htCommonDefine.h \
    htGLFunctions.h \
    htShaderFunctions.h \
    htMat.h \
    htVec.h \
    htStaticAssert.h \

FORMS    += wymainwindow.ui


unix {

}

win32 {
    DEFINES += _USE_CXX11_STATIC_ASSERT_=1
}
