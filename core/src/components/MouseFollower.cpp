#include "components/MouseFollower.h"
#include "objects/base/GameObject.h"
#include "objects/CameraWindow.h"

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(MouseFollower)
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

void MouseFollower::sfEvent(const QSFML::pair<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events)
{
    Objects::CameraWindow* cam = events.first;
    for (auto& event : events.second)
    {
        switch (event.type)
        {
            case sf::Event::MouseMoved:
            {
                sf::Vector2f worldPos = cam->getThisCameraMouseWorldPosition();
                sf::Vector2i pixelPos = cam->getThisCameraMousePosition();
    
                emit mousePosChanged(worldPos, pixelPos);
                return;
            }
            default:
                break;
        }
    }
}
}
}
