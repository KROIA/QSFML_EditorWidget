#include "BoxCollider.h"
namespace QSFML
{
namespace Components
{
BoxCollider::BoxCollider(const std::string &name)
    : Component(name)
    , AABB(0,0,0,0)
{

}
BoxCollider::BoxCollider(const BoxCollider &other)
    : Component(other)
    , AABB(other)
{

}
BoxCollider::BoxCollider(const std::string &name,
                         const Utilities::AABB &box)
    : Component(name)
    , AABB(box)
{

}
BoxCollider::~BoxCollider()
{

}
}
}
