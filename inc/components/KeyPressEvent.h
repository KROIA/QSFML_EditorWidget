#pragma once

#include <QObject>
#include <SFML/Graphics.hpp>
#include "AbstractPressEvent.h"

namespace QSFML
{
namespace Components
{
class KeyPressEvent : public AbstractPressEvent
{
        Q_OBJECT
    public:
        /**
         * \brief KeyPressEvent constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        KeyPressEvent(const std::string &name = "KeyPressEvent",
                      const sf::Keyboard::Key key = sf::Keyboard::Key::Space);
        KeyPressEvent(const KeyPressEvent &other);
        CLONE_FUNC(KeyPressEvent)

        void setTriggerKey(sf::Keyboard::Key key);
        sf::Keyboard::Key getTriggerKey() const;

    protected:
        bool getCurrentValue() override;
    private:
        sf::Keyboard::Key m_key;
};
}
}
