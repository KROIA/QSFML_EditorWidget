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
        enum class EventSequenceElement
        {
            childs,
            components,
            customEventFunctions
        };
        enum class UpdateSequenceElement
        {
            childs,
            components,
			thisUpdate,
            customUpdateFunctions
        };
        enum class DrawSequenceElement
		{
            childs,
			components,
            customDrawFunctions
		};

        GameObject(const std::string &name = "GameObject",
                     GameObject* parent = nullptr);
        GameObject(const GameObject &other);

        static void deleteObject(GameObjectPtr obj);
        

        virtual CLONE_FUNC_DEC(GameObject);
        
        /**
         * @brief 
		 * Sets the object as a child of the parent
		 * This removes the object from the old parent
         * @param parent 
         */
        void setParent(GameObjectPtr parent);
        
        /**
         * @brief 
		 * Gets the parent of the object
         * @return the parent object
         */
        GameObjectPtr getParent() const;
        
        /**
         * @brief 
		 * Gets the most top parent of the object
		 * @return most top parent
         */
        GameObjectPtr getRootParent() const;

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
        bool isEnabled() const;

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
        const std::string &getName() const;

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
        double getBirthTime() const;

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
        size_t getBirthTick() const;

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
		 * @param order 
		 */
		void setEventOrder(const std::vector<EventSequenceElement>& order) { m_eventOrder = order; }

		/**
		 * @brief 
		 * Gets the order in which events are processed by this object
		 * @return event order list
		 */
		const std::vector<EventSequenceElement>& getEventOrder() const { return m_eventOrder; }

		/**
		 * @brief
		 * Sets the order in which the object is updated
		 * @param order
		 */
		void setUpdateOrder(const std::vector<UpdateSequenceElement>& order) { m_updateOrder = order; }

		/**
		 * @brief
		 * Gets the order in which the object is updated
		 * @return update order list
		 */
		const std::vector<UpdateSequenceElement>& getUpdateOrder() const { return m_updateOrder; }

		/**
		 * @brief
		 * Sets the order in which the object is drawn
		 * @param order
		 */
        void setDrawOrder(const std::vector<DrawSequenceElement>& order) { m_drawOrder = order; }

        /**
         * @brief 
		 * Gets the order in which the object is drawn
		 * @return draw order list
         */
        const std::vector<DrawSequenceElement>& getDrawOrder() const { return m_drawOrder; }


        /**
         * @brief 
		 * Adds a custom event function to the object
		 * This function will be called when an event is received
         * The function gets the object.
         * The map contains all events that were received by a specific camera
         * @param func 
         */
        void addEventFunction(const std::function<void(GameObject&, const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>&)>& func) { m_onEventCallbacks.push_back(func); }
        
        /**
         * @brief 
		 * Removes all custom event functions
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
        const std::vector<std::function<void(GameObject&, const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>&)> >& getEventFunctions() const { return m_onEventCallbacks; }


		/**
		 * @brief 
		 * Adds a custom update function to the object
         * This function will be called on update.
		 * The function gets the object as parameter
		 * @param func 
		 */
		void addUpdateFunction(const std::function<void(GameObject&)>& func) { m_onUpdateCallbacks.push_back(func); }
		
        /**
         * @brief 
		 * Removes all custom update functions
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
		const std::vector<std::function<void(GameObject&)>>& getUpdateFunctions() const { return m_onUpdateCallbacks; }
        

		/**
		 * @brief 
		 * Adds a custom draw function to the object
		 * This function will be called on draw.
		 * The function gets the object, the render target and the render states as parameter
		 * @param func 
		 */
		void addDrawFunction(const std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)>& func) { m_onDrawCallbacks.push_back(func); }
		
        /**
         * @brief 
		 * Removes all custom draw functions
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
		const std::vector<std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)> >& getDrawFunctions() const { return m_onDrawCallbacks; }

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
        RenderLayer getRenderLayer() const;

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
        const SceneSettings::UpdateControlls& getUpdateControlls() const;
       
		
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

        
        // Childs operations

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
        void addChilds(const std::vector<GameObjectPtr>& childs);

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
        void removeChilds(const std::vector<GameObjectPtr>& childs);

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
        const std::vector<GameObjectPtr>& getChilds() const { return m_childObjectManagerData.objs; }

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
        std::vector<T*> getChilds() const;

        /**
         * @brief 
		 * Gets all childs of the given type recursive
		 * It scans the whole child hirarchy
		 * @tparam T childtype
		 * @return list of childs of the given type
         */
        template<typename T>
        std::vector<T*> getChildsRecusrive() const;

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
        std::vector<GameObjectPtr> getAllChilds(const std::string& name);

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
        std::vector<GameObjectPtr> getAllChildsRecursive(const std::string& name);

        // ---------

        // Component operations

        /**
         * @brief 
		 * Adds a component to this object
         * @param component 
         */
        void addComponent(Components::ComponentPtr component);

        /**
         * @brief 
		 * Adds multiple components to this object
         * @param components 
         */
        void addComponents(const std::vector<Components::ComponentPtr>& components);

        /**
         * @brief 
		 * Removes a component from this object
         * @param component 
         */
        void removeComponent(Components::ComponentPtr component);

        /**
         * @brief 
		 * Removes multiple components from this object
         * @param components 
         */
        void removeComponents(const std::vector<Components::ComponentPtr>& components);

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
        const std::vector<Components::ComponentPtr>& getComponents() const;
        template <typename T>
        std::vector<T*> getComponents() const;
        template <>
        std::vector<Components::Collider*> getComponents<Components::Collider>() const;
        template <>
        std::vector<Utilities::Updatable*> getComponents<Utilities::Updatable>() const;
        template <>
        std::vector<Components::SfEventHandle*> getComponents<Components::SfEventHandle>() const;
        template <>
        std::vector<Components::Transform*> getComponents<Components::Transform>() const;
        template <typename T>
        std::vector<T*> getComponentsRecursive() const;

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
        size_t getComponentCount() const { return m_componentsManagerData.all.size(); }
        
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
        std::vector<Components::ComponentPtr> getAllComponents(const std::string& name);

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
        std::vector<Components::ComponentPtr> getAllComponentsRecursive(const std::string& name);




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
            std::vector<Utilities::Collisioninfo>& collisions,
            bool onlyFirstCollision) const;

        /**
         * @brief 
		 * Checks for collisions between this and all objects contained in the quadtree
         * @param tree that consists many objects
         * @param collisions that have occured (out parameter)
		 * @param onlyFirstCollision if set to true, the check will stop for each object after the first collision
         */
        void checkCollision(const Utilities::ObjectQuadTree& tree,
            std::vector<Utilities::Collisioninfo>& collisions,
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

        /// <summary>
        /// Gets the time in seconds between each update call
        /// Timedomain: Real simulation time
        /// </summary>
        /// <returns>Delta time</returns>
        double getDeltaT() const;

        /// <summary>
        /// Gets the time in seconds between each update call
        /// Timedomain: Fixed simulation time
        /// </summary>
        /// <returns>Delta time</returns>
        double getFixedDeltaT() const;

        /// <summary>
        /// Gets the time in seconds since the start of the Scene
        /// Timedomain: Real simulation time
        /// </summary>
        /// <returns>Time since start</returns>
        double getElapsedTime() const; 

        /// <summary>
        /// Gets the time in seconds since the start of the Scene
        /// Timedomain: Fixed simulation time
        /// </summary>
        /// <returns>Time since start</returns>
        double getFixedElapsedTime() const; 

        /// <summary>
        /// Gets the Scene this object is contained in
        /// </summary>
        /// <returns>Parent Scene</returns>
        Scene* getSceneParent() const;

        /// <summary>
        /// Searches the object with the given name in the Scene.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr getFirstObjectGlobal(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the Scene
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> getAllObjectsGlobal(const std::string& name);

        /// <summary>
        /// Searches the object with the given name in the Scene
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr getFirstObjectGlobalRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the Scene
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> getAllObjectsGlobalRecusive(const std::string& name);

        /*
        template<typename T>
        T* findFirstObjectGlobal() const
        {
            Scene *scene = getSceneParent();
            if (!scene) return nullptr;
            return scene->findFirstObject<T>();
        }
        template<typename T>
        std::vector<T*> findAllObjectGlobal() const
        {
            Scene* scene = getSceneParent();
            if (!scene) return {};
            return scene->findAllObject<T>();
        }
        template<typename T>
        T* findFirstObjectGlobalRecursive() const
        {
            Scene* scene = getSceneParent();
            if (!scene) return nullptr;
            return scene->findFirstObjectRecursive<T>();
        }
        template<typename T>
        std::vector<T*> findAllObjectsGlobalRecursive() const
        {
            Scene* scene = getSceneParent();
            if (!scene) return {};
            return scene->findAllObjectRecursive<T>();
        }

        template<typename T>
        T* findFirstObjectGlobal(const std::string &objName) const
        {
            Scene* scene = getSceneParent();
            if (!scene) return nullptr;
            return scene->findFirstObject<T>(objName);
        }
        template<typename T>
        std::vector<T*> findAllObjectGlobal(const std::string& objName) const
        {
            Scene* scene = getSceneParent();
            if (!scene) return {};
            return scene->findAllObject<T>(objName);
        }
        template<typename T>
        T* findFirstObjectGlobalRecursive(const std::string& objName) const
        {
            Scene* scene = getSceneParent();
            if (!scene) return nullptr;
            return scene->findFirstObjectRecursive<T>(objName);
        }
        template<typename T>
        std::vector<T*> findAllObjectsGlobalRecursive(const std::string& objName) const
        {
            Scene* scene = getSceneParent();
            if (!scene) return {};
            return scene->findAllObjectRecursive<T>(objName);
        }
        */


        sf::Image captureScreen();

        // ---------

        /// Logging
        void createLogger(); // Creates a new logger for this object
                             // You don't need to call this function unless you want to create a new logger

        void log(const Log::Message& msg) const;

        void log(const std::string& msg) const;
        void log(const std::string& msg, Log::Level level) const;
        void log(const std::string& msg, Log::Level level, const Log::Color& col) const;

        void logDebug(const std::string& msg) const;
        void logInfo(const std::string& msg) const;
        void logWarning(const std::string& msg) const;
        void logError(const std::string& msg) const;

        // ---------

        /// Transform interface
        virtual void setPosition(float x, float y);
        virtual void setPosition(const sf::Vector2f& position);
        virtual void setRotation(float angle);
        virtual void setScale(float factorX, float factorY);
        virtual void setScale(const sf::Vector2f& factors);
        virtual void setOrigin(float x, float y);
        virtual void setOrigin(const sf::Vector2f& origin);
        virtual void move(float offsetX, float offsetY);
        virtual void move(const sf::Vector2f& offset);
        virtual void rotate(float angle);
        virtual void scale(float factorX, float factorY);
        virtual void scale(const sf::Vector2f& factor);
        const sf::Vector2f& getPosition() const;
        sf::Vector2f getGlobalPosition() const; // Gets the position in absolute world coords
        float getRotation() const;
        float getGlobalRotation() const; // Gets the rotation in absolute world coords
        const sf::Vector2f& getScale() const;
        sf::Vector2f getGlobalScale() const;
        const sf::Vector2f& getOrigin() const;
        
        const sf::Transform& getTransform() const;
        const sf::Transform& getInverseTransform() const;
        sf::Transform getGlobalTransform() const;
        // ---------


        // A function can be defined that gets called when ever the bounding box is updated
        // Return a custom bounding box in this function.
        // This box will be added to the other bounding boxes to calculate the final bounding box
        // All coordinates are in world space absolute coordinates
        void setCustomBoundingBoxFunction(const std::function<Utilities::AABB()>& func);
        void resetCustomBoundingBoxFunction();
        
        bool isColliderDirty() const;

        // Called to update the collider geometry
        void updateColliderData() const;

        // Called when new components/childs are added or removed
        void updateObjectChanges();

        void updateBoundingBox() const;

        const static size_t npos = -1;
    protected:
        /**
         * \brief update will be called once per frame
         */
        virtual void update();

        virtual void onAwake();
        virtual void onEnable();
        virtual void onDisable();

     

        virtual void onSceneParentChange(Scene *oldParent, Scene *newParent);
        virtual void onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent);

        virtual void internalOnSceneParentChange(Scene *oldParent, Scene *newParent);
        virtual void internalOnParentChange(GameObjectPtr oldParent, GameObjectPtr newParent);

        

        void deleteLater();

    private:

        std::vector<std::string> toStringInternal(const std::string &preStr) const;
        bool getAllChilds_internal(const std::string& name, std::vector<GameObjectPtr>& foundList);
        bool getAllChildsRecursive_internal(const std::string& name, std::vector<GameObjectPtr>& foundList);
        
        void markTransformDirty();
        sf::Transform updateTransformInternal(sf::Transform parentTransform) const;
		sf::Transform updateTransformInternal()const;
        
        //bool findAllComponentsRecursive_internal(const std::string& name, std::vector<Components::Component*>& foundList);

        //template<typename T>
        //void getChildsRecusrive_internal(std::vector<T*> &listOut) const;


        //void removeChild_internal();
        //void removeComponent_internal();
        //void deleteChild_internal();
        //void addChild_internal();
        //inline void addChild_internal(GameObjectPtr obj);
        void setParent_internal(GameObjectPtr parent,
                                GameObjectPtr rootParent,
                                Scene *SceneParent);
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
       

        
        //std::vector<GameObjectPtr> m_childs;
        //std::vector<Components::Component*> m_componentsManagerData.all;
       

        //std::vector<Utilities::Updatable*> m_updatableComponents;
        //std::vector<Components::Collider*> m_colliders;
        //std::vector<Utilities::Transformable*> m_transformables;
        mutable Utilities::AABB m_boundingBox; 
        //mutable sf::Vector2f m_oldPosition;
        std::function<Utilities::AABB()>* m_getCustomBoundingBoxFunction = nullptr;

        // Will send a signal to the parent to notify, the new status
        void needsEventUpdateChanged(bool needsEventUpdate);
        void needsEventUpdate(bool needsEventUpdate);
        //bool m_thisNeedsEventUpdate;
        //std::vector<Components::SfEventHandle*> m_eventComponents;

        // Will send a signal to the parent to notify, the new status
        void needsDrawUpdateChanged(bool needsDrawUpdate);
        void needsDrawUpdate(bool needsDrawUpdate);
        //bool m_thisNeedsDrawUpdate;
        //std::vector<Components::Drawable*> m_drawableComponents;

        
        //std::vector<GameObjectPtr> m_toAddChilds;
        //std::vector<GameObjectPtr> m_toDeleteChilds;
        //std::vector<GameObjectPtr> m_toRemoveChilds;
        
        //std::vector<Components::Component*> m_toAddComponents;
        //std::vector<Components::Component*> m_componentsManagerData.toRemove;

        SceneSettings::UpdateControlls m_updateControlls;
		std::vector< std::function<void(GameObject&, const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>&)> > m_onEventCallbacks;
		std::vector< std::function<void(GameObject&)> > m_onUpdateCallbacks;
		std::vector< std::function<void(const GameObject&,sf::RenderTarget&,sf::RenderStates)> > m_onDrawCallbacks;

        RenderLayer m_renderLayer;

        // Scene Object Internal functions
        void setSceneParent(Scene *parent);
        
        
        void sfEvent(const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>> &events);
        void update_internal();
        void inSceneAdded_internal();
        //void draw(sf::RenderWindow &window) const;
        void draw(sf::RenderWindow& window, sf::RenderStates states) const;



        // New structure
        void updateChanges_childObjectManager();
        void updateChanges_componentsManager();



        struct ChildObjectManagerData
        {
            std::vector<GameObjectPtr> objs;

            std::vector<GameObjectPtr> toAdd;
            std::vector<GameObjectPtr> toRemove;
            std::vector<GameObjectPtr> toDelete;

            bool objectsChanged;
        };
        struct ComponentManagerData
        {
            std::vector<Components::ComponentPtr> all;

            std::vector<Utilities::Updatable*> updatables;
            std::vector<Components::Collider*> colliders;
            std::vector<Components::SfEventHandle*> eventHandler;
            std::vector<Components::Drawable*> drawable;
            mutable Components::Transform* transform = nullptr;

            std::vector<Components::ComponentPtr> toAdd;
            std::vector<Components::ComponentPtr> toRemove;
            std::vector<Components::ComponentPtr> toDelete;

            std::atomic<bool> thisNeedsDrawUpdate = false;
            std::atomic<bool> thisNeedsEventUpdate = false;

			// Container to hold which are created by this object
			// This is needed to delete the components when the object is deleted
            std::vector<Components::ComponentPtr> selfCreatedComponents;
        };

		ChildObjectManagerData m_childObjectManagerData;
        ComponentManagerData m_componentsManagerData;

		std::vector<EventSequenceElement> m_eventOrder;
		std::vector<UpdateSequenceElement> m_updateOrder;
		std::vector<DrawSequenceElement> m_drawOrder;

        // Static
        static size_t s_objNameCounter;
};




