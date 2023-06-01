#pragma once

#include "QSFML_base.h"
#include "canvas/RenderLayer.h"
#include "canvas/CanvasSettings.h"

#include "utilities/CollisionInfo.h"
#include "utilities/Updatable.h"
#include "utilities/AABB.h"

#include <vector>
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

class QSFML_EDITOR_WIDGET_EXPORT CanvasObject: protected Utilities::Updatable
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


        void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;
        CanvasObject *getRootParent() const;

        void setEnabled(bool enable);
        bool isEnabled() const;

        void setName(const std::string &name);
        const std::string getName() const;

        void setPositionRelative(const sf::Vector2f& pos); // Sets the position relative to its parent
        void setPositionAbsolute(const sf::Vector2f& pos); // Sets the position in the absolute world coords.
        const sf::Vector2f& getPositionRelative() const;   // Gets the position relative to its parent
        sf::Vector2f getPositionAbsolute() const;          // Gets the position in absolute world coords

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
        size_t getChildCount() const;
        template<typename T>
        size_t getChildCount() const;
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
        const std::vector<Components::Collider*> &getCollider() const;
        bool checkCollision(const CanvasObject* other) const;
        bool checkCollision(const CanvasObject* other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
        void solveCollision(CanvasObject* other);
        // ---------

        // Canvas operations
        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i& pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f& worldSpace) const;

        const sf::View getCameraView() const;
        const sf::View &getDefaultCameraView() const;
        sf::Vector2u getCanvasSize() const;
        sf::Vector2u getOldCanvasSize() const;

        const sf::Font &getTextFont() const;

        size_t getTick() const;
        float getDeltaT() const; // Returns delta Time since last update in seconds

        Canvas* getCanvasParent() const;
        // ---------

        
        

        const static size_t npos = -1;
    protected:
        /**
         * \brief update will be called once per frame
         */
        void update() override;




        virtual void onCanvasParentChange(Canvas *oldParent, Canvas *newParent);
        virtual void onParentChange(CanvasObject *oldParent, CanvasObject *newParent);

        virtual void internalOnCanvasParentChange(Canvas *oldParent, Canvas *newParent);
        virtual void internalOnParentChange(CanvasObject *oldParent, CanvasObject *newParent);

        

        void deleteThis();

    private:
        std::vector<std::string> toStringInternal(const std::string &preStr) const;

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
        sf::Vector2f m_position;

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
        virtual void inCanvasAdded();
        void setCanvasParent(Canvas *parent);
        
        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update_internal();
        void draw(sf::RenderWindow &window) const;

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
}
}
