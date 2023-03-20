#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SFML_EDITORWIDGET_LIB)
#  define SFML_EDITORWIDGET_EXPORT Q_DECL_EXPORT
# else
#  define SFML_EDITORWIDGET_EXPORT Q_DECL_IMPORT
# endif
#else
# define SFML_EDITORWIDGET_EXPORT
#endif
