#pragma once

#include <SFML/Graphics.hpp>
#include <QDebug>
#include <vector>
#include "canvas/CanvasForwardDeclaration.h"
#include "canvas/RenderLayer.h"
#include "components/Component.h"
#include "canvas/CanvasSettings.h"
#include "components/SfEventHandle.h"
#include "components/Drawable.h"
#include "components/physics/Collider.h"
#include "utilities/Updatable.h"
#include "QSFML_debugSettings.h"
#include <easy/profiler.h>
#include "QSFML_global.h"

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
        CanvasObject(const std::string &name = "",
                     CanvasObject *parent = nullptr);
        CanvasObject(const CanvasObject &other);
        virtual ~CanvasObject();

        virtual CanvasObject* clone() const;


        void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;
        CanvasObject *getRootParent() const;

        void setEnabled(bool enable);
        bool isEnabled() const;

        void setName(const std::string &name);
        const std::string getName() const;

        void setRenderLayer(RenderLayer layer);
        RenderLayer getRenderLayer() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i &pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace) const;

        void addChild(CanvasObject *child);
        void removeChild(CanvasObject *child);
        void deleteChild(CanvasObject *child);
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
        void deleteComponent(Components::Component *comp);
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

        const sf::View getCameraView() const;
        const sf::View &getDefaultCameraView() const;
        sf::Vector2u getCanvasSize() const;
        sf::Vector2u getOldCanvasSize() const;

        const sf::Font &getTextFont() const;

        size_t getUpdateCount() const;
        float getDeltaT() const; // Returns delta Time since last update in seconds


        const CanvasSettings::UpdateControlls &getUpdateControlls() const;
        void setUpdateControlls(const CanvasSettings::UpdateControlls &controlls);

        std::string toString() const;
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

        Canvas *getCanvasParent() const;

        void deleteThis();

    private:

        std::vector<std::string> toStringInternal(const std::string &preStr) const;

        void removeChild_internal();
        void removeComponent_internal();
        void deleteChild_internal();
        //void deleteComponent_internal();
        void addChild_internal();
        inline void addChild_internal(CanvasObject *obj);
        inline void setParent_internal(CanvasObject *parent,
                                       CanvasObject *rootParent,
                                       Canvas *canvasParent);
        void addComponent_internal();
        void onObjectsChanged();

        static size_t m_objNameCounter;

        bool m_enabled;
        std::string m_name;
        Canvas *m_canvasParent;
        CanvasObject *m_parent;
        CanvasObject *m_rootParent;

        bool m_objectsChanged;
        std::vector<CanvasObject*> m_childs;
        std::vector<CanvasObject*> m_toAddChilds;

        /*struct ComponentMetadata
        {
            Components::Component* component;

        };*/


        std::vector<Components::Component*> m_components;
        std::vector<Components::Component*> m_toAddComponents;


        std::vector<Utilities::Updatable*> m_updatableComponents;

        std::vector<Components::Collider*> m_colliders;

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

        std::vector<CanvasObject*> m_toDeleteChilds;
        std::vector<CanvasObject*> m_toRemoveChilds;
        std::vector<Components::Component*> m_toRemoveComponents;

        CanvasSettings::UpdateControlls m_updateControlls;

        RenderLayer m_renderLayer;

        // Canvas Object Internal functions
        void setCanvasParent(Canvas *parent);
        inline void updateNewElements()
        {
            QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange));
            removeChild_internal();
            deleteChild_internal();
            removeComponent_internal();
            //deleteComponent_internal();
            addChild_internal();
            addComponent_internal();

            for(size_t i=0; i<m_childs.size(); ++i)
                m_childs[i]->updateNewElements();
            m_objectsChanged = false;
        }
        inline void sfEvent(const std::vector<sf::Event> &events)
        {
            if(!m_enabled || !m_updateControlls.enableEventLoop || !m_thisNeedsEventUpdate) return;
            QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange100));
            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components event", profiler::colors::Orange400));
            for(size_t i=0; i<m_eventComponents.size(); ++i)
            {
                if(!m_eventComponents[i]->isEnabled())
                    continue;

                for(size_t j=0; j<events.size(); ++j)
                    m_eventComponents[i]->sfEvent(events[j]);

            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs event", profiler::colors::Orange500));
            for(size_t i=0; i<m_childs.size(); ++i)
            {
                CanvasObject *obj = m_childs[i];
                if(obj->m_enabled)
                    obj->sfEvent(events);
            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
        }
        inline void update_internal()
        {
            if(!m_enabled || !m_updateControlls.enableUpdateLoop) return;
            QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange600));
            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("This update", profiler::colors::Orange700));
            update();
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components update", profiler::colors::Orange800));
            for(size_t i=0; i<m_updatableComponents.size(); ++i)
            {
                Utilities::Updatable* comp = m_updatableComponents[i];
                Components::Component* comp1 = dynamic_cast<Components::Component*>(comp);
                if(!comp1->isEnabled())
                    continue;
                comp->update();
            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs update", profiler::colors::Orange800));
            for(size_t i=0; i<m_childs.size(); ++i)
            {
                CanvasObject *obj = m_childs[i];
                if(obj->m_enabled)
                    obj->update_internal();
            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
        }
        /*inline void draw(sf::RenderWindow &window) const
        {
            if(!m_enabled || !m_updateControlls.enablePaintLoop) return;
            QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange900));
            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components draw", profiler::colors::OrangeA100));
            for(size_t i=0; i<m_drawableComponents.size(); ++i)
            {
                if(!m_drawableComponents[i]->isEnabled())
                    continue;
                window.draw(*m_drawableComponents[i]);
            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

            QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs draw", profiler::colors::OrangeA200));
            for(size_t i=0; i<m_childs.size(); ++i)
            {
                if(m_childs[i]->m_enabled && m_childs[i]->m_thisNeedsDrawUpdate)
                    m_childs[i]->draw(window);
            }
            QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
        }*/
        /*void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update_internal();*/
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
           /* m_childs.erase(m_childs.begin() + i);
            --i;
            delete child;*/

            bool match = false;
            for(size_t j=0; j<m_toDeleteChilds.size(); ++j)
                if(m_toDeleteChilds[j] == child)
                    match = true;
            if(!match)
                m_toDeleteChilds.push_back(child);
        }
    }
    m_objectsChanged = true;
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
    for(int i=0; i<(int)m_toAddComponents.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_toAddComponents[i]);
        if(!comp)
            continue;
        delete comp;
        m_toAddComponents.erase(m_toAddComponents.begin() + i);

        --i;
    }
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(!comp)
            continue;

        // Check for sfEventHandles
        Components::SfEventHandle *evComp = dynamic_cast<Components::SfEventHandle*>(comp);
        if(evComp)
        {
            for(size_t j=0; j<m_eventComponents.size(); ++j)
                if(m_eventComponents[j] == evComp)
                {
                    m_eventComponents.erase(m_eventComponents.begin() + j);
                    break;
                }
            if(m_eventComponents.size() == 0)
            {
                needsEventUpdate(false);
            }
        }

        // Check for Drawables
        Components::Drawable *drawComp = dynamic_cast<Components::Drawable*>(comp);
        if(drawComp)
        {
            for(size_t j=0; j<m_drawableComponents.size(); ++j)
                if(m_drawableComponents[j] == drawComp)
                {
                    m_drawableComponents.erase(m_drawableComponents.begin() + j);
                    break;
                }
            if(m_drawableComponents.size() == 0)
            {
                needsDrawUpdate(false);
            }
        }

        // Check for Updatables
        Utilities::Updatable *updatable = dynamic_cast<Utilities::Updatable*>(comp);
        if(updatable)
        {
            for(size_t j=0; j<m_updatableComponents.size(); ++j)
                if(m_updatableComponents[j] == comp)
                {
                    m_updatableComponents.erase(m_updatableComponents.begin() + j);
                    break;
                }
        }


        m_components.erase(m_components.begin() + i);
        --i;
        delete comp;
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
