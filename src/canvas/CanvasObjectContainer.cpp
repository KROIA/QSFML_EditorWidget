#include "CanvasObjectContainer.h"
#include "QSFML_Canvas.h"

CanvasObjectContainer::CanvasObjectContainer(QSFML_Canvas *parent)
{
    m_parent = parent;
}
CanvasObjectContainer::~CanvasObjectContainer()
{

}

void CanvasObjectContainer::addObject(CanvasObject *obj)
{
    EASY_FUNCTION(profiler::colors::Orange);
    if(!objectExists(obj))
    {
        if(obj->getCanvasParent() != m_parent && obj->getCanvasParent())
            obj->getCanvasParent()->removeObject(obj);
        m_container.push_back(obj);
        obj->setCanvasParent(m_parent);

       // addObject<SfEventHandleComponent>(obj,m_eventhandledObjects);
       // addObject<DrawableComponent>(obj,m_drawables);
       // addObject<CameraController>(obj,m_cameras);
    }
}
void CanvasObjectContainer::addObject(const std::vector<CanvasObject*> &objs)
{
    EASY_FUNCTION(profiler::colors::Orange);
    for(size_t i=0; i<objs.size(); ++i)
    {
        addObject(objs[i]);
    }
}

void CanvasObjectContainer::removeObject(CanvasObject *obj)
{
    EASY_FUNCTION(profiler::colors::Orange);
    size_t index = getObjectIndex(obj);
    if(index == npos) return;
    obj->setCanvasParent(nullptr);
    m_container.erase(m_container.begin() + index);

   // removeObject<SfEventHandleComponent>(obj,m_eventhandledObjects);
   // removeObject<DrawableComponent>(obj,m_drawables);
   // removeObject<CameraController>(obj,m_cameras);
}
void CanvasObjectContainer::removeObject(const std::vector<CanvasObject*> &objs)
{
    EASY_FUNCTION(profiler::colors::Orange);
    for(size_t i=0; i<objs.size(); ++i)
    {
        removeObject(objs[i]);
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

void CanvasObjectContainer::sfEvent(const std::vector<sf::Event> &events)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->sfEvent(events);
    }


   /* for(std::vector<SfEventHandleComponent*>::iterator it = m_eventhandledObjects.begin();
        it != m_eventhandledObjects.end(); ++it)
    {
        (*it)->sfEvent(e);
    }*/
}
void CanvasObjectContainer::draw(sf::RenderWindow &window)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->draw(window);
    }
    //qDebug() << m_drawables.size()<<" drawables";
    /*for(std::vector<DrawableComponent*>::iterator it = m_drawables.begin(); it != m_drawables.end(); ++it) {
        (*it)->draw();
    }*/
}

template<typename T>
bool CanvasObjectContainer::objectExists(T *obj,const std::vector<T*> &list)
{
    EASY_FUNCTION(profiler::colors::Orange100);
    size_t num = list.size();
    if(!num) return false;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return true;
    }
    return false;
}

template<typename T>
size_t CanvasObjectContainer::getObjectIndex(T *obj,const std::vector<T*> &list)
{
    EASY_FUNCTION(profiler::colors::Orange200);
    size_t num = list.size();
    if(!num) return npos;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return it - list.begin();
    }
    return npos;
}
template<typename T>
void CanvasObjectContainer::addObject(CanvasObject *obj,std::vector<T*> &list)
{
    EASY_FUNCTION(profiler::colors::Orange300);
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        list.push_back(transformed);
    }
}

template<typename T>
void CanvasObjectContainer::removeObject(CanvasObject *obj,std::vector<T*> &list)
{
    EASY_FUNCTION(profiler::colors::Orange400);
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        size_t index = getObjectIndex<T>(transformed,list);
        if(index != npos)
            list.erase(list.begin()+index);
    }
}
