#include "canvas/CanvasObjectContainer.h"
#include "canvas/Canvas.h"
#include "utilities/Stats.h"
#include "canvas/CanvasObjectGroup.h"
#include "canvas/CanvasThreadWorker.h"

#include "objects/base/CanvasObject.h"
#include "objects/CameraController.h"

#include "components/base/Drawable.h"



using namespace QSFML::Objects;
using namespace QSFML::Components;
using namespace QSFML;

CanvasObjectContainer::CanvasObjectContainer(Canvas *parent, const CanvasSettings &settings)
{
    m_updateCount = 0;
    m_parent = parent;
    m_allObjects = new CanvasObjectGroup(m_parent);
    StatsManager::setRootCanvesObject(m_allObjects->getObjectsCount());
    for(size_t i=0; i<RenderLayer::count; ++i)
    {
        m_renderLayerGroups.push_back(new CanvasObjectGroup(m_parent));
    }

    m_threadGroupCount = settings.updateControlls.threadSettings.objectGroups;
    m_currentThreadGroupInsertIndex = 0;
    m_threadWorker = nullptr;
    if(settings.updateControlls.enableMultithreading)
        setupThreads(settings.updateControlls.threadSettings.threadCount);

}
CanvasObjectContainer::~CanvasObjectContainer()
{
    StatsManager::setRootCanvesObject(0);
    delete m_allObjects;
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->clearObjects();
        delete m_renderLayerGroups[i];
    }
}
void CanvasObjectContainer::applyObjectChanges()
{
    updateNewElements();
}

void CanvasObjectContainer::setupThreads(size_t threadCount)
{
    if(m_threadWorker)
        return;
    for(size_t i=0; i<m_threadGroupCount; ++i)
    {
        m_threadGroups.push_back(new CanvasObjectGroup(m_parent));
    }
    std::vector<CanvasObject*> objs = m_allObjects->getObjects();
    for(size_t i=0; i<objs.size(); ++i)
    {
        m_threadGroups[i%m_threadGroups.size()]->addObject(objs[i]);
    }
    for(size_t i=0; i<m_threadGroups.size(); ++i)
    {
        m_threadGroups[i]->addObject_internal();
    }
    m_threadWorker = new CanvasThreadWorker(threadCount, &m_threadGroups);
}

void CanvasObjectContainer::addObject(CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);

    if(obj->getCanvasParent() != m_parent && obj->getCanvasParent())
        obj->getCanvasParent()->removeObject(obj);
    m_allObjects->addObject(obj);
    if(m_threadWorker)
    {
        m_threadGroups[m_currentThreadGroupInsertIndex]->addObject(obj);
        m_currentThreadGroupInsertIndex = (m_currentThreadGroupInsertIndex+1)%m_threadGroupCount;
    }
    if(obj->getRenderLayer() < RenderLayer::count)
        m_renderLayerGroups[obj->getRenderLayer()]->addObject(obj);
  
    obj->setCanvasParent(m_parent);
}
void CanvasObjectContainer::addObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    for(size_t i=0; i<objs.size(); ++i)
    {
        addObject(objs[i]);
    }
}
void CanvasObjectContainer::addObject_internal()
{
    
    m_allObjects->addObject_internal();
    
    if(m_threadWorker)
    {
        for(size_t i=0; i<m_threadGroups.size(); ++i)
        {
            m_threadGroups[i]->addObject_internal();
        }
    }
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->addObject_internal();
    }
    StatsManager::setRootCanvesObject(m_allObjects->getObjectsCount());

    
}
void CanvasObjectContainer::deleteObject_internal()
{
    m_allObjects->deleteObject_internal();
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->deleteObject_internal();
    }
    StatsManager::setRootCanvesObject(m_allObjects->getObjectsCount());
}

