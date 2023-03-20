#pragma once

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

namespace QSFML
{
    class Canvas;
    class CanvasObjectGroup;
    class CanvasObjectContainer;
    class CanvasThreadWorker;
    namespace Objects
    {
        class CanvasObject;
    }
    namespace Utilities
    {
        class Updatable;
    }

#define CLONE_FUNC(classNameVal) \
    classNameVal* clone() const override \
    { \
        return new classNameVal(*this); \
    }
}
