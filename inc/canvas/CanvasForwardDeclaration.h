#pragma once

namespace QSFML
{
    class Canvas;
    class CanvasObjectContainer;
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
