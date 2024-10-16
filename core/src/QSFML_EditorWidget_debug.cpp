#include "QSFML_EditorWidget_info.h"
#include "QSFML_EditorWidget_debug.h"

/// USER_SECTION_START 1

/// USER_SECTION_END



/// USER_SECTION_START 2
#if defined(EASTL) && defined(QSFML_USE_EASTL_IF_AVAILABLE)
// https://github.com/electronicarts/EASTL/blob/master/doc/CMake/EASTL_Project_Integration.md#setting-up-your-code
// https://stackoverflow.com/questions/40856087/how-to-build-eastl-library
void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	QSFML_UNUSED(name);
	QSFML_UNUSED(flags);
	QSFML_UNUSED(debugFlags);
	QSFML_UNUSED(file);
	QSFML_UNUSED(line);
	return malloc(size);
}
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	QSFML_UNUSED(alignment);
	QSFML_UNUSED(alignmentOffset);
	QSFML_UNUSED(pName);
	QSFML_UNUSED(flags);
	QSFML_UNUSED(debugFlags);
	QSFML_UNUSED(file);
	QSFML_UNUSED(line);
	return malloc(size);
}

// EASTL also wants us to define this (see string.h line 197)

int Vsnprintf8(char* pDestination, size_t n, const char* pFormat, va_list arguments)
{
#ifdef _MSC_VER
	QSFML_UNUSED(pDestination);
	QSFML_UNUSED(n);
	QSFML_UNUSED(pFormat);
	QSFML_UNUSED(arguments);
	return 0;// _vsnprintf(pDestination, n, pFormat, arguments);
#else
	return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}

int Vsnprintf16(char16_t* pDestination, size_t n, const char16_t* pFormat, va_list arguments)
{
#ifdef _MSC_VER
	QSFML_UNUSED(pDestination);
	QSFML_UNUSED(n);
	QSFML_UNUSED(pFormat);
	QSFML_UNUSED(arguments);
	return 0;// _vsnwprintf((wchar_t*)pDestination, n, (wchar_t*)pFormat, arguments);
#else
	char* d = new char[n + 1];
	int r = vsnprintf(d, n, convertstring<char16_t, char>(pFormat).c_str(), arguments);
	memcpy(pDestination, convertstring<char, char16_t>(d).c_str(), (n + 1) * sizeof(char16_t));
	delete[] d;
	return r;
#endif
}
namespace EA {
	namespace StdC {
		int Vsnprintf(char* pDestination, size_t n, const char* pFormat, va_list arguments)
		{
			QSFML_UNUSED(pDestination);
			QSFML_UNUSED(n);
			QSFML_UNUSED(pFormat);
			QSFML_UNUSED(arguments);
			return 0;
		}
	}
}
#endif
/// USER_SECTION_END

namespace QSFML
{
	/// USER_SECTION_START 3

	/// USER_SECTION_END

	// Implementation of the Profiler start/stop functions
	void Profiler::start()
	{
		/// USER_SECTION_START 4

		/// USER_SECTION_END
#ifdef QSFML_PROFILING
		EASY_PROFILER_ENABLE;
#endif
		/// USER_SECTION_START 5

		/// USER_SECTION_END
	}
	void Profiler::stop()
	{
		/// USER_SECTION_START 6

		/// USER_SECTION_END
		stop("profile.prof");
		/// USER_SECTION_START 7

		/// USER_SECTION_END
	}
	void Profiler::stop(const char* profilerOutputFile)
	{
		/// USER_SECTION_START 8

		/// USER_SECTION_END
#ifdef QSFML_PROFILING
		profiler::dumpBlocksToFile(profilerOutputFile);
#else
		(void)profilerOutputFile;
#endif
		/// USER_SECTION_START 9

		/// USER_SECTION_END
	}

/// USER_SECTION_START 55

/// USER_SECTION_END
#if defined(LOGGER)
	/// USER_SECTION_START 10

	/// USER_SECTION_END
	Logger::Logger()
		: m_logObject(std::string(LibraryInfo::name))
	{
		/// USER_SECTION_START 11

		/// USER_SECTION_END
	}
	/// USER_SECTION_START 12

	/// USER_SECTION_END

	Logger& Logger::instance()
	{
		static Logger logger;
		return logger;
	}

