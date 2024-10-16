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


#if defined(EASTL) && defined(QSFML_USE_EASTL_IF_AVAILABLE)
#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/map.h>
#include <EASTL/list.h>
#include <EASTL/string.h>

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
}

// https://github.com/electronicarts/EASTL/blob/master/doc/CMake/EASTL_Project_Integration.md#setting-up-your-code
// https://stackoverflow.com/questions/40856087/how-to-build-eastl-library
// EASTL requires you to have an overload for the operator new[], here is an example that just forwards to global new[]:
void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);

#else
#include <vector>
#include <unordered_map>
#include <map>
#include <list>

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
}
#endif

/// USER_SECTION_END