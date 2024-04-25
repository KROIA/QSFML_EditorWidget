#pragma once

#include "QSFML_EditorWidget_base.h"
#include "canvas/RenderLayer.h"
#include "canvas/CanvasSettings.h"

#include "utilities/CollisionInfo.h"
#include "utilities/Updatable.h"
#include "utilities/AABB.h"
#include "utilities/Transformable.h"
#include "components/base/Drawable.h"

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
 * \brief The CanvasObject class
 *
 *
 *
 *
 * The simplest implementation for a costum Object looks like this:
 * \code
 * class MyObject : public QSFML::Objects::CanvasObject
 * {
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              CanvasObject *parent = nullptr)
 *         : CanvasObject(name)
 *     {
 *
 *     }
 * };
 * \endcode
 *
 * If you want to add some Components, it will look like this:
 * \code
 * class MyObject : public QSFML::Objects::CanvasObject
 * {
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              CanvasObject *parent = nullptr)
 *         : CanvasObject(name)
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
 *          // Add the vectors as component to this CanvasObject
 *          addComponent(m_vec1);
 *          addComponent(m_vec2);
 *
 *          // The Canvas will handle the updating of the components
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
 * class MyObject : public QSFML::Objects::CanvasObject
 * {
 *      // Declarate a new Component class so its already visible in the constructor
 *      class MouseFollower;
 *     public:
 *     MyObject(const std::string &name = "MyObject",
 *              CanvasObject *parent = nullptr)
 *         : CanvasObject(name)
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
 *          // Add the vectors as component to this CanvasObject
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

class QSFML_EDITOR_WIDGET_EXPORT CanvasObject: 
    public Utilities::Transformable, 
    public Utilities::Updatable, 
    public Events::DestroyEvent
{
        friend Canvas;
        friend CanvasObjectGroup;
        friend CanvasObjectContainer;
    public:
        CanvasObject(const std::string &name = "CanvasObject",
                     CanvasObject *parent = nullptr);
        CanvasObject(const CanvasObject &other);
        virtual ~CanvasObject();

        virtual CLONE_FUNC_DEC(CanvasObject);

        /// <summary>
        /// Sets the object as a child of the parent
        /// This removes the object from the old parent
        /// </summary>
        /// <param name="parent">new parent</param>
        void setParent(CanvasObject *parent);

        /// <summary>
        /// Gets the parent of the object
        /// </summary>
        /// <returns>Parent object</returns>
        CanvasObject *getParent() const;

        /// <summary>
        /// Gets the root parent of the object
        /// The root is the topmost parent in the hirarchy
        /// </summary>
        /// <returns>Root object</returns>
        CanvasObject *getRootParent() const;

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
        /// Gets the time in seconds where the object was added to a canvas
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
        /// Gets the tick where the object was added to a canvas
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

        const CanvasSettings::UpdateControlls& getUpdateControlls() const;
        void setUpdateControlls(const CanvasSettings::UpdateControlls& controlls);

        const Utilities::AABB &getBoundingBox() const;
        void updateBoundingBox();

        std::string toString() const;

        
        // Childs operations
        void addChild(CanvasObject *child);
        void addChilds(const std::vector<CanvasObject*> &childs);

        void removeChild(CanvasObject *child);
        void removeChilds(const std::vector<CanvasObject*>& childs);
        void removeChilds();
        template<typename T>
        void removeChilds();

        void deleteChild(CanvasObject *child);
        void deleteChilds(const std::vector<CanvasObject*>& childs);
        void deleteChilds();
        template<typename T>
        void deleteChilds();

        bool childExists(CanvasObject *child) const;
        size_t getChildIndex(CanvasObject *child) const;
        const std::vector<CanvasObject*> &getChilds() const;

        template<typename T>
        std::vector<T*> getChilds() const;

        template<typename T>
        std::vector<T*> getChildsRecusrive() const;

        size_t getChildCount() const;

        template<typename T>
        size_t getChildCount() const;

        template<typename T>
        size_t getChildCountRecusrive() const;

        CanvasObject* findFirstChild(const std::string &name);
        std::vector<CanvasObject*> findAllChilds(const std::string &name);

        CanvasObject* findFirstChildRecursive(const std::string &name);
        std::vector<CanvasObject*> findAllChildsRecursive(const std::string &name);
        // ---------

        // Component operations
        void addComponent(Components::Component *comp);
        void addComponents(const std::vector<Components::Component*>& components);

        void removeComponent(Components::Component *comp);
        void removeComponents(const std::vector<Components::Component*>& components);
        template<typename T>
        void removeComponents();

        void deleteComponent(Components::Component *comp);
        void deleteComponents(const std::vector<Components::Component*>& components);
        void deleteComponents();
        template<typename T>
        void deleteComponents();

        bool componentExists(Components::Component *comp) const;
        size_t getComponentIndex(Components::Component *comp) const;
        const std::vector<Components::Component*> &getComponents()  const;
        template<typename T>
        std::vector<T*> getComponents() const;
        size_t getComponentCount() const;
        template<typename T>
        size_t getComponentCount() const;

        template<typename T>
        size_t getComponentCountRecusrive() const;

        Components::Component* findFirstComponent(const std::string& name);
        std::vector<Components::Component*> findAllComponents(const std::string& name);

        Components::Component* findFirstComponentRecursive(const std::string& name);
        std::vector<Components::Component*> findAllComponentsRecursive(const std::string& name);


        const std::vector<Components::Collider*> &getCollider() const;
        bool checkCollision(const CanvasObject* other) const;
        bool checkCollision(const CanvasObject* other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
        static void checkCollision(const Utilities::ObjectQuadTree& tree, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true);
        //void solveCollision(CanvasObject* other);
        // ---------

        // Canvas operations

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
        sf::Vector2u getCanvasSize() const;
        sf::Vector2u getOldCanvasSize() const;

        const sf::Font &getTextFont() const;

        /// <summary>
        /// Gets the current canvas tick count
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
        /// Gets the time in seconds since the start of the canvas
        /// Timedomain: Real simulation time
        /// </summary>
        /// <returns>Time since start</returns>
        double getElapsedTime() const; 

        /// <summary>
        /// Gets the time in seconds since the start of the canvas
        /// Timedomain: Fixed simulation time
        /// </summary>
        /// <returns>Time since start</returns>
        double getFixedElapsedTime() const; 

        /// <summary>
        /// Gets the canvas this object is contained in
        /// </summary>
        /// <returns>Parent canvas</returns>
        Canvas* getCanvasParent() const;

        /// <summary>
        /// Searches the object with the given name in the canvas.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::CanvasObject* findFirstObjectGlobal(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the canvas
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::CanvasObject*> findAllObjectsGlobal(const std::string& name);

        /// <summary>
        /// Searches the object with the given name in the canvas
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::CanvasObject* findFirstObjectGlobalRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the canvas
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::CanvasObject*> findAllObjectsGlobalRecusive(const std::string& name);
        // ---------

        
        

        const static size_t npos = -1;
    protected:
        /**
         * \brief update will be called once per frame
         */
        void update() override;

        virtual void inCanvasAdded();



        virtual void onCanvasParentChange(Canvas *oldParent, Canvas *newParent);
        virtual void onParentChange(CanvasObject *oldParent, CanvasObject *newParent);

        virtual void internalOnCanvasParentChange(Canvas *oldParent, Canvas *newParent);
        virtual void internalOnParentChange(CanvasObject *oldParent, CanvasObject *newParent);

        

        void deleteThis();

        void positionChanged(const sf::Vector2f& oldPosition, const sf::Vector2f& newPosition) override;
        void rotationChanged(float oldRotation, float newRotation) override;

    private:
        std::vector<std::string> toStringInternal(const std::string &preStr) const;
        bool findAllChilds_internal(const std::string& name, std::vector<CanvasObject*>& foundList);
        bool findAllChildsRecursive_internal(const std::string& name, std::vector<CanvasObject*>& foundList);
        bool findAllComponentsRecursive_internal(const std::string& name, std::vector<Components::Component*>& foundList);

        template<typename T>
        void getChildsRecusrive_internal(std::vector<T*> &listOut) const;


        void removeChild_internal();
        void removeComponent_internal();
        void deleteChild_internal();
        void addChild_internal();
        inline void addChild_internal(CanvasObject *obj);
        inline void setParent_internal(CanvasObject *parent,
                                       CanvasObject *rootParent,
                                       Canvas *canvasParent);
        void addComponent_internal();
        void onObjectsChanged();

        

        bool m_enabled;
        std::string m_name;
        //sf::Vector2f m_position;
        double m_birthTime; // Time of the canvas, in seconds where the object was added to a canvas (Time domain: real simulation time)
        size_t m_birthTick; // Tick of the canvas, where the object was added to a canvas

        // Hirarchy
        Canvas *m_canvasParent;
        CanvasObject *m_parent;
        CanvasObject *m_rootParent;

       

        
        std::vector<CanvasObject*> m_childs;
        std::vector<Components::Component*> m_components;
       

        std::vector<Utilities::Updatable*> m_updatableComponents;
        std::vector<Components::Collider*> m_colliders;
        Utilities::AABB m_boundingBox;
        

        // Will send a signal to the parent to notify, the new status
        void needsEventUpdateChanged(bool needsEventUpdate);
        void needsEventUpdate(bool needsEventUpdate);
        bool m_thisNeedsEventUpdate;
        std::vector<Components::SfEventHandle*> m_eventComponents;

        // Will send a signal to the parent to notify, the new status
        void needsDrawUpdateChanged(bool needsDrawUpdate);
        void needsDrawUpdate(bool needsDrawUpdate);
        bool m_thisNeedsDrawUpdate;
        std::vector<Components::Drawable*> m_drawableComponents;

        bool m_objectsChanged;
        std::vector<CanvasObject*> m_toAddChilds;
        std::vector<CanvasObject*> m_toDeleteChilds;
        std::vector<CanvasObject*> m_toRemoveChilds;
        
        std::vector<Components::Component*> m_toAddComponents;
        std::vector<Components::Component*> m_toRemoveComponents;

        CanvasSettings::UpdateControlls m_updateControlls;

        RenderLayer m_renderLayer;

        // Canvas Object Internal functions
        void setCanvasParent(Canvas *parent);
        
        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update_internal();
        void inCanvasAdded_internal();
        //void draw(sf::RenderWindow &window) const;
        void draw(sf::RenderWindow& window, sf::RenderStates states) const;

        // Static
        static size_t s_objNameCounter;
};
template<typename T>
void CanvasObject::removeChilds()
{
    removeChilds(getChilds<T>());
}
template<typename T>
void CanvasObject::deleteChilds()
{
    deleteChilds(getChilds<T>());
}
template<typename T>
std::vector<T*> CanvasObject::getChilds() const
{
    std::vector<T*> list;
    list.reserve(m_childs.size());
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        T* child = dynamic_cast<T*>(m_childs[i]);
        if(child)
            list.push_back(child);
    }
    return list;
}

template<typename T>
std::vector<T*> CanvasObject::getChildsRecusrive() const
{
    std::vector<T*> list;
    list.reserve(m_childs.size()*2);
    getChildsRecusrive_internal(list);
    return list;
}
template<typename T>
void CanvasObject::getChildsRecusrive_internal(std::vector<T*>& listOut) const
{
    for (CanvasObject* obj : m_childs)
    {
		T* child = dynamic_cast<T*>(obj);
		if(child)
			listOut.push_back(child);
        obj->getChildsRecusrive_internal(listOut);
	}
}


template<typename T>
size_t CanvasObject::getChildCount() const
{
    size_t counter = 0;
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        T* child = dynamic_cast<T*>(m_childs[i]);
        if(child)
            ++counter;
    }
    return counter;
}

template<typename T>
size_t CanvasObject::getChildCountRecusrive() const
{
	size_t counter = 0;
    for (CanvasObject* obj : m_childs)
    {
		T* child = dynamic_cast<T*>(obj);
		if(child)
			++counter;
		counter += obj->getChildCountRecusrive<T>();
	}
	return counter;
}

template<typename T>
void CanvasObject::removeComponents()
{
    removeComponent(getComponents<T>());
}

template<typename T>
void CanvasObject::deleteComponents()
{
    deleteComponents(getComponents<T>());
}

template<typename T>
std::vector<T*> CanvasObject::getComponents() const
{
    std::vector<T*> list;
    list.reserve(m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(comp)
            list.push_back(comp);
    }
    return list;
}
template<typename T>
size_t CanvasObject::getComponentCount() const
{
    size_t count = 0;
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(comp)
            ++count;
    }
    return count;
}

template<typename T>
size_t CanvasObject::getComponentCountRecusrive() const
{
    size_t count = getComponentCount<T>();
    for (const CanvasObject* &obj : m_childs)
    {
		count += obj->getComponentCountRecusrive<T>();
	}
	return count;
}
}
}
