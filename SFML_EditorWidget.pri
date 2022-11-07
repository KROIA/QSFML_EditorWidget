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
#$$inc/canvas \
#$$inc/objects \
#$$inc/components \
#$$inc/utilities

HEADERS += \
    $$inc/inc/SFML_EditorWidget.h \
    $$inc/inc/components/AbstractPressEvent.h \
    $$inc/inc/components/BoxCollider.h \
    $$inc/inc/components/Button.h \
    $$inc/inc/components/KeyPressEvent.h \
    $$inc/inc/components/Line.h \
    $$inc/inc/components/MouseFollower.h \
    $$inc/inc/components/MousePressEvent.h \
    $$inc/inc/components/Text.h \
    $$inc/inc/objects/DefaultEditor.h \
    $$inc/inc/objects/DrawableBoxCollider.h \
    $$inc/inc/objects/LineChart.h \
    $$inc/inc/utilities/VectorOperations.h \
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
    $$src/src/components/AbstractPressEvent.cpp \
    $$src/src/components/BoxCollider.cpp \
    $$src/src/components/Button.cpp \
    $$src/src/components/KeyPressEvent.cpp \
    $$src/src/components/Line.cpp \
    $$src/src/components/MouseFollower.cpp \
    $$src/src/components/MousePressEvent.cpp \
    $$src/src/components/Text.cpp \
    $$src/src/objects/DefaultEditor.cpp \
    $$src/src/objects/DrawableBoxCollider.cpp \
    $$src/src/objects/LineChart.cpp \
    $$src/src/utilities/VectorOperations.cpp \
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


