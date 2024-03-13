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

CanvasObjectContainer::CanvasObjectContainer(Canvas *parent, CanvasSettings &settings)
    : m_settings(settings)
    , m_renderLayerGroup(parent, RenderLayer::count)
{
    m_parent = parent;
    m_allObjects = new CanvasObjectGroup(m_parent);
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());

    m_currentThreadGroupInsertIndex = 0;
    m_threadWorker = nullptr;
    m_threadGroupCount = 0;
}
CanvasObjectContainer::~CanvasObjectContainer()
{
    m_parent->setRootCanvesObject(0);
    delete m_allObjects;
}
void CanvasObjectContainer::applyObjectChanges()
{
    updateNewElements();
}

void CanvasObjectContainer::initializeThreads(size_t threadCount)
{
    if(m_threadWorker)
        return;
    m_currentThreadGroupInsertIndex = 0;
    m_threadGroupCount = m_settings.updateControlls.threadSettings.objectGroups;
    for(size_t i=0; i<m_threadGroupCount; ++i)
    {
        m_threadGroups.push_back(new CanvasObjectGroup(m_parent));
    }
    const std::vector<CanvasObject*> &objs = m_allObjects->getObjects();
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
void CanvasObjectContainer::deinitializeThreads()
{
    CanvasThreadWorker* currentWorker = m_threadWorker;
    m_threadWorker = nullptr;
    
    for (size_t i = 0; i < m_threadGroups.size(); ++i)
    {
        delete m_threadGroups[i];
    }
    m_threadGroups.clear();
    m_threadGroupCount = 0;
    delete currentWorker;
}

void CanvasObjectContainer::addObject(CanvasObject *obj)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);

    if(obj->getCanvasParent() != m_parent && obj->getCanvasParent())
        obj->getCanvasParent()->removeObject(obj);
    m_allObjects->addObject(obj);
    if(m_threadWorker)
    {
        m_threadGroups[m_currentThreadGroupInsertIndex]->addObject(obj);
        m_currentThreadGroupInsertIndex = (m_currentThreadGroupInsertIndex+1)%m_threadGroupCount;
    }
    m_renderLayerGroup.addObject(obj, obj->getRenderLayer());

    obj->setCanvasParent(m_parent);
}
void CanvasObjectContainer::addObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
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
    m_renderLayerGroup.addObject_internal();
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());    
}
void CanvasObjectContainer::deleteObject_internal()
{
    m_allObjects->deleteObject_internal();
    m_renderLayerGroup.deleteObject_internal();
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());
}

void CanvasObjectContainer::removeObject(CanvasObject *obj)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    m_allObjects->removeObject(obj);
    if(m_threadWorker)
    {
        for(size_t i=0; i<m_threadGroups.size(); ++i)
        {
            m_threadGroups[i]->removeObject(obj);
        }
    }
    m_renderLayerGroup.removeObject(obj, obj->getRenderLayer());
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());
}
void CanvasObjectContainer::removeObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    for(size_t i=0; i<objs.size(); ++i)
    {
        removeObject(objs[i]);
    }
}
void CanvasObjectContainer::deleteObject(Objects::CanvasObject *obj)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    removeObject(obj);
    delete obj;
}
void CanvasObjectContainer::deleteObject(const std::vector<Objects::CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    removeObject(objs);
    for(size_t i=0; i<objs.size(); ++i)
    {
        delete objs[i];
    }
}
void CanvasObjectContainer::clearObjects()
{
    m_allObjects->clearObjects();
    m_renderLayerGroup.clearObjects();
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());
}

void CanvasObjectContainer::reserveObjectsCount(size_t size)
{
    m_allObjects->reserveObjectsCount(size);
    m_renderLayerGroup.reserveObjectsCount(size);
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
        return m_renderLayerGroup.objectExists(obj, obj->getRenderLayer());

    return m_allObjects->objectExists(obj);
}
size_t CanvasObjectContainer::getObjectIndex(CanvasObject *obj)
{
    if(obj->getRenderLayer() < RenderLayer::count)
        return m_renderLayerGroup.getObjectIndex(obj, obj->getRenderLayer());

    return m_allObjects->getObjectIndex(obj);
}
void CanvasObjectContainer::deleteLater(Objects::CanvasObject *obj)
{
    m_allObjects->deleteLater(obj);
    if(obj->getRenderLayer() < RenderLayer::count)
        m_renderLayerGroup.removeObject(obj, obj->getRenderLayer());
}
void CanvasObjectContainer::renderLayerSwitch(Objects::CanvasObject *obj, RenderLayer from, RenderLayer to)
{
    if(!obj)
        return;
    if(obj->m_canvasParent != m_parent)
        return; // not owner of this object
    if(from < RenderLayer::count)
        m_renderLayerGroup.removeObject(obj, obj->getRenderLayer());
    if(to < RenderLayer::count)
        m_renderLayerGroup.addObject(obj, obj->getRenderLayer());
}



void CanvasObjectContainer::updateNewElements()
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    deleteObject_internal();
    const std::vector<Objects::CanvasObject*> &toAdd = m_allObjects->getObjectsToAdd();

    addObject_internal();
    m_allObjects->updateNewElements();
    m_renderLayerGroup.updateNewElements();
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
            initializeThreads(m_parent->m_settings.updateControlls.threadSettings.threadCount);
        m_threadWorker->process();
    }
    else
    {
        m_allObjects->update();
    }
}
void CanvasObjectContainer::draw(sf::RenderWindow &window)
{
    for(size_t i=0; i< m_renderLayerGroup.size(); ++i)
    {
        QSFMLP_CANVAS_BLOCK("Draw layer", QSFML_COLOR_STAGE_1);
        m_renderLayerGroup[i].draw(window);
        QSFML_PROFILING_END_BLOCK;
    }
}