void CanvasObjectContainer::removeObject(CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    m_allObjects->removeObject(obj);
    if(m_threadWorker)
    {
        for(size_t i=0; i<m_threadGroups.size(); ++i)
        {
            m_threadGroups[i]->removeObject(obj);
        }
    }
    if(obj->getRenderLayer() < RenderLayer::count)
        m_renderLayerGroups[obj->getRenderLayer()]->removeObject(obj);
    StatsManager::setRootCanvesObject(m_allObjects->getObjectsCount());
}
void CanvasObjectContainer::removeObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    for(size_t i=0; i<objs.size(); ++i)
    {
        removeObject(objs[i]);
    }
}
void CanvasObjectContainer::deleteObject(Objects::CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    removeObject(obj);
    delete obj;
}
void CanvasObjectContainer::deleteObject(const std::vector<Objects::CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    removeObject(objs);
    for(size_t i=0; i<objs.size(); ++i)
    {
        delete objs[i];
    }
}
void CanvasObjectContainer::clearObjects()
{
    m_allObjects->clearObjects();
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->clearObjects();
    }
    StatsManager::setRootCanvesObject(m_allObjects->getObjectsCount());
}

void CanvasObjectContainer::reserveObjectsCount(size_t size)
{
    m_allObjects->reserveObjectsCount(size);
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->reserveObjectsCount(size);
    }
}
size_t CanvasObjectContainer::getObjectsCount() const
{
    return m_allObjects->getObjectsCount();
}

const std::vector<CanvasObject*> &CanvasObjectContainer::getObjects() const
{
    return m_allObjects->getObjects();
}

bool CanvasObjectContainer::objectExists(CanvasObject *obj)
{
    if(obj->getRenderLayer() < RenderLayer::count)
        return m_renderLayerGroups[obj->getRenderLayer()]->objectExists(obj);

    return m_allObjects->objectExists(obj);
}
size_t CanvasObjectContainer::getObjectIndex(CanvasObject *obj)
{
    if(obj->getRenderLayer() < RenderLayer::count)
        return m_renderLayerGroups[obj->getRenderLayer()]->getObjectIndex(obj);

    return m_allObjects->getObjectIndex(obj);
}
void CanvasObjectContainer::deleteLater(Objects::CanvasObject *obj)
{
    m_allObjects->deleteLater(obj);
    if(obj->getRenderLayer() < RenderLayer::count)
        m_renderLayerGroups[obj->getRenderLayer()]->removeObject(obj);
}
void CanvasObjectContainer::renderLayerSwitch(Objects::CanvasObject *obj, RenderLayer from, RenderLayer to)
{
    if(!obj)
        return;
    if(obj->m_canvasParent != m_parent)
        return; // not owner of this object
    if(from < RenderLayer::count)
        m_renderLayerGroups[obj->getRenderLayer()]->removeObject(obj);
    if(to < RenderLayer::count)
        m_renderLayerGroups[obj->getRenderLayer()]->addObject(obj);
}



void CanvasObjectContainer::updateNewElements()
{
    QSFMLP_FUNCTION(QSFMLP_CANVASCONTAINER_COLOR_1);
    deleteObject_internal();
    std::vector<Objects::CanvasObject*> toAdd = m_allObjects->getObjectsToAdd();

    addObject_internal();
    m_allObjects->updateNewElements();
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        m_renderLayerGroups[i]->updateNewElements();
    }
    for (auto obj : toAdd)
        obj->inCanvasAdded_internal();
}
void CanvasObjectContainer::sfEvent(const std::vector<sf::Event> &events)
{
    m_allObjects->sfEvent(events);
}
void CanvasObjectContainer::update()
{
    if(m_parent->m_settings.updateControlls.enableMultithreading)
    {
        if(!m_threadWorker)
            setupThreads(m_parent->m_settings.updateControlls.threadSettings.threadCount);
        m_threadWorker->process();
    }
    else
    {
        m_allObjects->update();
    }
    ++m_updateCount;
}
void CanvasObjectContainer::draw(sf::RenderWindow &window)
{
    for(size_t i=0; i<m_renderLayerGroups.size(); ++i)
    {
        QSFMLP_BLOCK("Draw layer", QSFMLP_CANVASCONTAINER_COLOR_1);
        m_renderLayerGroups[i]->draw(window);
        QSFMLP_END_BLOCK;
    }
}

