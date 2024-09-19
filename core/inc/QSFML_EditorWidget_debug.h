#pragma once
#include "QSFML_EditorWidget_global.h"

// The Logger library is automaticly included if the logger dependency .cmake file is available
#if defined(LOGGER) and not defined(LOGGER_LIB)
	#include "Logger.h"
#endif

/// USER_SECTION_START 1

/// USER_SECTION_END

// Debugging
#ifdef NDEBUG
	#define QSFML_CONSOLE(msg)
	#define QSFML_CONSOLE_FUNCTION(msg)
#else
	#include <iostream>

	#define QSFML_DEBUG
	#define QSFML_CONSOLE_STREAM std::cout

	#define QSFML_CONSOLE(msg) QSFML_CONSOLE_STREAM << msg;
	#define QSFML_CONSOLE_FUNCTION(msg) QSFML_CONSOLE_STREAM << __PRETTY_FUNCTION__ << " " << msg;
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

#ifdef QSFML_PROFILING
	#include "easy/profiler.h"
	#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here

	#define QSFML_PROFILING_BLOCK_C(text, color) EASY_BLOCK(text, color)
	#define QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color) EASY_NONSCOPED_BLOCK(text, color)
	#define QSFML_PROFILING_END_BLOCK EASY_END_BLOCK
	#define QSFML_PROFILING_FUNCTION_C(color) EASY_FUNCTION(color)
	#define QSFML_PROFILING_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK_C(text,profiler::colors::  colorStage)
	#define QSFML_PROFILING_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK_C(text,profiler::colors::  colorStage)
	#define QSFML_PROFILING_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION_C(profiler::colors:: colorStage)
	#define QSFML_PROFILING_THREAD(name) EASY_THREAD(name)

	#define QSFML_PROFILING_VALUE(name, value) EASY_VALUE(name, value)
	#define QSFML_PROFILING_TEXT(name, value) EASY_TEXT(name, value)

#else
	#define QSFML_PROFILING_BLOCK_C(text, color)
	#define QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
	#define QSFML_PROFILING_END_BLOCK
	#define QSFML_PROFILING_FUNCTION_C(color)
	#define QSFML_PROFILING_BLOCK(text, colorStage)
	#define QSFML_PROFILING_NONSCOPED_BLOCK(text, colorStage)
	#define QSFML_PROFILING_FUNCTION(colorStage)
	#define QSFML_PROFILING_THREAD(name)

	#define QSFML_PROFILING_VALUE(name, value)
	#define QSFML_PROFILING_TEXT(name, value)
#endif

// Special expantion tecniques are required to combine the color name
#define CONCAT_SYMBOLS_IMPL(x, y) x##y
#define CONCAT_SYMBOLS(x, y) CONCAT_SYMBOLS_IMPL(x, y)



// Different color stages
#define QSFML_COLOR_STAGE_1 50
#define QSFML_COLOR_STAGE_2 100
#define QSFML_COLOR_STAGE_3 200
#define QSFML_COLOR_STAGE_4 300
#define QSFML_COLOR_STAGE_5 400
#define QSFML_COLOR_STAGE_6 500
#define QSFML_COLOR_STAGE_7 600
#define QSFML_COLOR_STAGE_8 700
#define QSFML_COLOR_STAGE_9 800
#define QSFML_COLOR_STAGE_10 900
#define QSFML_COLOR_STAGE_11 A100 
#define QSFML_COLOR_STAGE_12 A200 
#define QSFML_COLOR_STAGE_13 A400 
#define QSFML_COLOR_STAGE_14 A700 

namespace QSFML
{
/// USER_SECTION_START 4

/// USER_SECTION_END
	class QSFML_EDITOR_WIDGET_EXPORT Profiler
	{
	public:
		/// USER_SECTION_START 5

		/// USER_SECTION_END

		// Implementation defined in LibraryName_info.cpp to save files.
		static void start();
		static void stop();
		static void stop(const char* profilerOutputFile);

		/// USER_SECTION_START 6

		/// USER_SECTION_END
	};

/// USER_SECTION_START 7

/// USER_SECTION_END


#if defined(LOGGER) and not defined(LOGGER_LIB)
	class QSFML_EDITOR_WIDGET_EXPORT Logger 
	{
		/// USER_SECTION_START 8

		/// USER_SECTION_END
		Logger();
		static Logger& instance();
		public:
		/// USER_SECTION_START 9

		/// USER_SECTION_END

		static void setEnabled(bool enable);
		static bool isEnabled();
		static void setName(const std::string& name);
		static std::string getName();
		static void setColor(const Log::Color& col);
		static Log::Color getColor();
		static Log::DateTime getCreationDateTime();
		static Log::LoggerID getID();
		static void setParentID(Log::LoggerID parentID);
		static Log::LoggerID getParentID();



		static void log(const Log::Message& msg);

		static void log(const std::string& msg);
		static void log(const std::string& msg, Log::Level level);
		static void log(const std::string& msg, Log::Level level, const Log::Color& col);