template<typename T>
void GameObject::removeChilds()
{
    m_childObjectManagerData.toRemove.reserve(m_childObjectManagerData.toRemove.size() + m_childObjectManagerData.objs.size());
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (dynamic_cast<T>(obj))
        {
            m_childObjectManagerData.toRemove.push_back(obj);
        }
    }
}

template<typename T>
T* GameObject::getFirstChild() const
{
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            return child;
        }
    }
    return nullptr;
}
template<typename T>
std::vector<T*> GameObject::getChilds() const
{
    std::vector<T*> childs;
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            childs.push_back(child);
        }
    }
    return childs;
}

template<typename T>
std::vector<T*> GameObject::getChildsRecusrive() const
{
    std::vector<T*> childs;
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            childs.push_back(child);
        }
        std::vector<T*> childChilds = obj->getChildsRecusrive<T>();
        childs.insert(childs.end(), childChilds.begin(), childChilds.end());
    }
    return childs;
}


template <typename T>
void GameObject::removeComponents()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.all.size());
    for (auto& comp : m_componentsManagerData.all)
    {
        if (dynamic_cast<T>(comp))
        {
            removeComponent(comp);
        }
    }
}
template <>
void GameObject::removeComponents<Components::Collider>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.colliders.size());
    for (size_t i = 0; i < m_componentsManagerData.colliders.size(); ++i)
    {
        Components::ComponentPtr component = static_cast<Components::Component*>(m_componentsManagerData.colliders[i]);
        removeComponent(component);
    }
}
template <>
void GameObject::removeComponents<Utilities::Updatable>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.updatables.size());
    for (auto& comp : m_componentsManagerData.updatables)
    {
        removeComponent(dynamic_cast<Components::Component*>(comp));
    }
}
template <>
void GameObject::removeComponents<Components::SfEventHandle>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.eventHandler.size());
    for (size_t i = 0; i < m_componentsManagerData.eventHandler.size(); ++i)
    {
        removeComponent(dynamic_cast<Components::Component*>(m_componentsManagerData.eventHandler[i]));
    }
}
template <>
void GameObject::removeComponents<Components::Transform>()
{
    removeComponent(static_cast<Components::Component*>(m_componentsManagerData.transform));
}

