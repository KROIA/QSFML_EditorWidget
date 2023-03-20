#pragma once
#include <chrono>
#include "canvas/CanvasForwardDeclaration.h"

#if GCC_COMPILER
typedef std::chrono::system_clock::time_point TimePoint;
#else
typedef std::chrono::steady_clock::time_point TimePoint;
#endif
