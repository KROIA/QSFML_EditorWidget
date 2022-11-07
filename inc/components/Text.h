#pragma once

#include "components/Drawable.h"
#include "SFML/Graphics/Text.hpp"
namespace QSFML
{
namespace Components
{

class Text : public Drawable
{
    public:
        /**
         * \brief Text constructor
         * \param name of this Component
         * \see Component::setName()
         */
        Text(const std::string &name = "Text");
        Text(const Text &other);
        CLONE_FUNC(Text)

        void setText(const std::string &text);
        std::string getText() const;

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;

        void setPosition(const sf::Vector2f &pos);
        const sf::Vector2f &getPosition() const;




        void setParent(Objects::CanvasObject *parent) override;

        /**
         * \brief draw
         * \details This function will be automaticlly called from the
         *          Canvas once per frame if the Component is enabled
         * \param target to draw on
         * \param states
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    protected:
        void updateCenter(const sf::Vector2f &pos);

        sf::Text m_text;
        sf::Vector2f m_pos;
};
}
}