		static void logTrace(const std::string& msg);
		static void logDebug(const std::string& msg);
		static void logInfo(const std::string& msg);
		static void logWarning(const std::string& msg);
		static void logError(const std::string& msg);
		static void logCustom(const std::string& msg);

		/// USER_SECTION_START 10

		/// USER_SECTION_END

		private:
		Log::LogObject m_logObject;

		/// USER_SECTION_START 11

		/// USER_SECTION_END
	};
/// USER_SECTION_START 12

/// USER_SECTION_END
#endif
}


// General
#define QSFML_GENERAL_PROFILING_COLORBASE Cyan
#define QSFML_GENERAL_PROFILING_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFML_GENERAL_PROFILING_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFML_GENERAL_PROFILING_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFML_GENERAL_PROFILING_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFML_GENERAL_PROFILING_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_GENERAL_PROFILING_COLORBASE, colorStage))
#define QSFML_GENERAL_PROFILING_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_GENERAL_PROFILING_COLORBASE, colorStage))
#define QSFML_GENERAL_PROFILING_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_GENERAL_PROFILING_COLORBASE, colorStage))
#define QSFML_GENERAL_PROFILING_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFML_GENERAL_PROFILING_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)


/// USER_SECTION_START 3
// General
#define QSFML_PROFILING_GENERAL_COLORBASE Cyan
#define QSFMLP_GENERAL_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFMLP_GENERAL_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFMLP_GENERAL_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFMLP_GENERAL_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFMLP_GENERAL_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_GENERAL_COLORBASE, colorStage))
#define QSFMLP_GENERAL_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_GENERAL_COLORBASE, colorStage))
#define QSFMLP_GENERAL_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_PROFILING_GENERAL_COLORBASE, colorStage))
#define QSFMLP_GENERAL_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFMLP_GENERAL_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)

// Canvas
#define QSFML_PROFILING_CANVAS_COLORBASE Green
#define QSFMLP_CANVAS_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFMLP_CANVAS_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFMLP_CANVAS_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFMLP_CANVAS_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFMLP_CANVAS_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_CANVAS_COLORBASE, colorStage))
#define QSFMLP_CANVAS_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_CANVAS_COLORBASE, colorStage))
#define QSFMLP_CANVAS_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_PROFILING_CANVAS_COLORBASE, colorStage))
#define QSFMLP_CANVAS_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFMLP_CANVAS_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)

// Objects
#define QSFML_PROFILING_OBJECT_COLORBASE Orange
#define QSFMLP_OBJECT_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFMLP_OBJECT_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFMLP_OBJECT_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFMLP_OBJECT_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFMLP_OBJECT_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_OBJECT_COLORBASE, colorStage))
#define QSFMLP_OBJECT_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_OBJECT_COLORBASE, colorStage))
#define QSFMLP_OBJECT_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_PROFILING_OBJECT_COLORBASE, colorStage))
#define QSFMLP_OBJECT_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFMLP_OBJECT_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)

// Components
#define QSFML_PROFILING_COMPONENT_COLORBASE Yellow
#define QSFMLP_COMPONENT_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFMLP_COMPONENT_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFMLP_COMPONENT_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFMLP_COMPONENT_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFMLP_COMPONENT_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_COMPONENT_COLORBASE, colorStage))
#define QSFMLP_COMPONENT_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_COMPONENT_COLORBASE, colorStage))
#define QSFMLP_COMPONENT_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_PROFILING_COMPONENT_COLORBASE, colorStage))
#define QSFMLP_COMPONENT_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFMLP_COMPONENT_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)

// Physics
#define QSFML_PROFILING_PHYSICS_COLORBASE Blue
#define QSFMLP_PHYSICS_BLOCK_C(text, color) QSFML_PROFILING_BLOCK_C(text, color)
#define QSFMLP_PHYSICS_NONSCOPED_BLOCK_C(text, color) QSFML_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define QSFMLP_PHYSICS_END_BLOCK QSFML_PROFILING_END_BLOCK;
#define QSFMLP_PHYSICS_FUNCTION_C(color) QSFML_PROFILING_FUNCTION_C(color)
#define QSFMLP_PHYSICS_BLOCK(text, colorStage) QSFML_PROFILING_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_PHYSICS_COLORBASE, colorStage))
#define QSFMLP_PHYSICS_NONSCOPED_BLOCK(text, colorStage) QSFML_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(QSFML_PROFILING_PHYSICS_COLORBASE, colorStage))
#define QSFMLP_PHYSICS_FUNCTION(colorStage) QSFML_PROFILING_FUNCTION(CONCAT_SYMBOLS(QSFML_PROFILING_PHYSICS_COLORBASE, colorStage))
#define QSFMLP_PHYSICS_VALUE(name, value) QSFML_PROFILING_VALUE(name, value)
#define QSFMLP_PHYSICS_TEXT(name, value) QSFML_PROFILING_TEXT(name, value)

/// USER_SECTION_END