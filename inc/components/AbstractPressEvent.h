#pragma once

#include <QObject>
#include <SFML/Graphics.hpp>
#include "components/Component.h"
namespace QSFML
{
namespace Components
{
class AbstractPressEvent : public QObject,  public Component
{
        Q_OBJECT
    public:
        /**
         * \brief SfEventHandle constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        AbstractPressEvent(const std::string &name = "AbstractPressEvent");
        AbstractPressEvent(const AbstractPressEvent &other);

        /**
         * \brief update will be called once per frame
         */
        void update() final;

    signals:
        void fallingEdge();
        void down();
        void risingEdge();

    protected:
        virtual bool getCurrentValue() = 0;
    private:
        bool m_fallingEdge;
        bool m_down;
        bool m_risingEdge;
};
}
}
