#pragma once

#include <QtCore/qglobal.h>
#include <chrono>

//#define QSFML_OPTIMIZED

#ifdef BUILD_STATIC
#define QSFML_EDITOR_WIDGET_STATIC
#endif

//#define SFML_STATIC
#ifndef QSFML_EDITOR_WIDGET_STATIC
	#if defined(QSFML_EDITOR_WIDGET_LIB)
		#define QSFML_EDITOR_WIDGET_EXPORT Q_DECL_EXPORT
	#else
		#define QSFML_EDITOR_WIDGET_EXPORT Q_DECL_IMPORT
	#endif
#else
	
	//#define EASY_PROFILER_STATIC
	#define QSFML_EDITOR_WIDGET_EXPORT
#endif


// MSVC Compiler
#ifdef _MSC_VER 
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	typedef std::chrono::steady_clock::time_point TimePoint;
#else
	typedef std::chrono::system_clock::time_point TimePoint;
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_2PI 6.28318530717958647692
#endif
