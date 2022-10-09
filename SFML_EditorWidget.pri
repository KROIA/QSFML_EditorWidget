QT += core gui
CONFIG += c++11
#CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

inc = $$PWD/inc
src = $$PWD/src

include(extern/SFML.pri)
include(extern/easy_profiler.pri)

INCLUDEPATH += \
    $$inc \
    $$inc/canvas \
    $$inc/canvas/objects \
    $$inc/canvas/objects/base

HEADERS += \
    $$inc/canvas/CameraInterface.h \
    $$inc/canvas/CanvasForwardDeclaration.h \
    $$inc/canvas/DrawInterface.h \
    $$inc/canvas/objects/BackgroundGrid.h \
    $$inc/canvas/objects/base/CanvasObject.h \
    $$inc/canvas/CanvasObjectContainer.h \
    $$inc/canvas/objects/base/Drawable.h \
    $$inc/canvas/objects/SfEventHandle.h \
    $$inc/canvas/objects/VisibleCamera.h \
    $$inc/canvas/CanvasSettings.h \
    $$inc/canvas/QSFML_Canvas.h \
    $$inc/canvas/objects/base/CameraController.h

SOURCES += \
    $$src/canvas/objects/base/CanvasObject.cpp \
    $$src/canvas/CanvasObjectContainer.cpp \
    $$src/canvas/objects/base/Drawable.cpp \
    $$src/canvas/QSFML_Canvas.cpp \
    $$src/canvas/objects/base/CameraController.cpp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


