#pragma once

#include <SFML/Graphics.hpp>
#include <QDebug>
#include <vector>
#include "CanvasForwardDeclaration.h"
#include "Component.h"
#include <easy/profiler.h>

namespace QSFML
{
namespace Objects
{
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
 *             VectorDisplayer *m_vec;
 *     };
 * };
 * \endcode
 */

class CanvasObject
{
        friend Canvas;
        friend CanvasObjectContainer;
    public:
        CanvasObject(const std::string &name = "",
                     CanvasObject *parent = nullptr);
        virtual ~CanvasObject();

        void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;

        void setEnabled(bool enable);
        bool isEnabled() const;

        void setName(const std::string &name);
        const std::string getName() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i &pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace) const;

        void addChild(CanvasObject *child);
        void removeChild(CanvasObject *child);
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

        void addComponent(Components::Component *comp);
        void removeComponent(Components::Component *comp);
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

        const sf::View getCameraView() const;
        const sf::View &getDefaultCameraView() const;
        sf::Vector2u getCanvasSize() const;
        sf::Vector2u getOldCanvasSize() const;

        virtual void update();

        std::string toString() const;
        const static size_t npos = -1;
    protected:
        std::vector<std::string> toStringInternal(const std::string &preStr) const;


        virtual void onCanvasParentChange(Canvas *newParent);
        virtual void onParentChange(CanvasObject *newParent);

        virtual void internalOnCanvasParentChange(Canvas *newParent);
        virtual void internalOnParentChange(CanvasObject *newParent);

        Canvas *getCanvasParent() const;



    private:
        static size_t m_objNameCounter;

        bool m_enabled;
        std::string m_name;
        Canvas *m_canvasParent;
        CanvasObject *m_parent;
        std::vector<CanvasObject*> m_childs;
        std::vector<Components::Component*> m_components;

        // Canvas Object Internal functions
        void setCanvasParent(Canvas *parent);
        void sfEvent(const std::vector<sf::Event> &events);
        void update_internal();
        void draw(sf::RenderWindow &window) const;

};

template<typename T>
void CanvasObject::deleteChilds()
{
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        T* child = dynamic_cast<T*>(m_childs[i]);
        if(child)
        {
            m_childs.erase(m_childs.begin() + i);
            --i;
            delete child;
        }
    }
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
void CanvasObject::deleteComponents()
{
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(comp)
        {
            m_components.erase(m_components.begin() + i);
            --i;
            delete comp;
        }
    }
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
