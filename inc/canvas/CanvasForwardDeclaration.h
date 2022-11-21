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

#define CLONE_FUNC(classNameVal) \
    classNameVal* clone() const override \
    { \
        return new classNameVal(*this); \
    }
}
