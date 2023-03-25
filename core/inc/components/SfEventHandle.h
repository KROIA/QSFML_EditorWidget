#pragma once

#include <SFML/Graphics.hpp>
#include "components/Component.h"
#include "QSFML_global.h"

namespace QSFML
{
namespace Components
{

/**
 * \brief SfEventHandle handle sf::Events
 * \details This is one of the predefined Components.
 *          The SfEventHandle has a event function
 *          which will provide the sf::Events.
 *          This function will be called on every frame (if there are events)
 *          If multiple events are detected, this function gets called multiple times.
 *
 *          You must inherit from this class and then override the sfEvent(...) function
 * \code
 * class MyHandler : public QSFML::Components::SfEventHandle
 * {
 *     public:
 *     MyHandler(const std::string &name = "MyHandler")
 *         : SfEventHandle(name)
 *     {
 *
 *     }
 *
 *     void sfEvent(const sf::Event &e) override
 *     {
 *          switch(e.type)
 *          {
 *              case sf::Event::MouseMoved:
 *              {
 *                  sf::Vector2i mousePos = sf::Vector2i(e.mouseMove.x,e.mouseMove.y);
 *                  // Do something with the moved mouse pos
 *              }
 *          }
 *          default:
 *              break;
 *     }
 * };
 * \endcode
 */
class QSFML_EDITOR_WIDGET_EXPORT SfEventHandle :   public Component
{
    public:
        /**
         * \brief SfEventHandle constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        SfEventHandle(const std::string &name = "SfEventHandle");
        SfEventHandle(const SfEventHandle &other);
        virtual ~SfEventHandle();


        /**
         * \brief sfEvent
         * \details This function will be automaticlly called from the
         *          Canvas n times per frame if the Component is enabled
         * \param e, The sf::Event object
         */
        virtual void sfEvent(const sf::Event &e) = 0;
};
}
}