	void Logger::setEnabled(bool enable)
	{
		/// USER_SECTION_START 13

		/// USER_SECTION_END
		instance().m_logObject.setEnabled(enable);
		/// USER_SECTION_START 14

		/// USER_SECTION_END
	}
	bool Logger::isEnabled()
	{
		/// USER_SECTION_START 15

		/// USER_SECTION_END
		return instance().m_logObject.isEnabled();
		/// USER_SECTION_START 16

		/// USER_SECTION_END
	}
	void Logger::setName(const std::string& name)
	{
		/// USER_SECTION_START 17

		/// USER_SECTION_END
		instance().m_logObject.setName(name);
		/// USER_SECTION_START 18

		/// USER_SECTION_END
	}
	std::string Logger::getName()
	{
		/// USER_SECTION_START 19

		/// USER_SECTION_END
		return instance().m_logObject.getName();
		/// USER_SECTION_START 20

		/// USER_SECTION_END
	}
	void Logger::setColor(const Log::Color& col)
	{
		/// USER_SECTION_START 21

		/// USER_SECTION_END
		instance().m_logObject.setColor(col);
		/// USER_SECTION_START 22

		/// USER_SECTION_END
	}
	Log::Color Logger::getColor()
	{
		/// USER_SECTION_START 23

		/// USER_SECTION_END
		return instance().m_logObject.getColor();
		/// USER_SECTION_START 24

		/// USER_SECTION_END
	}
	Log::DateTime Logger::getCreationDateTime()
	{
		/// USER_SECTION_START 25

		/// USER_SECTION_END
		return instance().m_logObject.getCreationDateTime();
		/// USER_SECTION_START 26

		/// USER_SECTION_END
	}
	Log::LoggerID Logger::getID()
	{
		/// USER_SECTION_START 27

		/// USER_SECTION_END
		return instance().m_logObject.getID();
		/// USER_SECTION_START 28

		/// USER_SECTION_END
	}
	void Logger::setParentID(Log::LoggerID parentID)
	{
		/// USER_SECTION_START 29

		/// USER_SECTION_END
		instance().m_logObject.setParentID(parentID);
		/// USER_SECTION_START 30

		/// USER_SECTION_END
	}
	Log::LoggerID Logger::getParentID()
	{
		/// USER_SECTION_START 31

		/// USER_SECTION_END
		return instance().m_logObject.getParentID();
		/// USER_SECTION_START 32

		/// USER_SECTION_END
	}



	void Logger::log(const Log::Message& msg)
	{
		/// USER_SECTION_START 33

		/// USER_SECTION_END
		instance().m_logObject.log(msg);
		/// USER_SECTION_START 34

		/// USER_SECTION_END
	}

	void Logger::log(const std::string& msg)
	{
		/// USER_SECTION_START 35

		/// USER_SECTION_END
		instance().m_logObject.log(msg);
		/// USER_SECTION_START 36

		/// USER_SECTION_END
	}
	void Logger::log(const std::string& msg, Log::Level level)
	{
		/// USER_SECTION_START 37

		/// USER_SECTION_END
		instance().m_logObject.log(msg, level);
		/// USER_SECTION_START 38

		/// USER_SECTION_END
	}
	void Logger::log(const std::string& msg, Log::Level level, const Log::Color& col)
	{
		/// USER_SECTION_START 39

		/// USER_SECTION_END
		instance().m_logObject.log(msg, level, col);
		/// USER_SECTION_START 40

		/// USER_SECTION_END
	}

	void Logger::logTrace(const std::string& msg)
	{
		/// USER_SECTION_START 41

		/// USER_SECTION_END
		instance().m_logObject.logTrace(msg);
		/// USER_SECTION_START 42

		/// USER_SECTION_END
	}
	void Logger::logDebug(const std::string& msg)
	{
		/// USER_SECTION_START 43

		/// USER_SECTION_END
		instance().m_logObject.logDebug(msg);
		/// USER_SECTION_START 44

		/// USER_SECTION_END
	}
	void Logger::logInfo(const std::string& msg)
	{
		/// USER_SECTION_START 45

		/// USER_SECTION_END
		instance().m_logObject.logInfo(msg);
		/// USER_SECTION_START 46

		/// USER_SECTION_END
	}
	void Logger::logWarning(const std::string& msg)
	{
		/// USER_SECTION_START 47

		/// USER_SECTION_END
		instance().m_logObject.logWarning(msg);
		/// USER_SECTION_START 48

		/// USER_SECTION_END
	}
	void Logger::logError(const std::string& msg)
	{
		/// USER_SECTION_START 49

		/// USER_SECTION_END
		instance().m_logObject.logError(msg);
		/// USER_SECTION_START 50

		/// USER_SECTION_END
	}
	void Logger::logCustom(const std::string& msg)
	{
		/// USER_SECTION_START 51

		/// USER_SECTION_END
		instance().m_logObject.logCustom(msg);
		/// USER_SECTION_START 52

		/// USER_SECTION_END
	}

	/// USER_SECTION_START 53

	/// USER_SECTION_END

#endif
/// USER_SECTION_START 54

/// USER_SECTION_END
	

}