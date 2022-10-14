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
    $$inc/objects \
    $$inc/components \
    $$inc/utilities

HEADERS += \
    $$PWD/inc/components/BoxCollider.h \
    $$PWD/inc/objects/DrawableBoxCollider.h \
    $$inc/canvas/Canvas.h \
    $$inc/canvas/CanvasSettings.h \
    $$inc/canvas/CanvasObjectContainer.h \
    $$inc/canvas/CanvasForwardDeclaration.h \
    $$inc/components/DrawableVector.h \
    $$inc/components/Component.h \
    $$inc/components/Drawable.h \
    $$inc/components/SfEventHandle.h \
    $$inc/objects/CanvasObject.h \
    $$inc/objects/BackgroundGrid.h \
    $$inc/objects/CameraController.h \
    $$inc/objects/VisibleCamera.h \
    $$inc/utilities/AABB.h






SOURCES += \
    $$PWD/src/components/BoxCollider.cpp \
    $$PWD/src/objects/DrawableBoxCollider.cpp \
    $$src/canvas/Canvas.cpp \
    $$src/canvas/CanvasObjectContainer.cpp \
    $$src/components/DrawableVector.cpp \
    $$src/components/SfEventHandle.cpp \
    $$src/components/Component.cpp \
    $$src/components/Drawable.cpp \
    $$src/objects/CanvasObject.cpp \
    $$src/objects/BackgroundGrid.cpp \
    $$src/objects/CameraController.cpp \
    $$src/utilities/AABB.cpp



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


