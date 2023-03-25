#pragma once

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
    inline classNameVal* clone() const override \
    { \
        return new classNameVal(*this); \
    }
}
