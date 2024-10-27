#pragma once

/// USER_SECTION_START 1
#define _USE_MATH_DEFINES
#include <math.h>
/// USER_SECTION_END

#include "QSFML_EditorWidget_global.h"
#include "QSFML_EditorWidget_debug.h"
#include "QSFML_EditorWidget_info.h"

/// USER_SECTION_START 2
#include "QSFML_EditorWidget_ForwardDeclaration.h"
#include "SFML/System/Vector2.hpp"


#if EASTL_LIBRARY_AVAILABLE == 1 && defined(QSFML_USE_EASTL_IF_AVAILABLE)

namespace EA {
	namespace StdC {
		int Vsnprintf(char* pDestination, size_t n, const char* pFormat, va_list arguments);
	}
}

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/map.h>
#include <EASTL/list.h>
#include <EASTL/string.h>
#include <EASTL/sort.h>

namespace QSFML
{
	template<typename T>
	using vector = eastl::vector<T>;

	template<typename T>
	using list = eastl::list<T>;

	template<typename A, typename B>
	using pair = eastl::pair<A, B>;

	template<typename A, typename B>
	using unordered_map = eastl::unordered_map<A, B>;

	template<typename A, typename B>
	using map = eastl::map<A, B>;

	using string = eastl::string;

	template<typename T>
	string to_string(T v)
	{
		return eastl::to_string(v);
	}

	// sort redirection with lamda
	template<typename T>
	void sort(T begin, T end, std::function<bool(const typename T::value_type&, const typename T::value_type&)> compare)
	{
		eastl::sort(begin, end, compare);
	}
}

// https://github.com/electronicarts/EASTL/blob/master/doc/CMake/EASTL_Project_Integration.md#setting-up-your-code
// https://stackoverflow.com/questions/40856087/how-to-build-eastl-library
// EASTL requires you to have an overload for the operator new[], here is an example that just forwards to global new[]:
void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);

// EASTL also wants us to define this (see string.h line 197)
//int Vsnprintf(char* pDestination, size_t n, const char* pFormat, va_list arguments);
//int Vsnprintf8(char* pDestination, size_t n, const char* pFormat, va_list arguments);
//int Vsnprintf16(char16_t* pDestination, size_t n, const char16_t* pFormat, va_list arguments);


#else
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <string>

namespace QSFML
{
	template<typename T>
	using vector = std::vector<T>;

	template<typename T>
	using list = std::list<T>;

	template<typename A, typename B>
	using pair = std::pair<A, B>;

	template<typename A, typename B>
	using unordered_map = std::unordered_map<A, B>;

	template<typename A, typename B>
	using map = std::map<A, B>;

	using string = std::string;

	// to_string redirection
	template<typename T>
	string to_string(T v)
	{
		return std::to_string(v);
	}

	// sort redirection with lamda
	template<typename T>
	void sort(T begin, T end, std::function<bool(const typename T::value_type&, const typename T::value_type&)> compare)
	{
		std::sort(begin, end, compare);
	}
}
#endif

/// USER_SECTION_END