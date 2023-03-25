#pragma once

#include <string>
#include "canvas/CanvasForwardDeclaration.h"
#include "QSFML_global.h"

namespace QSFML
{
namespace Components
{

/**
 * \brief Component class for CanvasObjects
 * \details Each CanvasObject can contain multiple Components.
 *          A Component is a module which can do some work on an
 *          CanvasObject.
 *          Examples for modules are:
 *           - Drawing domething
 *           - Handle sf::Events
 *           - Check for collision
 *           ...
 *
 *           See predefined Components
 *
 */
class QSFML_EDITOR_WIDGET_EXPORT Component
{
    public:
        /**
         * \brief Component constructor
         * \param name of the Component
         * \see setName()
         */
        Component(const std::string &name = "");
        Component(const Component &other);
        virtual ~Component();

        virtual Component *clone() const;

        /**
         * \brief setParent
         * \param parent
         *        Assigns this Component to an CanvasObject.
         *        This will be called by the CanvasObject::addChild(...),
         *        So you don't have to call this manually.
         */
        virtual void setParent(Objects::CanvasObject *parent);

        /**
         * \brief getParent
         * \return returns the parent CanvasObject, this component belongs to
         */
        inline Objects::CanvasObject *getParent() const { return m_parent; }

        /**
         * \brief setName
         * \param name of the Component
         */
        void setName(const std::string &name);

        /**
         * \brief getName
         * \param returns the name of this Component
         */
        inline const std::string getName() const { return m_name; }

        /**
         * \brief setEnabled
         * \param enable or disable this Component
         *        If disabled, the component will not get processed
         */
        void setEnabled(bool enable);

        /**
         * \brief isEnabled
         * \param return true if this component is enabled, otherwise false
         */
        inline bool isEnabled() const { return m_enabled; }


    protected:

    private:
        bool m_enabled;
        std::string m_name;
        Objects::CanvasObject *m_parent;
};
}
}
