#include "components/MouseFollower.h"
#include "objects/CanvasObject.h"

namespace QSFML
{
namespace Components
{
MouseFollower::MouseFollower(const std::string &name)
    : QObject()
    , SfEventHandle(name)
{

}
MouseFollower::MouseFollower(const MouseFollower &other)
    : QObject()
    , SfEventHandle(other)
{

}

void MouseFollower::sfEvent(const sf::Event &e)
{
     switch(e.type)
     {
         case sf::Event::MouseMoved:
         {
             sf::Vector2f worldPos = getParent()->getMouseWorldPosition();
             sf::Vector2i pixelPos = getParent()->getMousePosition();

             emit mousePosChanged(worldPos,pixelPos);
         }
         default:
             break;
     }
}
}
}
