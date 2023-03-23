#pragma once
// #include "utilities/AABB.h"
#include "components/Component.h"
#include "canvas/CanvasForwardDeclaration.h"
#include <vector>

namespace QSFML
{
namespace Components
{
    class Collider;
    class Collisioninfo
    {
        public:
        Collider *collider1;
        Collider *collider2;
    };

    class Collider: public Component
    {
        public:
            explicit Collider(const std::string &name = "Collider");
            Collider(const Collider &other);

            CLONE_FUNC(Collider)


        protected:
            void checkCollision(const std::vector<Objects::CanvasObject*> &objs,
                                std::vector<Collisioninfo> &collisions);
    };


}
}
