#pragma once

#include "QSFML_EditorWidget_base.h"
#include "scene/RenderLayer.h"
#include "scene/SceneSettings.h"

#include "utilities/CollisionInfo.h"

#include "utilities/AABB.h"
#include "utilities/Transformable.h"
#include "components/Transform.h"
#include "components/base/Drawable.h"
#include "components/physics/Collider.h"
#include "components/base/SfEventHandle.h"

#include "utilities/Updatable.h"


#include "events/DestroyEvent.h"


#include <vector>
#include <functional>
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace QSFML
{
namespace Objects
{
#define OBJECT_DECL(className) \
    CLONE_FUNC_DEC(className) override;

#define OBJECT_IMPL(className) \
    CLONE_FUNC_IMPL(className)

#define OBJECT_TEMPLATE_IMPL(className) \
    CLONE_FUNC_TEMPLATE_IMPL(className)

/**
 * \brief The GameObject class
 *
 *
 *
 *
 * The simplest implementation for a costum Object looks like this:
 * \code
 * class MyObject : public QSFML::Objects::GameObject
 * {
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              GameObjectPtr parent = nullptr)
 *         : GameObject(name)
 *     {
 *
 *     }
 * };
 * \endcode
 *
 * If you want to add some Components, it will look like this:
 * \code
 * class MyObject : public QSFML::Objects::GameObject
 * {
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              GameObjectPtr parent = nullptr)
 *         : GameObject(name)
 *     {
 *          // Instantiate the components
 *          m_vec1 = new QSFML::Components::DrawableVector("vec1");
 *          m_vec2 = new QSFML::Components::DrawableVector("vec2");
 *
 *          // Set some values which will be needed to display a vector
 *          m_vec1->setStart(sf::Vector2f(0,0));
 *          m_vec1->setEnd(sf::Vector2f(10,30));
 *          m_vec1->setColor(sf::Color::Blue);
 *
 *          // Attach the second vector to the end of the first vector
 *          m_vec2->setStart(m_vec1->getEnd());
 *          m_vec2->setDirection(sf::Vector2f(10,10));
 *          m_vec2->setColor(sf::Color::Red);
 *
 *          // Add the vectors as component to this GameObject
 *          addComponent(m_vec1);
 *          addComponent(m_vec2);
 *
 *          // The Scene will handle the updating of the components
 *          // As long as you don't change the positions of the vectors
 *          // the vectors will stay static
 *
 *          // You can add some movement behavior with the next code example
 *     }
 *
 *     private:
 *     // Create two DrawableVector components as pointers
 *     QSFML::Components::DrawableVector *m_vec1, *m_vec2;
 * };
 * \endcode
 *
 * You want to add some custom behavior for your components?
 * \code
 * class MyObject : public QSFML::Objects::GameObject
 * {
 *      // Declarate a new Component class so its already visible in the constructor
 *      class MouseFollower;
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              GameObjectPtr parent = nullptr)
 *         : GameObject(name)
 *     {
 *          // Instantiate the components
 *          m_vec1 = new QSFML::Components::DrawableVector("vec1");
 *          m_vec2 = new QSFML::Components::DrawableVector("vec2");
 *
 *          // Set some values which will be needed to display a vector
 *          m_vec1->setStart(sf::Vector2f(0,0));
 *          m_vec1->setEnd(sf::Vector2f(10,30));
 *          m_vec1->setColor(sf::Color::Blue);
 *
 *          // Attach the second vector to the end of the first vector
 *          m_vec2->setStart(m_vec1->getEnd());
 *          m_vec2->setDirection(sf::Vector2f(10,10));
 *          m_vec2->setColor(sf::Color::Red);
 *
 *          // Add the vectors as component to this GameObject
 *          addComponent(m_vec1);
 *          addComponent(m_vec2);
 *
 *          // Create the new defined component
 *          MouseFollower *follower = new MouseFollower("MouseFollower");
 *
 *          // set the "parent", so the component has access to the members of this
 *          follower->m_vec = this;
 *          addComponent(follower);
 *     }
 *
 *     // Mark the new component as a frient of this to get access to
 *     // the private members of this.
 *     // This is fine, because we defined the component in this classes private section
 *     // so no other class can instantiate this component.
 *     friend MouseFollower;
 *
 *     private:
 *     // Create two DrawableVector components as pointers
 *     QSFML::Components::DrawableVector *m_vec1, *m_vec2;
 *
 *     // The following will show you how you can declarate and define
 *     // a new component class
 *     // The component inherits from the event handler
 *     class MouseFollower : public QSFML::Components::SfEventHandle
 *     {
 *         public:
 *             MouseFollower(const std::string &name = "MouseFollower")
 *                 : SfEventHandle(name)
 *             {
 *
 *             }
 *
 *             // The received events are processed in this function
 *             void sfEvent(const sf::Event &e) override
 *             {
 *                 switch(e.type)
 *                 {
 *                     case sf::Event::MouseMoved:
 *                     {
 *                         // Get the current mouse position
 *                         sf::Vector2f pos = m_vec->getInWorldSpace(sf::Vector2i(e.mouseMove.x,e.mouseMove.y));
 *
 *                         // Set the end and the start of both vectors to the
 *                         // mouse position
 *                         m_vec->m_vec1->setEnd(pos);
 *                         m_vec->m_vec2->setStart(pos);
 *                     }
 *                 }
 *             }
 *             // A pointer to the parent to access the two member vectors
 *             DrawableVector *m_vec;
 *     };
 * };
 * \endcode
 */
class QSFML_EDITOR_WIDGET_EXPORT GameObject: 
    public Events::DestroyEvent
{
        friend Scene;
        friend GameObjectGroup;
        friend GameObjectContainer;
        friend class Internal::LifetimeChecker;

protected:
        virtual ~GameObject();
    public:
        /**
         * @brief 
		 * The order in which events are processed by this object
         */
        enum class EventSequenceElement
        {
            childs,
            components,
            customEventFunctions
        };
        /**
         * @brief 
		 * The order in which the object is updated
         */
        enum class UpdateSequenceElement
        {
            childs,
            components,
			thisUpdate,
            customUpdateFunctions
        };
        /**
         * @brief 
		 * The order in which the object is drawn
         */
        enum class DrawSequenceElement
		{
            childs,
			components,
			sfDrawables,
            customDrawFunctions
		};

        GameObject(const std::string &name = "GameObject",
                     GameObject* parent = nullptr);
        GameObject(const GameObject &other);

        /**
         * @brief 
         * Safely deletes the given object
         * @param obj 
         */
        static void deleteObject(GameObjectPtr obj);
        

        virtual CLONE_FUNC_DEC(GameObject);
        
        /**
         * @brief 
		 * Gets the parent of the object
         * @return the parent object
         */
        GameObjectPtr getParent() const 
        { return m_parent; }
        
        /**
         * @brief 
		 * Gets the most top parent of the object
		 * @return most top parent
         */
        GameObjectPtr getRootParent() const 
        { return m_rootParent; }

        /**
         * @brief 
		 * Enables/Disables the object
         * @param enable 
         */
        void setEnabled(bool enable);

        /**
         * @brief 
		 * Gets the current enabled state of the object
         * @return true if the object is enabled
         */
        bool isEnabled() const
        { return m_enabled; }

        /**
         * @brief 
		 * Sets the name of the object
         * @param name 
         */
        void setName(const std::string &name);

        /**
         * @brief 
		 * Gets the name of the object
         * @return name of the object
         */
		const std::string& getName() const 
        { return m_name; }

        /**
         * @brief 
		 * Gets the age of the object in seconds
		 * Timedomain: Real simulation time
         * @return age in seconds
         */
        double getAge() const;

        /**
         * @brief 
		 * Gets the time in seconds where the object was added to a Scene
		 * Timedomain: Real simulation time
		 * @return time of birth
         */
		double getBirthTime() const 
        { return m_birthTime; }

        /**
         * @brief 
		 * Gets the age of the object in ticks
		 * @return age in ticks
         */
        size_t getAgeTicks() const;

        /**
         * @brief 
		 * Gets the tick where the object was added to a Scene
		 * @return the tick where the object was added to a Scene
         */
		size_t getBirthTick() const 
        { return m_birthTick; }

        /**
         * @brief 
		 * Gets the age of the object in seconds
		 * Timedomain: Fixed simulation time
		 * @return age in seconds
         */
        double getAgeFixed() const;

		/**
		 * @brief 
         * Sets the order in which events are processed by this object
         * @warning Do not use this function from within a event handler
		 * @param order 
		 */
		void setEventOrder(const QSFML::vector<EventSequenceElement>& order) { m_eventOrder = order; }

		/**
		 * @brief 
		 * Gets the order in which events are processed by this object
		 * @return event order list
		 */
		const QSFML::vector<EventSequenceElement>& getEventOrder() const { return m_eventOrder; }

		/**
		 * @brief
		 * Sets the order in which the object is updated
		 * @warning Do not use this function from within a update function
		 * @param order
		 */
		void setUpdateOrder(const QSFML::vector<UpdateSequenceElement>& order) { m_updateOrder = order; }

		/**
		 * @brief
		 * Gets the order in which the object is updated
		 * @return update order list
		 */
		const QSFML::vector<UpdateSequenceElement>& getUpdateOrder() const { return m_updateOrder; }

		/**
		 * @brief
		 * Sets the order in which the object is drawn
		 * @warning Do not use this function from within a draw function
		 * @param order
		 */
        void setDrawOrder(const QSFML::vector<DrawSequenceElement>& order) { m_drawOrder = order; }

        /**
         * @brief 
		 * Gets the order in which the object is drawn
		 * @return draw order list
         */
        const QSFML::vector<DrawSequenceElement>& getDrawOrder() const { return m_drawOrder; }


        /**
         * @brief 
		 * Adds a custom event function to the object
		 * This function will be called when an event is received
         * The function gets the object.
         * The map contains all events that were received by a specific camera
		 * @warning Do not use this function from within another custom event function
         * @param func 
         */
        void addEventFunction(const std::function<void(GameObject&, const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>&)>& func) { m_onEventCallbacks.push_back(func); }
        
        /**
         * @brief 
		 * Removes all custom event functions
		 * @warning Do not use this function from within another custom event function
         */
        void clearEventFunctions() { m_onEventCallbacks.clear(); }

        /**
         * @brief 
		 * Gets the number of custom event functions
		 * @return the number of custom event functions
         */
        size_t getEventFunctionCount() const { return m_onEventCallbacks.size(); }

        /**
         * @brief 
		 * Gets the custom event functions
		 * @return the custom event functions
         */
        const QSFML::vector<std::function<void(GameObject&, const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>&)> >& getEventFunctions() const { return m_onEventCallbacks; }


		/**
		 * @brief 
		 * Adds a custom update function to the object
         * This function will be called on update.
		 * The function gets the object as parameter
		 * @warning Do not use this function from within another custom update function
		 * @param func 
		 */
        void addUpdateFunction(const std::function<void(GameObject&)>& func);
		
        /**
         * @brief 
		 * Removes all custom update functions
		 * @warning Do not use this function from within another custom update function
         */
        void clearUpdateFunctions() { m_onUpdateCallbacks.clear(); }

		/**
		 * @brief 
		 * Gets the number of custom update functions
		 * @return number of custom update functions
		 */
		size_t getUpdateFunctionCount() const { return m_onUpdateCallbacks.size(); }

		/**
		 * @brief 
		 * Gets the custom update functions
		 * @return list of update functions
		 */
		const QSFML::vector<std::function<void(GameObject&)>>& getUpdateFunctions() const { return m_onUpdateCallbacks; }
        

		/**
		 * @brief 
		 * Adds a custom draw function to the object
		 * This function will be called on draw.
		 * The function gets the object, the render target and the render states as parameter
		 * @warning Do not use this function from within another custom draw function
		 * @param func 
		 */
        void addDrawFunction(const std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)>& func);
		
        /**
         * @brief 
		 * Removes all custom draw functions
		 * @warning Do not use this function from within another custom draw function
         */
        void clearDrawFunctions() { m_onDrawCallbacks.clear(); }

		/**
		 * @brief 
		 * Gets the number of custom draw functions
		 * @return size of custom draw functions
		 */
		size_t getDrawFunctionCount() const { return m_onDrawCallbacks.size(); }

		/**
		 * @brief 
		 * Gets the custom draw functions
		 * @return list of custom draw functions
		 */
		const QSFML::vector<std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)> >& getDrawFunctions() const { return m_onDrawCallbacks; }

        /**
         * @brief 
		 * Sets the render layer of the root object of this
		 * Objects in a higher layer will be drawn on top of objects in a lower layer
         * The renderlayer can only be applyed to the root object.
         * If this function is called from a child, it will change the renderlayer of the root object
         * @param layer 
         */
        void setRenderLayer(RenderLayer layer);

        /**
         * @brief 
		 * Gets the render layer of the root object
         * @return renderlayer of the root object
         */
        RenderLayer getRenderLayer() const
        { return m_rootParent->m_renderLayer; }

        /**
         * @brief 
		 * Sets the update controlls for this object
         * Can be used to enable/disable event,update and draw calls
		 * @param controlls
         */
        void setUpdateControlls(const SceneSettings::UpdateControlls& controlls);

        /**
         * @brief 
		 * Gets the update controlls for this object
         * @return the update controlls for this object
         */
		const SceneSettings::UpdateControlls& getUpdateControlls() const 
        { return m_updateControlls; }
       
		
        /**
         * @brief 
		 * Checks if the boundingBox of this object needs to be updated
         * If so, it will be updated
         * @return the updated boundingBox
         */
        const Utilities::AABB& getBoundingBox() const;

		/**
		 * @brief 
         * Gets the boundingBox without updating it
		 * @return boundingBox
		 */
		const Utilities::AABB& getBoundingBoxNoUpdate() const { return m_boundingBox; }
        
        /**
         * @brief 
         * Stringifies the objects child and component hirarchy
		 * @return stringified object
         */
        std::string toString() const;

//
// 
// 
//      Child operations
// 
//
//


        /**
         * @brief 
		 * Adds a child to this object
         * @param child 
         */
        void addChild(GameObjectPtr child);

        /**
         * @brief 
		 * Adds multiple childs to this object
         * @param childs 
         */
        void addChilds(const QSFML::vector<GameObjectPtr>& childs);

        /**
         * @brief 
		 * Removes a child from this object
         * This does not delete the child
         * @param child 
         */
        void removeChild(GameObjectPtr child);

        /**
         * @brief 
		 * Removes multiple childs from this object
		 * This does not delete the childs
         * @param childs 
         */
        void removeChilds(const QSFML::vector<GameObjectPtr>& childs);

        /**
         * @brief 
         * Removes all childs
		 * This does not delete the childs
         */
        void clearChilds();

		/**
		 * @brief 
         * Removes and deletes the child before the next update loop
		 * The child only gets deleted if this is its parent
		 * @param child which will be deleted
		 */
		void deleteChildLater(GameObjectPtr child);

        /**
         * @brief 
		 * Removes all childs of the given type
         * @tparam T childtype
         */
        template<typename T>
        void removeChilds();

        /**
         * @brief 
         * Checks if the given object is a child of this
         * @param child to search in this
         * @return true uf child is a direct child of this, otherwise false
         */
        bool hasChild(GameObjectPtr child) const;

        /**
         * @brief 
		 * Gets the index of the given child
         * @param child 
         * @return index where the child is stored in this object
         */
        size_t getChildIndex(GameObjectPtr child) const;

        /**
         * @brief 
         * Gets the list of childs
         * @return list of childs
         */
        const QSFML::vector<GameObjectPtr>& getChilds() const { return m_childObjectManagerData.objs; }

		/**
		 * @brief
		 * Gets the first child of the given type
		 * @tparam T childtype
		 * @return first child of the given type
		 */
        template<typename T>
        T* getFirstChild() const;

        /**
         * @brief 
		 * Gets all childs of the given type
         * @tparam T childtype
		 * @return list of childs of the given type
         */
        template<typename T>
        QSFML::vector<T*> getChilds() const;

        /**
         * @brief 
		 * Gets all childs of the given type recursive
		 * It scans the whole child hirarchy
		 * @tparam T childtype
		 * @return list of childs of the given type
         */
        template<typename T>
        QSFML::vector<T*> getChildsRecusrive() const;

        /**
         * @brief 
		 * Searches the first child with the given name
         * @param name of the searched child
		 * @return child with the given name or nullptr if not found
         */
        GameObjectPtr getFirstChild(const std::string& name);

        /**
         * @brief 
		 * Searches all childs with the given name
         * @param name of the searched child
		 * @return list of childs with the given name
         */
        QSFML::vector<GameObjectPtr> getAllChilds(const std::string& name);

        /**
         * @brief 
		 * Searches the first child with the given name recursive
		 * It scans the whole child hirarchy
         * @param name of the searched child
		 * @return child with the given name or nullptr if not found
         */
        GameObjectPtr getFirstChildRecursive(const std::string& name);

        /**
         * @brief 
		 * Searches all childs with the given name recursive
		 * It scans the whole child hirarchy
         * @param name of the searched child
		 * @return list of childs with the given name
         */
        QSFML::vector<GameObjectPtr> getAllChildsRecursive(const std::string& name);

		/**
		 * @brief 
		 * Gets the amount of childs
		 * @return child object count
		 */
		size_t getChildCount() const { return m_childObjectManagerData.objs.size(); }

		/**
		 * @brief 
		 * Gets the amount of childs of the whole object hirarchy
		 * @return child object count of object hirarchy
		 */
		size_t getChildCountRecursive() const;

        // ---------

        // Component operations

        /**
         * @brief 
		 * Adds a Component or sf::Drawable* to this object
         * @param component to add
         */
        template <typename T>
        void addComponent(T* component);

        /**
         * @brief 
		 * Adds multiple components or sf::Drawable* to this object
         * @param sfDrawables 
         */
        template <typename T>
        void addComponents(const QSFML::vector<T*>& components);


        /**
         * @brief 
		 * Removes a component from this object
         * @param component 
         */
        //void removeComponent(Components::ComponentPtr component);

        template <typename T>
        void removeComponent(T* component);

        /**
         * @brief 
		 * Removes multiple sf::Drawable* components from this object
         * @param sfDrawables 
         */
        template <typename T>
        void removeComponents(const QSFML::vector<T*>& components);

        /**
         * @brief 
		 * Removes all components of a given type
         * It will not delete the removed component
         * @tparam T of component to remove
         */
        template <typename T>
        void removeComponents();
        template <>
        void removeComponents<Components::Collider>();
        template <>
        void removeComponents<Utilities::Updatable>();
        template <>
        void removeComponents<Components::SfEventHandle>();
        template <>
        void removeComponents<Components::Transform>();

        /**
         * @brief 
		 * Removes all components
         * It will not delete the removed components
         */
        void clearComponents();

        /**
         * @brief 
		 * Removes and deletes the component before the next update loop
         * The component only gets deleted if this is its parent
         * @param comp that shuld be deleted
         */
        void deleteComponentLater(Components::ComponentPtr comp);

        template <typename T, typename = std::enable_if_t<!std::is_base_of<Components::Component, T>::value>>
        void deleteComponentLater(T sfDrawable);

		/**
		 * @brief
		 * Gets the first component of the given type
		 * @tparam T type of the component
		 * @return component or nullptr if not found
		 */
		template <typename T>
        T* getFirstComponent() const;
        template <>
        Components::Collider* getFirstComponent<Components::Collider>() const;
        template <>
        Utilities::Updatable* getFirstComponent<Utilities::Updatable>() const;
        template <>
        Components::SfEventHandle* getFirstComponent<Components::SfEventHandle>() const;
        template <>
        Components::Transform* getFirstComponent<Components::Transform>() const;

        /**
         * @brief 
		 * Gets all components of the given type
		 * @return list of components with the given type
         */
        const QSFML::vector<Components::ComponentPtr>& getComponents() const;
        template <typename T>
        QSFML::vector<T*> getComponents() const;
        template <>
        QSFML::vector<Components::Collider*> getComponents<Components::Collider>() const;
        template <>
        QSFML::vector<Utilities::Updatable*> getComponents<Utilities::Updatable>() const;
        template <>
        QSFML::vector<Components::SfEventHandle*> getComponents<Components::SfEventHandle>() const;
        template <>
        QSFML::vector<Components::Transform*> getComponents<Components::Transform>() const;
        template <typename T>
        QSFML::vector<T*> getComponentsRecursive() const;

        /**
         * @brief 
		 * Checks if this object has a component with the given name
         * @param name of the component
		 * @return true if this object has a component with the given name, otherwise false
         */
        bool hasComponent(const std::string& name) const;

        /**
         * @brief 
		 * Checks if this object has the given component
         * @param component 
		 * @return true if this object has the given component, otherwise false
         */
        bool hasComponent(Components::ComponentPtr component) const;


        /**
         * @brief 
         * Gets the amount of components
         * @return amount of components
         */
        size_t getComponentCount() const {
            return m_componentsManagerData.all.size() + 
                m_componentsManagerData.sfDrawable.size();
        }
        
        /**
         * @brief 
         * Gets the amount of components of the whole object hirarchy
         * @return amount of components of the whole object hirarchy
         */
        size_t getComponentCountRecursive() const;

        /**
         * @brief 
         * Gets the amount of components of a specific component type
         * @tparam T type of the component
         * @return amount of components that are from type T
         */
        template<typename T>
        size_t getComponentCount() const;

        /**
         * @brief 
         * Gets the amount of components of a specific component type in the whole object hirarchy
         * @tparam T type of the component
         * @return amount of components in the tree that are from type T
         */
        template<typename T>
        size_t getComponentCountRecusrive() const;

        /**
         * @brief 
		 * Gets the first component with the given name
         * @param name of the component
		 * @return first component with the given name or nullptr if not found
         */
        Components::ComponentPtr getFirstComponent(const std::string& name);
        
        /**
         * @brief 
		 * Gets all components with the given name
         * @param name of the component
         * @return list of all components with the given name
         */
        QSFML::vector<Components::ComponentPtr> getAllComponents(const std::string& name);

        /**
         * @brief 
		 * Gets the first component with the given name recursive
		 * It scans the whole object hirarchy
         * @param name of the component
		 * @return component with the given name or nullptr if not found
         */
        Components::ComponentPtr getFirstComponentRecursive(const std::string& name);
        
        /**
         * @brief 
		 * Gets all components with the given name recursive
		 * It scans the whole object hirarchy
         * @param name of the component
		 * @return list of all components with the given name
         */
        QSFML::vector<Components::ComponentPtr> getAllComponentsRecursive(const std::string& name);




        /**
         * @brief 
         * Checks if this object has components of type SfEventHandle
		 * @return true if this object has components of type SfEventHandle, otherwise false
         */
        bool hasEventHandlers() const { return !m_componentsManagerData.eventHandler.empty(); }
        
        /**
         * @brief 
		 * Checks if this object has components of type Collider
		 * @return true if this object has components of type Collider, otherwise false
         */
        bool hasColliders() const { return !m_componentsManagerData.colliders.empty(); }
       
        /**
         * @brief 
		 * Checks if this object has components of type Updatable
		 * @return true if this object has components of type Updatable, otherwise false
         */
        bool hasUpdatables() const { return !m_componentsManagerData.updatables.empty(); }
        
        /**
         * @brief 
         * Checks if this object has transform component
         * @return true if this object has a transform component, otherwise false
         */
        bool hasTransform() const { return m_componentsManagerData.transform != nullptr; }

        /**
         * @brief 
         * Checks for collisions between this and the other object
         * @param other object collision is checked against
         * @return true if a collision occured, otherwise false
         */
        bool checkCollision(const GameObjectPtr other) const;

        /**
         * @brief 
		 * Checks for collisions between this and the other object
         * @param other object collision is checked against
         * @param collisions that have occured (out parameter)
         * @param onlyFirstCollision, if set to true, the check will stop after the first collision 
         * @return true if a collision occured, otherweise false
         */
        bool checkCollision(const GameObjectPtr other,
            QSFML::vector<Utilities::Collisioninfo>& collisions,
            bool onlyFirstCollision) const;

        /**
         * @brief 
		 * Checks for collisions between this and all objects contained in the quadtree
         * @param tree that consists many objects
         * @param collisions that have occured (out parameter)
		 * @param onlyFirstCollision if set to true, the check will stop for each object after the first collision
         */
        void checkCollision(const Utilities::ObjectQuadTree& tree,
            QSFML::vector<Utilities::Collisioninfo>& collisions,
            bool onlyFirstCollision);
        // ---------

        // Scene operations

        /**
         * @brief 
		 * Gets the current mouse position in pixel space of the main camera
         * @return pixel coordinate of the mouse
         */
        sf::Vector2i getMousePosition() const;

        /**
         * @brief 
         * Gets the current mouse position in the world 
		 * @return world coordinate of the mouse
         */
        sf::Vector2f getMouseWorldPosition() const;

        /**
         * @brief 
         * Gets the relative (to this object) mouse position in world coordinates 
         * @return relative mouse position in world coordinates
         */
        sf::Vector2f getMouseObjectPosition() const;

        /**
         * @brief 
		 * Converts a pixel coordinate to a world coordinate
		 * Pixel coordinates are relative to the main camera
         * @param pixelSpace coordinates relative to the main camera
         * @return world space coordinates
         */
        sf::Vector2f getInWorldSpace(const sf::Vector2i& pixelSpace) const;
        
        /**
         * @brief 
		 * Converts a world coordinate to a pixel coordinate
		 * Pixel coordinates are relative to the main camera
         * @param worldSpace coordinates
         * @return pixel coordinates relative to the main camera
         */
        sf::Vector2i getInScreenSpace(const sf::Vector2f& worldSpace) const;

        /**
         * @brief 
         * Gets the view of the main camera
         * @return main camera view
         */
        const sf::View getCameraView() const;

        /**
         * @brief 
         * Gets the default view of the main camera
         * @return default view of the main camera
         */
        const sf::View &getDefaultCameraView() const;

        /**
         * @brief 
         * Gets the view rect of the main camera
         * @return view rect of the main camera
         */
        Utilities::AABB getCameraViewRect() const;

        /**
         * @brief 
         * Gets the main camera size
         * @return main camera size
         */
        sf::Vector2u getCameraSize() const;

        /**
         * @brief 
         * Gets the main camera size before a change in size was detected
		 * @return main camera size before a change in size was detected
         */
        sf::Vector2u getOldCameraSize() const;

		/**
		 * @brief
		 * Gets the default camera of the scene
		 * @return default camera or nullptr if object is not in a scene
         */
        CameraWindow* getDefaultCamera() const;

        /**
         * @brief 
		 * Gets the camera that is currently used for rendering
		 * @return current render camera, or nullptr if object is not in rendering step
         */
        CameraWindow* getCurrentRenderCamera() const;

        /**
         * @brief 
		 * Gets the default font for text rendering
         * @return default font
         */
        const sf::Font& getDefaultTextFont() const;

        /**
         * @brief 
         * Gets the current tick count of the scene
         * @return scene tick count
         */
        size_t getTick() const;

        /**
         * @brief 
		 * Gets the time in seconds between each update call
		 * Timedomain: Real simulation time
		 * @return time in seconds since the last upfate call
         */
        double getDeltaT() const;

        /**
         * @brief 
         * Gets a fixed delta time
		 * Timedomain: Fixed simulation time
         * @return 
         */
        double getFixedDeltaT() const;
        
        /**
         * @brief 
		 * Gets the time in seconds since the start of the Scene
		 * @return time in seconds since the start of the Scene
         */
        double getElapsedTime() const; 
        
        /**
         * @brief 
		 * Gets the fixed time in seconds since the start of the Scene
		 * @return fixed time in seconds since the start of the Scene
         */
        double getFixedElapsedTime() const; 

        /**
         * @brief 
		 * Gets the scene the object is in
		 * @return the scene the object is in, or nullptr if the object is not in a scene
         */
        Scene* getSceneParent() const;
   
        /**
         * @brief 
		 * Searches the object with the given name in the Scene
		 * Searches only the top layer objects
		 * @param name of the object
		 * @return object with the given name or nullptr if not found
         */
        Objects::GameObjectPtr getFirstObjectGlobal(const std::string& name);

        /**
         * @brief 
		 * Searches all objects with the given name in the Scene
		 * Searches only the top layer objects
		 * @param name of the object
		 * @return list of found objects
         */
        QSFML::vector<Objects::GameObjectPtr> getAllObjectsGlobal(const std::string& name);
        
        /**
         * @brief 
		 * Searches the object with the given name in the Scene
		 * Searches all object tree hirarchies
		 * @param name of the object
		 * @return first object with the given name or nullptr if not found
         */
        Objects::GameObjectPtr getFirstObjectGlobalRecursive(const std::string& name);

        /**
         * @brief 
		 * Searches all objects with the given name in the Scene
		 * Searches all object tree hirarchies
		 * @param name of the object
		 * @return list of found objects
         */
        QSFML::vector<Objects::GameObjectPtr> getAllObjectsGlobalRecusive(const std::string& name);

        /**
         * @brief 
		 * Creates a screenshot of the current camera
		 * @return A screenshot of the current camera
         */
        sf::Image captureScreen();

        // ---------

        /// Logging

        /**
         * @brief 
		 * Creates a new logger for this object
         * You don't need to call this function before you want to use the logger
         * If you call this function, every message will be logged in the context of this objects name
         */
        void createLogger(); 
                             
        /**
         * @brief 
		 * Logs a message to the console
         * @param msg 
         */
        void log(const Log::Message& msg) const;
        void log(const std::string& msg) const;
        void log(const std::string& msg, Log::Level level) const;
        void log(const std::string& msg, Log::Level level, const Log::Color& col) const;

        void logDebug(const std::string& msg) const;
        void logInfo(const std::string& msg) const;
        void logWarning(const std::string& msg) const;
        void logError(const std::string& msg) const;

        // ---------

        // Transform interface

        /**
         * @brief 
		 * Sets the position of the object
		 * @param x coordinate in wolrd space
		 * @param y coordinate in world space
         */
        virtual void setPosition(float x, float y);

        /**
         * @brief 
		 * Sets the position of the object
		 * @param position in world space
         */
        virtual void setPosition(const sf::Vector2f& position);

        /**
         * @brief 
		 * Sets the rotation of the object
		 * @param angle in degrees
         */
        virtual void setRotation(float angle);

        /**
         * @brief 
		 * Sets the scale of the object
		 * @param factorX to scale in x direction
		 * @param factorY to scale in y direction
         */
        virtual void setScale(float factorX, float factorY);

        /**
         * @brief 
		 * Sets the scale of the object
         * @param factors 
         */
        virtual void setScale(const sf::Vector2f& factors);

        /**
         * @brief 
		 * Sets the origin of the object
		 * @param x coordinates in world space
		 * @param y coordinates in world space
         */
        virtual void setOrigin(float x, float y);

        /**
         * @brief 
		 * Sets the origin of the object
		 * @param origin in world space
         */
        virtual void setOrigin(const sf::Vector2f& origin);

        /**
         * @brief 
		 * Moves the object by the given offset
		 * @param offsetX in world space
		 * @param offsetY in world space
         */
        virtual void move(float offsetX, float offsetY);

        /**
         * @brief 
		 * Moves the object by the given offset
		 * @param offset in world space
         */
        virtual void move(const sf::Vector2f& offset);

        /**
         * @brief 
		 * Rotates the object by the given angle
         * @param angle in degrees
         */
        virtual void rotate(float angle);

        /**
         * @brief 
		 * Scales the object by the given factors
		 * @param factorX in x direction
		 * @param factorY in y direction
         */
        virtual void scale(float factorX, float factorY);

        /**
         * @brief 
		 * Scales the object by the given factors
         * @param factor 
         */
        virtual void scale(const sf::Vector2f& factor);

        /**
         * @brief 
		 * Gets the position of the object
		 * @return object local position 
         */
        const sf::Vector2f& getPosition() const;

        /**
         * @brief 
		 * Gets the position of the object in absolute world coords
		 * @return object position in absolute world coords
         */
        sf::Vector2f getGlobalPosition() const;

        /**
         * @brief 
		 * Gets the rotation of the object
		 * @return local rotation of the object
         */
        float getRotation() const;

        /**
         * @brief 
		 * Gets the rotation of the object in absolute world coords
		 * @return objects absolute rotation
         */
        float getGlobalRotation() const; 

        /**
         * @brief 
		 * Gets the scale of the object
		 * @return local scale of the object
         */
        const sf::Vector2f& getScale() const;

        /**
         * @brief 
		 * Gets the scale of the object in absolute world coords
		 * @return objects absolute scale
         */
        sf::Vector2f getGlobalScale() const;

        /**
         * @brief 
		 * Gets the origin of the object
		 * @return origin of the object
         */
        const sf::Vector2f& getOrigin() const;
        
        /**
         * @brief 
		 * Gets the transform of this object
		 * @return transform of this object
         */
        const sf::Transform& getTransform() const;

        /**
         * @brief 
		 * Gets the inverse transform of this object
		 * @return the inverse transform of this object
         */
        const sf::Transform& getInverseTransform() const;

        /**
         * @brief 
		 * Gets the global transform of this object
		 * All transforamtions of the parent objects are applied
		 * @return global transform of this object
         */
        sf::Transform getGlobalTransform() const;

        // ---------



        /**
         * @brief 
		 * Sets a custom bounding box function
         * @details
		 * A function can be defined that gets called when ever the bounding box is updated
		 * Return a custom bounding box in this function.
		 * This box will be added to the other bounding boxes to calculate the final bounding box
		 * All coordinates are in world space absolute coordinates
         * @param func 
         */
        void setCustomBoundingBoxFunction(const std::function<Utilities::AABB()>& func);

        /**
         * @brief 
		 * Removes the custom bounding box function
         */
        void removeCustomBoundingBoxFunction();
        
        /**
         * @brief 
		 * Checks if any collider is not up to date
		 * @return true if any collider is not up to date, otherwise false
         */
        bool isColliderDirty() const;

        /**
         * @brief 
         * If any collider is not up to date, this function must be called before 
         * collisions can be checked
         */
        void updateColliderData() const;

        /**
         * @brief 
         * Applys changes in components and childs of this object.
		 * @details
         * If a component or child was added or removed, this function must be called
		 * When the scene is running, this function is called automatically
		 * If you want to access components or childs by a function like getComponents() or getChilds(),
		 * you should call this function before.
		 * Without calling this function, the added or removed components or childs are not available
         */
        void updateObjectChanges();

        /**
         * @brief 
		 * Recalculates the bounding box of this object
         */
        void updateBoundingBox() const;

        /**
         * @brief 
		 * Adds a child GameObject/Component or a sf::Drawable* to this object
		 * @see addComponent
		 * @see addChild
         * @param sfDrawable 
         */
        template <typename T>
        void add(T* item);

		

        /**
         * @brief 
		 * Removes a child GameObject/Component or a sf::Drawable* from this object
		 * @see removeComponent
		 * @see removeChild
         * @param sfDrawable 
         */
        template <typename T>
        void remove(T* item);

        /**
         * @brief 
		 * Removes and deletes the child GameObject/Component or a sf::Drawable* before the next update loop
		 * @see deleteComponentLater
		 * @see deleteChildLater
         * @param sfDrawable 
         */
        template <typename T>
        void deleteLater(T* item);

        /**
         * @brief 
		 * Deletes the object before the next update loop
         */
        void deleteLater();

		const static size_t npos = -1; ///< Represents an invalid index
    protected:
        /**
         * @brief 
         * Will be called once per frame
		 * Override this function to implement custom update logic
         */
        virtual void update();

        /**
         * @brief 
		 * Will be called once when the object is added to a scene
		 * Override this function to implement custom initialization logic
         */
        virtual void onAwake();

        /**
         * @brief 
		 * Will be called when the object gets enabled
		 * Override this function to implement custom logic
         */
        virtual void onEnable();

        /**
         * @brief 
		 * Will be called when the object gets disabled
		 * Override this function to implement custom logic
         */
        virtual void onDisable();

        /**
         * @brief 
         * Will be called when the object changes to another scene
         * @param oldParent 
         * @param newParent 
         */
        virtual void onSceneParentChange(Scene *oldParent, Scene *newParent);

		/**
		 * @brief
		 * Will be called when the object changes its parent
		 * @param oldParent
		 * @param newParent
		 */
        virtual void onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent);
    private:

        QSFML::vector<std::string> toStringInternal(const std::string &preStr) const;
        bool getAllChilds_internal(const std::string& name, QSFML::vector<GameObjectPtr>& foundList);
        bool getAllChildsRecursive_internal(const std::string& name, QSFML::vector<GameObjectPtr>& foundList);
        
        void markTransformDirty();
        sf::Transform updateTransformInternal(sf::Transform parentTransform) const;
		sf::Transform updateTransformInternal()const;
        
        //bool findAllComponentsRecursive_internal(const std::string& name, QSFML::vector<Components::Component*>& foundList);

        //template<typename T>
        //void getChildsRecusrive_internal(QSFML::vector<T*> &listOut) const;


        //void removeChild_internal();
        //void removeComponent_internal();
        //void deleteChild_internal();
        //void addChild_internal();
        //inline void addChild_internal(GameObjectPtr obj);
        void setParent_internal(GameObjectPtr parent,
                                GameObjectPtr rootParent,
                                Scene *SceneParent);
		void setLogParent_internal(Log::LogObject* parent);
        //void addComponent_internal();
        void onObjectsChanged();
        bool objectsChanged() const { return m_childObjectManagerData.objectsChanged; }

        

        bool m_enabled;
        std::string m_name;
        //sf::Vector2f m_position;
        double m_birthTime; // Time of the Scene, in seconds where the object was added to a Scene (Time domain: real simulation time)
        size_t m_birthTick; // Tick of the Scene, where the object was added to a Scene

        // Hirarchy
        Scene *m_sceneParent;
        GameObject* m_parent;
        GameObject* m_rootParent;
        Log::LogObject* m_logObject = nullptr;
        Log::LogObject* m_selfOwnedLogObject = nullptr;
       

        
        //QSFML::vector<GameObjectPtr> m_childs;
        //QSFML::vector<Components::Component*> m_componentsManagerData.all;
       

        //QSFML::vector<Utilities::Updatable*> m_updatableComponents;
        //QSFML::vector<Components::Collider*> m_colliders;
        //QSFML::vector<Utilities::Transformable*> m_transformables;
        mutable Utilities::AABB m_boundingBox; 
        //mutable sf::Vector2f m_oldPosition;
        std::function<Utilities::AABB()>* m_getCustomBoundingBoxFunction = nullptr;

        // Will send a signal to the parent to notify, the new status
        void needsEventUpdateChanged(bool needsEventUpdate);
        void needsEventUpdate(bool needsEventUpdate);
        //bool m_thisNeedsEventUpdate;
        //QSFML::vector<Components::SfEventHandle*> m_eventComponents;

        // Will send a signal to the parent to notify, the new status
        void needsDrawUpdateChanged(bool needsDrawUpdate);
        void needsDrawUpdate(bool needsDrawUpdate);
        //bool m_thisNeedsDrawUpdate;
        //QSFML::vector<Components::Drawable*> m_drawableComponents;

        
        //QSFML::vector<GameObjectPtr> m_toAddChilds;
        //QSFML::vector<GameObjectPtr> m_toDeleteChilds;
        //QSFML::vector<GameObjectPtr> m_toRemoveChilds;
        
        //QSFML::vector<Components::Component*> m_toAddComponents;
        //QSFML::vector<Components::Component*> m_componentsManagerData.toRemove;

        SceneSettings::UpdateControlls m_updateControlls;
		QSFML::vector< std::function<void(GameObject&, const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>&)> > m_onEventCallbacks;
		QSFML::vector< std::function<void(GameObject&)> > m_onUpdateCallbacks;
		QSFML::vector< std::function<void(const GameObject&,sf::RenderTarget&,sf::RenderStates)> > m_onDrawCallbacks;

        RenderLayer m_renderLayer;

        // Scene Object Internal functions
        void setSceneParent(Scene *parent);
        
        
        void sfEvent(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>> &events);
        void update_internal();
        void inSceneAdded_internal();
        //void draw(sf::RenderWindow &window) const;
        void draw(sf::RenderWindow& window, sf::RenderStates states) const;



        // New structure
        void updateChanges_childObjectManager();
        void updateChanges_componentsManager();



        struct ChildObjectManagerData
        {
            QSFML::vector<GameObjectPtr> objs;

            QSFML::vector<GameObjectPtr> toAdd;
            QSFML::vector<GameObjectPtr> toRemove;
            QSFML::vector<GameObjectPtr> toDelete;

            bool objectsChanged;
        };
        struct ComponentManagerData
        {
            QSFML::vector<Components::ComponentPtr> all;

            QSFML::vector<Utilities::Updatable*> updatables;
            QSFML::vector<Components::Collider*> colliders;
            QSFML::vector<Components::SfEventHandle*> eventHandler;
            QSFML::vector<Components::Drawable*> drawable;
            QSFML::vector<sf::Drawable*> sfDrawable;
            mutable Components::Transform* transform = nullptr;

            QSFML::vector<Components::ComponentPtr> toAdd;
            QSFML::vector<sf::Drawable*> sfDrawableToAdd;
            QSFML::vector<Components::ComponentPtr> toRemove;
            QSFML::vector<sf::Drawable*> sfDrawableToRemove;
            QSFML::vector<Components::ComponentPtr> toDelete;
            QSFML::vector<sf::Drawable*> sfDrawableToDelete;

            std::atomic<bool> thisNeedsDrawUpdate = false;
            std::atomic<bool> thisNeedsEventUpdate = false;

			// Container to hold which are created by this object
			// This is needed to delete the components when the object is deleted
            //QSFML::vector<Components::ComponentPtr> selfCreatedComponents;
        };

		ChildObjectManagerData m_childObjectManagerData;
        ComponentManagerData m_componentsManagerData;

		QSFML::vector<EventSequenceElement> m_eventOrder;
		QSFML::vector<UpdateSequenceElement> m_updateOrder;
		QSFML::vector<DrawSequenceElement> m_drawOrder;

        // Static
        static size_t s_objNameCounter;
};









}
}

#include "GameObject.inl"


