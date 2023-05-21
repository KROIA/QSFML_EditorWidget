#pragma once
#include "canvas/CanvasForwardDeclaration.h"
#include "QSFML_base.h"

namespace QSFML
{
namespace Utilities
{
    class QSFML_EDITOR_WIDGET_EXPORT Updatable
    {
            friend Objects::CanvasObject;
        protected:
            virtual void update() = 0;
    };
}
}
