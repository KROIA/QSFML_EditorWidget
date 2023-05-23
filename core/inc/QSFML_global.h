#pragma once

#include <QtCore/qglobal.h>
#include <chrono>

#ifdef BUILD_STATIC
#define QSFML_EDITOR_WIDGET_STATIC
#endif

#ifndef QSFML_EDITOR_WIDGET_STATIC
	#if defined(QSFML_EDITOR_WIDGET_LIB)
		#define QSFML_EDITOR_WIDGET_EXPORT Q_DECL_EXPORT
	#else
		#define QSFML_EDITOR_WIDGET_EXPORT Q_DECL_IMPORT
	#endif
#else
	#define SFML_STATIC
	#define EASY_PROFILER_STATIC
	#define QSFML_EDITOR_WIDGET_EXPORT
#endif


// MSVC Compiler
#ifdef _MSC_VER 
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	typedef std::chrono::steady_clock::time_point TimePoint;
#else
	typedef std::chrono::system_clock::time_point TimePoint;
#endif

