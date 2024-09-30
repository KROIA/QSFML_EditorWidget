#pragma once

#include "QSFML_EditorWidget_base.h"
#include "Scene/RenderLayer.h"
#include "Scene/SceneSettings.h"

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
    public Utilities::Transformable, 
    public Utilities::Updatable, 
    public Events::DestroyEvent
{
        friend Scene;
        friend GameObjectGroup;
        friend GameObjectContainer;
    public:
        GameObject(const std::string &name = "GameObject",
                     GameObject* parent = nullptr);
        GameObject(const GameObject &other);
        virtual ~GameObject();

        virtual CLONE_FUNC_DEC(GameObject);

        /// <summary>
        /// Sets the object as a child of the parent
        /// This removes the object from the old parent
        /// </summary>
        /// <param name="parent">new parent</param>
        void setParent(GameObjectPtr parent);

        /// <summary>
        /// Gets the parent of the object
        /// </summary>
        /// <returns>Parent object</returns>
        GameObjectPtr getParent() const;

        /// <summary>
        /// Gets the root parent of the object
        /// The root is the topmost parent in the hirarchy
        /// </summary>
        /// <returns>Root object</returns>
        GameObjectPtr getRootParent() const;

        /// <summary>
        /// Enables/Disables the object
        /// If the object is disabled, it will not be updated or drawn
        /// and no events get received
        /// </summary>
        /// <param name="enable">enable=true, disable=false</param>
        void setEnabled(bool enable);

        /// <summary>
        /// Gets the current enabled state of the object
        /// </summary>
        /// <returns>true if the object is enabled</returns>
        bool isEnabled() const;

        /// <summary>
        /// Sets the name of the object
        /// </summary>
        /// <param name="name">name text</param>
        void setName(const std::string &name);

        /// <summary>
        /// Gets the name of the object
        /// </summary>
        /// <returns>name text</returns>
        const std::string &getName() const;

        /// <summary>
        /// Gets the age of the object in seconds
        /// Timedomain: Real simulation time
        /// </summary>
        /// <returns>Age</returns>
        double getAge() const;

        /// <summary>
        /// Gets the time in seconds where the object was added to a Scene
        /// Timedomain: Real simulation time
        /// </summary>
        /// <returns>Time of birth</returns>
        double getBirthTime() const;

        /// <summary>
        /// Gets the age of the object in ticks
        /// </summary>
        /// <returns>Age in ticks</returns>
        size_t getAgeTicks() const;

        /// <summary>
        /// Gets the tick where the object was added to a Scene
        /// </summary>
        /// <returns>Birth tick</returns>
        size_t getBirthTick() const;

        /// <summary>
        /// Gets the age of the object in seconds
        /// Timedomain: Fixed simulation time
        /// </summary>
        /// <returns>Age in fixed timedomain</returns>
        double getAgeFixed() const;



        //void setPositionRelative(const sf::Vector2f& pos); // Sets the position relative to its parent
        //void setPosition(const sf::Vector2f& pos); // Sets the position in the absolute world coords.
        //const sf::Vector2f& getPositionRelative() const;   // Gets the position relative to its parent
        //sf::Vector2f getPosition() const;          // Gets the position in absolute world coords
        sf::Vector2f getGlobalPosition() const; // Gets the position in absolute world coords

        float getGlobalRotation() const; // Gets the rotation in absolute world coords


        void setRenderLayer(RenderLayer layer);
        RenderLayer getRenderLayer() const;

        const SceneSettings::UpdateControlls& getUpdateControlls() const;
        void setUpdateControlls(const SceneSettings::UpdateControlls& controlls);

        const Utilities::AABB &getBoundingBox() const;
        void updateBoundingBox() const;

        std::string toString() const;

        
        // Childs operations
        void addChild(GameObjectPtr child);
        void addChilds(const std::vector<GameObjectPtr>& childs);

        void removeChild(GameObjectPtr child);
        void removeChilds(const std::vector<GameObjectPtr>& childs);
        void clearChilds();
        template<typename T>
        void removeChilds();
        template<typename T>
        void GameObject::removeChilds()
        {
            m_toRemoveChildObjects.reserve(m_toRemoveChildObjects.size() + m_childObjectManagerData.objs.size());
            for (auto& obj : m_childObjectManagerData.objs)
            {
                if (dynamic_cast<T>(obj))
                {
                    m_toRemoveChildObjects.push_back(obj);
                }
            }
        }



        

        bool hasChild(GameObjectPtr child) const;
        size_t getChildIndex(GameObjectPtr child) const;
        const std::vector<GameObjectPtr>& getChilds() const { return m_childObjectManagerData.objs; }

        template<typename T>
        std::vector<T*> getChilds() const
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
        std::vector<T*> getChildsRecusrive() const
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

        //size_t getChildCount() const;
        //
        //template<typename T>
        //size_t getChildCount() const;
        //
        //template<typename T>
        //size_t getChildCountRecusrive() const;

        GameObjectPtr findFirstChild(const std::string& name);
        std::vector<GameObjectPtr> findAllChilds(const std::string& name);

        GameObjectPtr findFirstChildRecursive(const std::string& name);
        std::vector<GameObjectPtr> findAllChildsRecursive(const std::string& name);

        // ---------

        // Component operations
        void addComponent(Components::ComponentPtr component);
        void addComponents(const std::vector<Components::ComponentPtr>& components);

        void createTransform();

        void removeComponent(Components::ComponentPtr component);
        void removeComponents(const std::vector<Components::ComponentPtr>& components);
        template <typename T>
        void removeComponents()
        {
            m_toRemoveComponents.reserve(m_toRemoveComponents.size() + m_componentsManagerData.all.size());
            for (auto& comp : m_componentsManagerData.all)
            {
                if (dynamic_cast<T>(comp))
                {
                    removeComponent(comp);
                }
            }
        }
        template <>
        void removeComponents<Components::Collider>()
        {
            m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.colliders.size());
            for (size_t i = 0; i < m_componentsManagerData.colliders.size(); ++i)
            {
                Components::ComponentPtr component = static_cast<Components::Component*>(m_componentsManagerData.colliders[i]);
                removeComponent(component);
            }
        }
        template <>
        void removeComponents<Utilities::Updatable>()
        {
            m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.updatables.size());
            for (auto& comp : m_componentsManagerData.updatables)
            {
                removeComponent(dynamic_cast<Components::Component*>(comp));
            }
        }
        template <>
        void removeComponents<Components::SfEventHandle>()
        {
            m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.eventHandler.size());
            for (size_t i = 0; i < m_componentsManagerData.eventHandler.size(); ++i)
            {
                removeComponent(dynamic_cast<Components::Component*>(m_componentsManagerData.eventHandler[i]));
            }
        }
        template <>
        void removeComponents<Components::Transform>()
        {
            removeComponent(static_cast<Components::Component*>(m_componentsManagerData.transform));
        }

        void clearComponents();
        void deleteComponentLater(Components::ComponentPtr component);

        Components::ComponentPtr getComponent(const std::string& name) const;
        const std::vector<Components::ComponentPtr>& getComponents() const;
        template <typename T>
        std::vector<T*> getComponents() const
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
        std::vector<Components::Collider*> getComponents<Components::Collider>() const
        {
            return m_componentsManagerData.colliders;
        }
        template <>
        std::vector<Utilities::Updatable*> getComponents<Utilities::Updatable>() const
        {
            return m_componentsManagerData.updatables;
        }
        template <>
        std::vector<Components::SfEventHandle*> getComponents<Components::SfEventHandle>() const
        {
            return m_componentsManagerData.eventHandler;
        }
        template <>
        std::vector<Components::Transform*> getComponents<Components::Transform>() const
        {
            return { m_componentsManagerData.transform };
        }

        bool hasComponent(const std::string& name) const;
        bool hasComponent(Components::ComponentPtr component) const;



        size_t getComponentCount() const { return m_componentsManagerData.all.size(); }
        size_t getComponentCountRecursive() const;
        template<typename T>
        size_t getComponentCount() const
        {
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
        size_t getComponentCountRecusrive() const
        {
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

        Components::ComponentPtr findFirstComponent(const std::string& name);
        std::vector<Components::ComponentPtr> findAllComponents(const std::string& name);

        Components::ComponentPtr findFirstComponentRecursive(const std::string& name);
        std::vector<Components::ComponentPtr> findAllComponentsRecursive(const std::string& name);





        bool hasEventHandlers() const { return !m_componentsManagerData.eventHandler.empty(); }
        bool hasColliders() const { return !m_componentsManagerData.colliders.empty(); }
        bool hasUpdatables() const { return !m_componentsManagerData.updatables.empty(); }
        bool hasTransform() const { return m_componentsManagerData.transform != nullptr; }

        bool checkCollision(const GameObjectPtr other) const;
        bool checkCollision(const GameObjectPtr other,
            std::vector<Utilities::Collisioninfo>& collisions,
            bool onlyFirstCollision) const;
        void checkCollision(const Utilities::ObjectQuadTree& tree,
            std::vector<Utilities::Collisioninfo>& collisions,
            bool onlyFirstCollision);
        // ---------

        // Scene operations

        /// <summary>
        /// Gets the pixel coordinate of the mouse
        /// </summary>
        /// <returns>pixel pos</returns>
        sf::Vector2i getMousePosition() const;

        /// <summary>
        /// Gets the world coordinate of the mouse
        /// </summary>
        /// <returns>world mouse pos</returns>
        sf::Vector2f getMouseWorldPosition() const;

        /// <summary>
        /// Gets the world coordinate of the mouse, relative to this object
        /// </summary>
        /// <returns>relative mouse pos</returns>
        sf::Vector2f getMouseObjectPosition() const;


        sf::Vector2f getInWorldSpace(const sf::Vector2i& pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f& worldSpace) const;

        
        const sf::View getCameraView() const;
        const sf::View &getDefaultCameraView() const;
        Utilities::AABB getCameraViewRect() const;
        sf::Vector2u getSceneSize() const;
        sf::Vector2u getOldSceneSize() const;

        const sf::Font& getDefaultTextFont() const;

        /// <summary>
        /// Gets the current Scene tick count
        /// </summary>
        /// <returns>current tick</returns>
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
        Objects::GameObjectPtr findFirstObjectGlobal(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the Scene
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> findAllObjectsGlobal(const std::string& name);

        /// <summary>
        /// Searches the object with the given name in the Scene
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr findFirstObjectGlobalRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the Scene
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> findAllObjectsGlobalRecusive(const std::string& name);
        // ---------

        
        

        const static size_t npos = -1;
    protected:
        /**
         * \brief update will be called once per frame
         */
        void update() override;

        virtual void inSceneAdded();



        virtual void onSceneParentChange(Scene *oldParent, Scene *newParent);
        virtual void onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent);

        virtual void internalOnSceneParentChange(Scene *oldParent, Scene *newParent);
        virtual void internalOnParentChange(GameObjectPtr oldParent, GameObjectPtr newParent);

        

        void deleteLater();

    private:
        std::vector<std::string> toStringInternal(const std::string &preStr) const;
        bool findAllChilds_internal(const std::string& name, std::vector<GameObjectPtr>& foundList);
        bool findAllChildsRecursive_internal(const std::string& name, std::vector<GameObjectPtr>& foundList);
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
        Scene *m_SceneParent;
        GameObject* m_parent;
        GameObject* m_rootParent;

       

        
        //std::vector<GameObjectPtr> m_childs;
        //std::vector<Components::Component*> m_componentsManagerData.all;
       

        //std::vector<Utilities::Updatable*> m_updatableComponents;
        //std::vector<Components::Collider*> m_colliders;
        //std::vector<Utilities::Transformable*> m_transformables;
        mutable Utilities::AABB m_boundingBox; // <! Todo: Update the bounding box if the object changes
        mutable sf::Vector2f m_oldPosition;

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
        //std::vector<Components::Component*> m_toRemoveComponents;

        SceneSettings::UpdateControlls m_updateControlls;

        RenderLayer m_renderLayer;

        // Scene Object Internal functions
        void setSceneParent(Scene *parent);
        
        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
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

            bool objectsChanged;
        };
        struct ComponentManagerData
        {
            std::vector<Components::ComponentPtr> all;

            std::vector<Utilities::Updatable*> updatables;
            std::vector<Components::Collider*> colliders;
            std::vector<Components::SfEventHandle*> eventHandler;
            std::vector<Components::Drawable*> drawable;
            Components::Transform* transform;

            std::vector<Components::ComponentPtr> toAdd;
            std::vector<Components::ComponentPtr> toRemove;
            std::vector<Components::ComponentPtr> toDelete;

            bool thisNeedsDrawUpdate;
            bool thisNeedsEventUpdate;
        };

		ChildObjectManagerData m_childObjectManagerData;
        ComponentManagerData m_componentsManagerData;

        // Static
        static size_t s_objNameCounter;
};




















}
}
