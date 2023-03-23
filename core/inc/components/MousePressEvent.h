#pragma once

#include <QObject>
#include <SFML/Graphics.hpp>
#include "components/AbstractPressEvent.h"

namespace QSFML
{
namespace Components
{
class MousePressEvent : public AbstractPressEvent
{
        Q_OBJECT
    public:
        /**
         * \brief MousePressEvent constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        MousePressEvent(const std::string &name = "MousePressEvent",
                        sf::Mouse::Button button = sf::Mouse::Button::Left);
        MousePressEvent(const MousePressEvent &other);
        CLONE_FUNC(MousePressEvent)

        void setTriggerButton(sf::Mouse::Button button);
        sf::Mouse::Button getTriggerButton() const;

        const sf::Vector2f &getLastPressedWorldPos() const;
        const sf::Vector2i &getLastPressedPixelPos() const;

    protected:
        bool getCurrentValue() override;
    private:
        sf::Mouse::Button m_button;
        sf::Vector2f m_currentPressedWorldPosition;
        sf::Vector2i m_currentPressedPixelPosition;
};
}
}
