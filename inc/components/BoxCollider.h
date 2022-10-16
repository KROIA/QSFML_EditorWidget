#pragma once

#include "AABB.h"
#include "Component.h"

namespace QSFML
{
namespace Components
{

class BoxCollider : public Component, public Utilities::AABB
{
    public:
        explicit BoxCollider(const std::string &name = "BoxCollider");
        explicit BoxCollider(const std::string &name, const Utilities::AABB &box);
        BoxCollider(const BoxCollider &other);
        ~BoxCollider();

        CLONE_FUNC(BoxCollider)


};
}
}
