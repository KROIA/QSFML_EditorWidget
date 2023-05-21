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
    namespace Components
    {
        class Collider;
    }

#define CLONE_FUNC_DEC(className) \
    className *clone() const

#define CLONE_FUNC_IMPL(className) \
    className* className::clone() const \
    { \
        return new className(*this); \
    }

}
