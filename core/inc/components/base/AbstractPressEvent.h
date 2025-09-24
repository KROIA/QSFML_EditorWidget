#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "utilities/Updatable.h"

#include <QObject>
#include <SFML/Graphics.hpp>

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_API AbstractPressEvent : public QObject,  public Component, public Utilities::Updatable
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

    signals:
        void fallingEdge();
        void down();
        void risingEdge();

    protected:
        virtual void onFallingEdge();
        virtual void onDown();
        virtual void onRisingEdge();

        /**
         * \brief update will be called once per frame
         */
        void update() final;

        virtual bool getCurrentValue() = 0;
    private:
        bool m_fallingEdge;
        bool m_down;
        bool m_risingEdge;
};
}
}
