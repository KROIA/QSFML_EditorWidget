QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG+=BUILD_WITH_EASY_PROFILER

include(../SFML_EditorWidget.pri)

inc = inc
src = src

INCLUDEPATH += $$inc

HEADERS += \
    $$inc/sandbox.h \
    inc/AABBDisplayer.h \
    inc/VectorDisplayer.h

SOURCES += \
    $$src/main.cpp \
    $$src/sandbox.cpp \

FORMS += \
    sandbox.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

