#pragma once
// #include "utilities/AABB.h"
#include "components/Component.h"
#include "canvas/CanvasForwardDeclaration.h"
#include <vector>
#include "QSFML_global.h"

namespace QSFML
{
namespace Components
{
    class Collider;
    class QSFML_EDITOR_WIDGET_EXPORT Collisioninfo
    {
        public:
        Collider *collider1;
        Collider *collider2;
    };

    class QSFML_EDITOR_WIDGET_EXPORT Collider: public Component
    {
        public:
            explicit Collider(const std::string &name = "Collider");
            Collider(const Collider &other);

            COMPONENT_DECL(Collider)


        protected:
            void checkCollision(const std::vector<Objects::CanvasObject*> &objs,
                                std::vector<Collisioninfo> &collisions);
    };


}
}
