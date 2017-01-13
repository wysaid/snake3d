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
    wyGround.cpp \
    wySceneWindow.cpp \
    wySky.cpp \
    wySnake.cpp \
    wyShaderFunctions.cpp \
    wyPlatform_QT.cpp

HEADERS  += wymainwindow.h \
    wyGLFunctions.h \
    wyGLFunctions.h \
    wyMat.h \
    wyPlatform_QT.h \
    wyShaderFunctions.h \
    wyVec.h \
    wyStages.h \
    wySnake.h \
    wySky.h \
    wyGround.h \
    wyScenewindow.h

FORMS    += wymainwindow.ui


unix {

}

win32 {
#    DEFINES += _USE_CXX11_STATIC_ASSERT_=1
}
