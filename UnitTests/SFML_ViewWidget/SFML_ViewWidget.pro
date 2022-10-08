QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

include(../../SFML_EditorWidget.pri)

TEMPLATE = app

SOURCES +=  tst_sfml_viewwidget.cpp
