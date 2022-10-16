#include "MouseFollower.h"
#include "CanvasObject.h"

namespace QSFML
{
namespace Components
{
MouseFollower::MouseFollower(const std::string &name)
    : QObject()
    , SfEventHandle(name)
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
