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

    protected:
        bool getCurrentValue() override;
    private:
        sf::Mouse::Button m_button;
};
}
}
