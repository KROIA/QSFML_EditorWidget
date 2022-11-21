#include "components/physics/Collider.h"
#include "objects/CanvasObject.h"



namespace QSFML
{
namespace Components
{

Collider::Collider(const std::string &name)
    :   Component(name)
{

}
Collider::Collider(const Collider &other)
    :   Component(other)
{

}



void Collider::checkCollision(const std::vector<Objects::CanvasObject*> &objs,
                              std::vector<Collisioninfo> &collisions)
{

    collisions.clear();
    for(size_t i=0; i<objs.size(); ++i)
    {
        Objects::CanvasObject *obj1 = objs[i];
        if(!obj1->isEnabled())
            continue;
        if(getParent()->getRootParent() == obj1->getRootParent())
            continue; // Don't check collision on objects chich are
                      // contained within the same root object.
        std::vector<Components::Collider*> coll = obj1->getCollider();

    }

}
}
}
