#pragma once
#include "canvas/CanvasForwardDeclaration.h"

namespace QSFML
{
namespace Utilities
{
    class Updatable
    {
            friend Objects::CanvasObject;
        protected:
            virtual void update() = 0;
    };
}
}
