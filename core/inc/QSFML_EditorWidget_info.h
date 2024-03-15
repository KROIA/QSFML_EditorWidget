#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

#include "QSFML_EditorWidget_base.h"
#include <sstream>

/// USER_SECTION_START 2

/// USER_SECTION_END

class QWidget;

/// USER_SECTION_START 3

/// USER_SECTION_END

namespace QSFML
{

/// USER_SECTION_START 4

/// USER_SECTION_END

	class QSFML_EDITOR_WIDGET_EXPORT LibraryInfo
	{
		LibraryInfo() = delete;
		LibraryInfo(const LibraryInfo&) = delete;
	public:
		// Current version of the library
		static constexpr int versionMajor				= 1;
		static constexpr int versionMinor				= 0;
		static constexpr int versionPatch				= 0;

		// Library name
		static constexpr const char* name				= "QSFML_EditorWidget";
		static constexpr const char* author				= "Alex Krieg";
		static constexpr const char* email				= "";
		static constexpr const char* website			= "";
		static constexpr const char* license			= "MIT";
		static constexpr const char* compilationDate	= __DATE__;
		static constexpr const char* compilationTime	= __TIME__;

		// Compiler information
#ifdef _MSC_VER
		static constexpr const char* compiler			= "MSVC";
		static constexpr const long compilerVersion		= _MSC_VER;
#elif defined(__GNUC__)
		static constexpr const char* compiler			= "GCC";
		static constexpr const long compilerVersion		= __VERSION__;
#elif defined(__clang__)

		static constexpr const char* compiler			= "Clang";
		static constexpr const long compilerVersion		= __clang_version__;
#else
		static constexpr const char* compiler			= "Unknown";
		static constexpr const long compilerVersion		= "Unknown";
#endif

		// Build type
		enum class BuildType
		{
			debug,
			release
		};
#ifdef NDEBUG
		static constexpr const char* buildTypeStr		= "Release";
		static constexpr const BuildType buildType		= BuildType::release;
#else
		static constexpr const char* buildTypeStr		= "Debug";
		static constexpr const BuildType buildType		= BuildType::debug;
#endif

		static const std::string& versionStr()
		{
			static const std::string str = {
				'0' + versionMajor / 10,
				'0' + versionMajor % 10,
				'.',
				'0' + versionMinor / 10,
				'0' + versionMinor % 10,
				'.',
				'0' + versionPatch / 10,
				'0' + versionPatch % 10
			};
			return str;
		}

		static void printInfo();
		static void printInfo(std::ostream& stream);

		// This function is only available when QT_ENABLE was set to ON in the CMakeLists.txt and
		// QT_MODULES contains the value "Widgets"
		// It creates a widget with the library information
		// No button is created to close the widget
		static QWidget *createInfoWidget(QWidget* parent = nullptr);

/// USER_SECTION_START 5

/// USER_SECTION_END
	};

/// USER_SECTION_START 6

/// USER_SECTION_END
}

/// USER_SECTION_START 7

/// USER_SECTION_END
