#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include "components/Component.h"
namespace QSFML
{
namespace Components
{

/**
 * \brief Drawable to draw stuff
 * \details This is one of the predefined Components.<br>
 *          The Drawable overrides the draw(...)
 *          function from the sf::Drawable<br>
 *          The draw(...) function will be called on every frame.
 *
 *          You must inherit from this class and then override the draw(...) function<br>
 * \code
 * class MyDrawable : public QSFML::Components::Drawable
 * {
 *     public:
 *     MyDrawable(const std::string &name = "MyDrawable")
 *         : Drawable(name)
 *     {
 *
 *     }
 *     MyDrawable(const MyDrawable &other)
 *          : Drawable(other)
 *     {
 *
 *     }
 *
 *     void draw(sf::RenderTarget& target, sf::RenderStates states) const override
 *     {
 *          // target.draw(...) draw stuff
 *     }
 * };
 * \endcode
 *
 */
class Drawable : public Component, public sf::Drawable
{
    public:
        /**
         * \brief Drawable constructor
         * \param name of this Component
         * \see Component::setName()
         */
        Drawable(const std::string &name = "Drawable");
        Drawable(const Drawable &other);
        ~Drawable();



        /**
         * \brief draw
         * \details This function will be automaticlly called from the
         *          Canvas once per frame if the Component is enabled
         * \param target to draw on
         * \param states
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};
}
}
