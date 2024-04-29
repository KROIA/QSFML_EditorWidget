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
    cleanup();
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
    m_objectsToAdd.push_back(obj);
}
void CanvasObjectContainer::addObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToAdd.insert(m_objectsToAdd.end(), objs.begin(), objs.end());
}
void CanvasObjectContainer::addObject_internal()
{
    m_allObjects->addObject_internal();
    if(m_threadWorker)
    {
        for(auto &group : m_threadGroups)
		{
			group->addObject_internal();
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
    m_objectsToRemove.push_back(obj);
}
void CanvasObjectContainer::removeObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToRemove.insert(m_objectsToRemove.end(), objs.begin(), objs.end());
}
void CanvasObjectContainer::deleteObject(Objects::CanvasObject *obj)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToDelete.push_back(obj);
}
void CanvasObjectContainer::deleteObject(const std::vector<Objects::CanvasObject*> &objs)
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToDelete.insert(m_objectsToRemove.end(), objs.begin(), objs.end());
}
void CanvasObjectContainer::clearObjects()
{
    m_allObjects->clearObjects();
    m_renderLayerGroup.clearObjects();
    m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());
}
void CanvasObjectContainer::cleanup()
{
    if (!m_allObjects)
        return; // Already cleaned up
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    
    for (auto& group : m_threadGroups)
    {
        group->clearObjects();
        delete group;
    }
    m_allObjects->cleanup();
    delete m_allObjects;
    delete m_threadWorker;

    std::vector<Objects::CanvasObject*> objs = m_allObjects->getObjects();
    m_allObjects->clearObjects();
    m_renderLayerGroup.clearObjects();
    for (auto& obj : objs)
    {
        obj->setCanvasParent(nullptr);
        delete obj;
    }
    m_allObjects = nullptr;
    m_threadWorker = nullptr;
    m_threadGroups.clear();
    m_parent->setRootCanvesObject(0);
}

void CanvasObjectContainer::applyObjectChanges()
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<CanvasObject*> objectsToAdd = m_objectsToAdd;
    std::vector<CanvasObject*> objectsToRemove = m_objectsToRemove;
    std::vector<CanvasObject*> objectsToDelete = m_objectsToDelete;
    m_objectsToAdd.clear();
    m_objectsToRemove.clear();
    m_objectsToDelete.clear();
    objectsToRemove.insert(objectsToRemove.end(), objectsToDelete.begin(), objectsToDelete.end());


    for (auto& obj : objectsToRemove)
    {
        m_allObjects->removeObject(obj);
        if (m_threadWorker)
        {
            for (size_t i = 0; i < m_threadGroups.size(); ++i)
            {
                m_threadGroups[i]->removeObject(obj);
            }
        }
        m_renderLayerGroup.removeObject(obj, obj->getRenderLayer());
    }
    

    for (auto& obj : objectsToDelete)
    {
		delete obj;
	}
   
    m_allObjects->reserveObjectsCount(m_allObjects->getObjectsCount() + objectsToAdd.size());
    for (auto& obj : objectsToAdd)
    {
        if (obj->getCanvasParent() != m_parent && obj->getCanvasParent())
            obj->getCanvasParent()->removeObject(obj);
        m_allObjects->addObject(obj);
        if (m_threadWorker)
        {
            m_threadGroups[m_currentThreadGroupInsertIndex]->addObject(obj);
            m_currentThreadGroupInsertIndex = (m_currentThreadGroupInsertIndex + 1) % m_threadGroupCount;
        }
        m_renderLayerGroup.addObject(obj, obj->getRenderLayer());

        obj->setCanvasParent(m_parent);
    }
    if (m_parent)
        m_parent->setRootCanvesObject(m_allObjects->getObjectsCount());
    
    updateNewElements();
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
Objects::CanvasObject* CanvasObjectContainer::findFirstObject(const std::string& name)
{
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string &objName = obj->getName();
        if(objName.size() != nameSize)
			continue;
		if(objName == name)
			return obj;
    }
    return nullptr;
}
std::vector<Objects::CanvasObject*> CanvasObjectContainer::findAllObjects(const std::string& name)
{
    std::vector<Objects::CanvasObject*> results;
    results.reserve(20);
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
            continue;
        if (objName == name)
            results.push_back(obj);
    }
    return results;
}
Objects::CanvasObject* CanvasObjectContainer::findFirstObjectRecursive(const std::string& name)
{
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
        {
			Objects::CanvasObject* child = obj->findFirstChildRecursive(name);
			if (child)
				return child;
            continue;
        }
        if (objName == name)
            return obj;
    }
    return nullptr;
}
std::vector<Objects::CanvasObject*> CanvasObjectContainer::findAllObjectsRecursive(const std::string& name)
{
    std::vector<Objects::CanvasObject*> results;
    results.reserve(50);
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
        {
            std::vector<CanvasObject*> childs = obj->findAllChildsRecursive(name);
            if (childs.size())
                results.insert(results.end(), childs.begin(), childs.end());
            continue;
        }
        if (objName == name)
            results.push_back(obj);
    }
    return results;
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
    if (obj->m_canvasParent != m_parent)
        return; // not owner of this object
    if(from < RenderLayer::count)
        m_renderLayerGroup.removeObject(obj, from);
    if(to < RenderLayer::count)
        m_renderLayerGroup.addObject(obj, to);
}
void CanvasObjectContainer::setRenderLayer(Objects::CanvasObject* obj, RenderLayer to)
{
    if (!obj)
        return;
    if (obj->m_canvasParent != m_parent)
        return; // not owner of this object
    m_renderLayerGroup.removeObject(obj);
    m_renderLayerGroup.addObject(obj, to);
}

std::string CanvasObjectContainer::getObjectsTreeString() const
{
    std::string str;
    for(const auto &obj : m_allObjects->getObjects())
	{
		str += obj->toString();
	}
	return str;
}


void CanvasObjectContainer::updateNewElements()
{
    QSFMLP_CANVAS_FUNCTION(QSFML_COLOR_STAGE_1);
    
    deleteObject_internal();
    
    const std::vector<Objects::CanvasObject*> &toAdd = m_allObjects->getObjectsToAdd();

    addObject_internal();
    m_allObjects->updateNewElements();
   // m_renderLayerGroup.updateNewElements();
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
    size_t size = m_renderLayerGroup.size();
    for(size_t i=0; i < size; ++i)
    {
        QSFMLP_CANVAS_BLOCK("Draw layer", QSFML_COLOR_STAGE_1);
        m_renderLayerGroup[i].draw(window);
        QSFML_PROFILING_END_BLOCK;
    }
}

