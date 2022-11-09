QT += core gui
CONFIG += c++11
#CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

inc = $$PWD/inc
src = $$PWD/src

#CONFIG+=BUILD_WITH_EASY_PROFILER

include(extern/SFML.pri)
include(extern/easy_profiler.pri)

INCLUDEPATH += \
    $$inc \
#$$inc/canvas \
#$$inc/objects \
#$$inc/components \
#$$inc/utilities

HEADERS += \
    $$PWD/inc/QSFML_debugSettings.h \
    $$inc/SFML_EditorWidget.h \
    $$inc/components/AbstractPressEvent.h \
    $$inc/components/BoxCollider.h \
    $$inc/components/Button.h \
    $$inc/components/KeyPressEvent.h \
    $$inc/components/Line.h \
    $$inc/components/MouseFollower.h \
    $$inc/components/MousePressEvent.h \
    $$inc/components/Text.h \
    $$inc/objects/DefaultEditor.h \
    $$inc/objects/DrawableBoxCollider.h \
    $$inc/objects/LineChart.h \
    $$inc/utilities/VectorOperations.h \
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
    $$src/components/AbstractPressEvent.cpp \
    $$src/components/BoxCollider.cpp \
    $$src/components/Button.cpp \
    $$src/components/KeyPressEvent.cpp \
    $$src/components/Line.cpp \
    $$src/components/MouseFollower.cpp \
    $$src/components/MousePressEvent.cpp \
    $$src/components/Text.cpp \
    $$src/objects/DefaultEditor.cpp \
    $$src/objects/DrawableBoxCollider.cpp \
    $$src/objects/LineChart.cpp \
    $$src/utilities/VectorOperations.cpp \
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


