#include "canvas/CanvasObjectContainer.h"
#include "canvas/Canvas.h"


using namespace QSFML::Objects;
using namespace QSFML::Components;
using namespace QSFML;

CanvasObjectContainer::CanvasObjectContainer(Canvas *parent)
{
    m_parent = parent;
}
CanvasObjectContainer::~CanvasObjectContainer()
{

}

void CanvasObjectContainer::addObject(CanvasObject *obj)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));

    if(obj->getCanvasParent() != m_parent && obj->getCanvasParent())
        obj->getCanvasParent()->removeObject(obj);
    m_toAddContainer.push_back(obj);
    obj->setCanvasParent(m_parent);
}
void CanvasObjectContainer::addObject(const std::vector<CanvasObject*> &objs)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));
    for(size_t i=0; i<objs.size(); ++i)
    {
        addObject(objs[i]);
    }
}
void CanvasObjectContainer::addObject_internal()
{
    for(size_t i=0; i<m_toAddContainer.size(); ++i)
    {
        if(objectExists(m_toAddContainer[i]))
            continue;
        //m_toAddContainer[i]->setCanvasParent(m_parent);
        m_container.push_back(m_toAddContainer[i]);
    }
    m_toAddContainer.clear();
}
void CanvasObjectContainer::deleteObject_internal()
{
    for(size_t i=0; i<m_toDelete.size(); ++i)
    {
        deleteObject(m_toDelete[i]);
    }
    m_toDelete.clear();
}

void CanvasObjectContainer::removeObject(CanvasObject *obj)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));
    size_t index = getObjectIndex(obj);
    if(index == npos) return;
    obj->setCanvasParent(nullptr);
    m_container.erase(m_container.begin() + index);
}
void CanvasObjectContainer::removeObject(const std::vector<CanvasObject*> &objs)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));
    for(size_t i=0; i<objs.size(); ++i)
    {
        removeObject(objs[i]);
    }
}
void CanvasObjectContainer::deleteObject(Objects::CanvasObject *obj)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));
    removeObject(obj);
    delete obj;
}
void CanvasObjectContainer::deleteObject(const std::vector<Objects::CanvasObject*> &objs)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange));
    removeObject(objs);
    for(size_t i=0; i<objs.size(); ++i)
    {
        delete objs[i];
    }
}
void CanvasObjectContainer::clearObjects()
{
    m_container.clear();
}

void CanvasObjectContainer::reserveObjectsCount(size_t size)
{
    m_container.reserve(size);
}
size_t CanvasObjectContainer::getObjectsCount() const
{
    return m_container.size();
}

const std::vector<CanvasObject*> &CanvasObjectContainer::getObjects() const
{
    return m_container;
}

bool CanvasObjectContainer::objectExists(CanvasObject *obj)
{
    return objectExists<CanvasObject>(obj,m_container);
}
size_t CanvasObjectContainer::getObjectIndex(CanvasObject *obj)
{
    return getObjectIndex<CanvasObject>(obj,m_container);
}
void CanvasObjectContainer::deleteLater(Objects::CanvasObject *obj)
{
    for(size_t i=0; i<m_toAddContainer.size(); ++i)
    {
        if(m_toAddContainer[i] == obj)
        {
            m_toAddContainer.erase(m_toAddContainer.begin() + i);
            break;
        }
    }
    /*if(objectExists(obj))
    {
        m_toDelete.push_back(obj);
    }*/
    m_toDelete.push_back(obj);
}

void CanvasObjectContainer::updateNewElements()
{
    deleteObject_internal();
    addObject_internal();
    for(size_t i=0; i<m_container.size(); ++i)
    {
        if(m_container[i]->m_objectsChanged)
            m_container[i]->updateNewElements();
    }
}
void CanvasObjectContainer::sfEvent(const std::vector<sf::Event> &events)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->sfEvent(events);
    }
}
void CanvasObjectContainer::update()
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->update_internal();
    }
}
void CanvasObjectContainer::draw(sf::RenderWindow &window)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        if(m_container[i]->m_thisNeedsDrawUpdate)
            m_container[i]->draw(window);
    }
}

