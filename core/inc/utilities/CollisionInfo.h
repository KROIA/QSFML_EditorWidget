#pragma once

#include "QSFML_EditorWidget_base.h"
namespace QSFML
{
    namespace Utilities
    {
        struct QSFML_EDITOR_WIDGET_API Collisioninfo
        {
            const Components::Collider* collider1;
            const Components::Collider* collider2;
            struct QSFML_EDITOR_WIDGET_API VertexIndex
            {
                size_t index1;
                size_t index2;
            };
            VertexIndex vertexIndex; // Vector from this vertex to the next had a collision 
            sf::Vector2f collisionPos;
        };
    }
}
