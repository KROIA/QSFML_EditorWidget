QT += core gui
CONFIG += c++11
#CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

inc = $$PWD/inc
src = $$PWD/src

include(extern/SFML.pri)

INCLUDEPATH += \
    $$inc \
    $$inc/canvas \
    $$inc/camera \
    $$inc/canvas/objects

HEADERS += \
    $$PWD/inc/camera/CameraInterface.h \
    $$PWD/inc/canvas/CanvasForwardDeclaration.h \
    $$PWD/inc/canvas/DrawInterface.h \
    $$PWD/inc/canvas/objects/BackgroundGrid.h \
    $$PWD/inc/canvas/objects/CanvasObject.h \
    $$PWD/inc/canvas/CanvasObjectContainer.h \
    $$PWD/inc/canvas/objects/Drawable.h \
    $$PWD/inc/canvas/objects/VisibleCamera.h \
    $$inc/canvas/CanvasSettings.h \
    $$inc/canvas/QSFML_Canvas.h \
    $$inc/camera/CameraController.h

SOURCES += \
    $$PWD/src/canvas/objects/CanvasObject.cpp \
    $$PWD/src/canvas/CanvasObjectContainer.cpp \
    $$PWD/src/canvas/objects/Drawable.cpp \
    $$src/canvas/QSFML_Canvas.cpp \
    $$src/camera/CameraController.cpp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


