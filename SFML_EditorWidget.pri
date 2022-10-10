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
    $$inc/canvas/components

HEADERS += \
    $$inc/canvas/Canvas.h \
    $$inc/canvas/CanvasSettings.h \
    $$inc/canvas/CanvasObjectContainer.h \
    $$inc/canvas/CanvasForwardDeclaration.h \
    $$inc/canvas/components/DrawableVector.h \
    $$inc/canvas/components/Component.h \
    $$inc/canvas/components/Drawable.h \
    $$inc/canvas/components/SfEventHandle.h \
    $$inc/canvas/objects/CanvasObject.h \
    $$inc/canvas/objects/BackgroundGrid.h \
    $$inc/canvas/objects/CameraController.h \
    $$inc/canvas/objects/VisibleCamera.h






SOURCES += \
    $$src/canvas/Canvas.cpp \
    $$src/canvas/CanvasObjectContainer.cpp \
    $$src/canvas/components/DrawableVector.cpp \
    $$src/canvas/components/SfEventHandle.cpp \
    $$src/canvas/components/Component.cpp \
    $$src/canvas/components/Drawable.cpp \
    $$src/canvas/objects/CanvasObject.cpp \
    $$src/canvas/objects/BackgroundGrid.cpp \
    $$src/canvas/objects/CameraController.cpp



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