template <typename T>
T* GameObject::getFirstComponent() const
{
    for (auto& comp : m_componentsManagerData.all)
    {
        if (T* t = dynamic_cast<T*>(comp))
        {
            return t;
        }
    }
    return nullptr;
}

template <>
Components::Collider* GameObject::getFirstComponent<Components::Collider>() const
{
    if (m_componentsManagerData.colliders.size() > 0)
        return m_componentsManagerData.colliders[0];
    return nullptr;
}
template <>
Utilities::Updatable* GameObject::getFirstComponent<Utilities::Updatable>() const
{
    if (m_componentsManagerData.updatables.size() > 0)
        return m_componentsManagerData.updatables[0];
    return nullptr;
}
template <>
Components::SfEventHandle* GameObject::getFirstComponent<Components::SfEventHandle>() const
{
    if (m_componentsManagerData.eventHandler.size() > 0)
        return m_componentsManagerData.eventHandler[0];
    return nullptr;
}
template <>
Components::Transform* GameObject::getFirstComponent<Components::Transform>() const
{
    return m_componentsManagerData.transform;
}


template <typename T>
std::vector<T*> GameObject::getComponents() const
{
    std::vector<T*> components;
    components.reserve(m_componentsManagerData.all.size());
    for (auto& comp : m_componentsManagerData.all)
    {
        if (T* t = dynamic_cast<T*>(comp))
        {
            components.push_back(t);
        }
    }
    return components;
}
template <>
std::vector<Components::Collider*> GameObject::getComponents<Components::Collider>() const
{
    return m_componentsManagerData.colliders;
}
template <>
std::vector<Utilities::Updatable*> GameObject::getComponents<Utilities::Updatable>() const
{
    return m_componentsManagerData.updatables;
}
template <>
std::vector<Components::SfEventHandle*> GameObject::getComponents<Components::SfEventHandle>() const
{
    return m_componentsManagerData.eventHandler;
}
template <>
std::vector<Components::Transform*> GameObject::getComponents<Components::Transform>() const
{
    return { m_componentsManagerData.transform };
}

template <typename T>
std::vector<T*> GameObject::getComponentsRecursive() const
{
    std::vector<T*> comps = getComponents<T>();
    for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
    {
        std::vector<T*> comps2 = m_childObjectManagerData.objs[i]->getComponentsRecursive<T>();
        comps.insert(comps.end(), comps2.begin(), comps2.end());
    }
    return comps;
}


template<typename T>
size_t GameObject::getComponentCount() const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t count = 0;
    for (auto& comp : m_componentsManagerData.all)
    {
        if (T* t = dynamic_cast<T*>(comp))
        {
            ++count;
        }
    }
    return count;
}
template<typename T>
size_t GameObject::getComponentCountRecusrive() const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t count = 0;
    for (auto& comp : m_componentsManagerData.all)
    {
        if (T* t = dynamic_cast<T*>(comp))
        {
            ++count;
        }
    }
    for (auto& obj : m_childObjectManagerData.objs)
    {
        count += obj->getComponentCountRecusrive<T>();
    }
    return count;
}










}
}
