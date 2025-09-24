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
    CLONE_FUNC_DECL(QSFML::Components::Component, className)

#define COMPONENT_IMPL(className) \
    CLONE_FUNC_IMPL(QSFML::Components::Component, className)

#define COMPONENT_HEADER_IMPL(className) \
    CLONE_FUNC_HEADER_IMPL(QSFML::Components::Component, className)

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
class QSFML_EDITOR_WIDGET_API Component : public Events::DestroyEvent
{
    friend class Internal::LifetimeChecker;
	friend class Objects::GameObject;
    protected:
        virtual ~Component();
    public:
        /**
         * \brief Component constructor
         * \param name of the Component
         * \see setName()
         */
        Component(const std::string &name = "");
        Component(const Component &other);

        static void deleteComponent(ComponentPtr comp);
        

        virtual Component* clone() const;

       

        /**
         * \brief getParent
         * \return returns the parent GameObject, this component belongs to
         */
        Objects::GameObjectPtr getParent() const
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
        const std::string getName() const 
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
            if (m_enabled == enable)
                return;
			m_enabled = enable;
			if (m_enabled)
			{
				onEnable();
			}
			else
			{
				onDisable();
			}
		}

        /**
         * \brief isEnabled
         * \param return true if this component is enabled, otherwise false
         */
        bool isEnabled() const 
        { 
            return m_enabled; 
        }

        /**
         * @brief 
         * Used to draw debug informations of the component.
		 * For example the collider component will draw the bounding box.
         * @param target 
         * @param states 
         */
        virtual void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const;

        

        void deleteLater();
    protected:
        /**
        * \brief setParent
        * \param parent
        *        Assigns this Component to an GameObject.
        *        This will be called by the GameObject::addChild(...),
        *        So you don't have to call this manually.
        */
        virtual void setParent(Objects::GameObjectPtr parent)
        {
            m_parent = parent;
        }

        virtual void setSceneParent(Scene* parent)
        {
            m_sceneParent = parent;
        }

		virtual void onEnable() {}
		virtual void onDisable() {}
        

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
		double getFixedDeltaT() const; // Returns fixed delta Time in seconds

        /**
         * \brief getSceneParent
         * \return returns the Scene, this component belongs to
         */
        Scene* getSceneParent() const
        {
            return m_sceneParent;
        }

        /// Logging
        void log(const Log::Message& msg) const;

        void log(const std::string& msg) const;
        void log(const std::string& msg, Log::Level level) const;
        void log(const std::string& msg, Log::Level level, const Log::Color& col) const;

        void logDebug(const std::string& msg) const;
        void logInfo(const std::string& msg) const;
        void logWarning(const std::string& msg) const;
        void logError(const std::string& msg) const;
        // ---------

        Objects::GameObjectPtr m_parent;
        Scene* m_sceneParent;

    private:
        

        bool m_enabled;
        std::string m_name;
        
};

}
}
