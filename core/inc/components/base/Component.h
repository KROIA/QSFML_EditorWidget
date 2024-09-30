#pragma once

#include "QSFML_EditorWidget_base.h"

#include "utilities/AABB.h"
#include "events/DestroyEvent.h"

#include <string>

namespace QSFML
{
namespace Components
{
#define COMPONENT_DECL(className) \
    CLONE_FUNC_DEC(className) override;

#define COMPONENT_IMPL(className) \
    CLONE_FUNC_IMPL(className)

class Component;
typedef std::shared_ptr<Component> ComponentPtr;

/**
 * \brief Component class for GameObjects
 * \details Each GameObject can contain multiple Components.
 *          A Component is a module which can do some work on an
 *          GameObject.
 *          Examples for modules are:
 *           - Drawing domething
 *           - Handle sf::Events
 *           - Check for collision
 *           ...
 *
 *           See predefined Components
 *
 */
class QSFML_EDITOR_WIDGET_EXPORT Component : public Events::DestroyEvent
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

        virtual CLONE_FUNC_DEC(Component);

        /**
         * \brief setParent
         * \param parent
         *        Assigns this Component to an GameObject.
         *        This will be called by the GameObject::addChild(...),
         *        So you don't have to call this manually.
         */
        virtual void setParent(Objects::GameObject* parent)
        {
			m_parent = parent;
		}

        virtual void setSceneParent(Scene* parent)
        {
            m_SceneParent = parent;
        }

        /**
         * \brief getParent
         * \return returns the parent GameObject, this component belongs to
         */
        Objects::GameObject* getParent() const
        {
			return m_parent;
        }

        /**
         * \brief setName
         * \param name of the Component
         */
        void setName(const std::string& name)
        {
			m_name = name;
        }

        /**
         * \brief getName
         * \param returns the name of this Component
         */
        inline const std::string getName() const 
        { 
            return m_name; 
        }

        /**
         * \brief setEnabled
         * \param enable or disable this Component
         *        If disabled, the component will not get processed
         */
        void setEnabled(bool enable)
        {
			m_enabled = enable;
		}

        /**
         * \brief isEnabled
         * \param return true if this component is enabled, otherwise false
         */
        inline bool isEnabled() const 
        { 
            return m_enabled; 
        }

        


    protected:
        void deleteThis();

        // Scene operations
        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i& pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f& worldSpace) const;


        const sf::View getCameraView() const;
        const sf::View& getDefaultCameraView() const;
        Utilities::AABB getCameraViewRect() const;
        sf::Vector2u getSceneSize() const;
        sf::Vector2u getOldSceneSize() const;

        const sf::Font& getTextFont() const;

        size_t getTick() const;
        double getDeltaT() const; // Returns delta Time since last update in seconds

        /**
         * \brief getSceneParent
         * \return returns the Scene, this component belongs to
         */
        Scene* getSceneParent() const;
        // ---------

        Objects::GameObject* m_parent;
        Scene* m_SceneParent;

    private:

        bool m_enabled;
        std::string m_name;
        
};

}
}
