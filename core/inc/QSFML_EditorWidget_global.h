#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

#include <chrono>
// Params
// <LIBRARY NAME SHORT>=QSFML 
//
#ifndef BUILD_STATIC
	//#pragma message("QSFML_EDITOR_WIDGET_LIB is a shared library")
	#if defined(QSFML_EDITOR_WIDGET_LIB)
		#define QSFML_EDITOR_WIDGET_EXPORT __declspec(dllexport)
	#else
		#define QSFML_EDITOR_WIDGET_EXPORT __declspec(dllimport)
	#endif
#else 
	//#pragma message("QSFML_EDITOR_WIDGET_LIB is a static library")
	#define QSFML_EDITOR_WIDGET_EXPORT
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

#ifdef QT_ENABLED
	//#pragma message("QT is enabled")
	#ifdef QT_WIDGETS_ENABLED
		//#pragma message("QT_WIDGETS is enabled")
	#endif
#endif

/// USER_SECTION_START 3
//#define QSFML_OPTIMIZED
/// USER_SECTION_END

// MSVC Compiler
#ifdef _MSC_VER 
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	typedef std::chrono::steady_clock::time_point TimePoint;
#else
	typedef std::chrono::system_clock::time_point TimePoint;
#endif


#define QSFML_UNUSED(x) (void)x;

/// USER_SECTION_START 4
#ifndef QSFML_ASSERT
#include <assert.h>
#define QSFML_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif
/// USER_SECTION_END

#if defined(QSFML_EDITOR_WIDGET_LIB)
	#pragma warning (error : 4715) // not all control paths return a value shuld be an error instead of a warning
	#pragma warning (error : 4700) // uninitialized local variable used shuld be an error instead of a warning
	#pragma warning (error : 4244) // Implicit conversions between data types 
	#pragma warning (error : 4100) // Unused variables
	#pragma warning (error : 4018) // Type mismatch 
	#pragma warning (error : 4996) // Unsafe function calls
	#pragma warning (error : 4456) // declaration of 'x' hides previous local declaration
	#pragma warning (error : 4065) // switch statement contains 'default' but no 'case' labels
	#pragma warning (error : 4189) // Unused return value
	#pragma warning (error : 4996) // unsafe function calls
	#pragma warning (error : 4018) // signed/unsigned mismatch
	#pragma warning (error : 4172) // Returning address of local temporary object
#endif

/// USER_SECTION_START 5

/*
* Enables OpenGL to draw internal Components
* If not defined, the default SFML drawing will be used
*/
#define QSFML_USE_GL_DRAW

/*
* Enables EA STL library if available
* EA STL is a custom implementation of the C++ Standard Library
* It is used in the Electronic Arts games
* Git: https://github.com/electronicarts/EASTL.git
* If not defined, the default STL will be used
*/
//#define QSFML_USE_EASTL_IF_AVAILABLE

/*
* Enables the use of break points in debug mode.
* The debuger will break at lines where a critical error occurs.
* If not defined, the program will continue to run or may crash
*/
#define QSFML_ENABLE_DEBUG_BREAK

/// USER_SECTION_END