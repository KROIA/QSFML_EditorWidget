#include "Scene/GameObjectContainer.h"
#include "Scene/Scene.h"
#include "utilities/Stats.h"
#include "utilities/LifetimeChecker.h"
#include "Scene/GameObjectGroup.h"
#include "Scene/SceneThreadWorker.h"

#include "objects/base/GameObject.h"
#include "objects/CameraController.h"
#include "objects/CameraWindow.h"

#include "components/base/Drawable.h"
#include <thread>



using namespace QSFML::Objects;
using namespace QSFML::Components;
using namespace QSFML;

GameObjectContainer::GameObjectContainer(Scene* parent, SceneSettings& settings)
    : m_settings(settings)
    , m_renderLayerGroup(parent, RenderLayer::count)
{
    m_parent = parent;
    m_allObjects = new GameObjectGroup(m_parent);
    m_parent->setRootGameObject(m_allObjects->getObjectsCount());

    m_currentThreadGroupInsertIndex = 0;
    m_threadWorker = nullptr;
    m_threadGroupCount = 0;
}
GameObjectContainer::~GameObjectContainer()
{
    cleanup();
}


void GameObjectContainer::initializeThreads(size_t threadCount)
{
    if (m_threadWorker)
        return;
    m_currentThreadGroupInsertIndex = 0;
    m_threadGroupCount = m_settings.updateControlls.threadSettings.objectGroups;
    for (size_t i = 0; i < m_threadGroupCount; ++i)
    {
        m_threadGroups.push_back(new GameObjectGroup(m_parent));
    }
    const QSFML::vector<GameObjectPtr>& objs = m_allObjects->getObjects();
    for (size_t i = 0; i < objs.size(); ++i)
    {
        m_threadGroups[i % m_threadGroups.size()]->addObject(objs[i]);
    }
    for (size_t i = 0; i < m_threadGroups.size(); ++i)
    {
        m_threadGroups[i]->addObject_internal();
    }
    m_threadWorker = new SceneThreadWorker(threadCount, &m_threadGroups);
}
void GameObjectContainer::deinitializeThreads()
{
    SceneThreadWorker* currentWorker = m_threadWorker;
    m_threadWorker = nullptr;

    for (size_t i = 0; i < m_threadGroups.size(); ++i)
    {
        delete m_threadGroups[i];
    }
    m_threadGroups.clear();
    m_threadGroupCount = 0;
    delete currentWorker;
}

void GameObjectContainer::addObject(GameObjectPtr obj)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToAdd.push_back(obj);
}
void GameObjectContainer::addObject(const QSFML::vector<GameObjectPtr>& objs)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToAdd.insert(m_objectsToAdd.end(), objs.begin(), objs.end());
}
void GameObjectContainer::addObject_internal()
{
    m_allObjects->addObject_internal();
    if (m_threadWorker)
    {
        for (auto& group : m_threadGroups)
        {
            group->addObject_internal();
        }
    }
    m_renderLayerGroup.addObject_internal();
    m_parent->setRootGameObject(m_allObjects->getObjectsCount());
}
void GameObjectContainer::deleteObject_internal()
{
    m_allObjects->deleteObject_internal();
    m_renderLayerGroup.deleteObject_internal();
    m_parent->setRootGameObject(m_allObjects->getObjectsCount());
}

void GameObjectContainer::removeObject(GameObjectPtr obj)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToRemove.push_back(obj);
}
void GameObjectContainer::removeObject(const QSFML::vector<GameObjectPtr>& objs)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToRemove.insert(m_objectsToRemove.end(), objs.begin(), objs.end());
}
void GameObjectContainer::deleteObject(Objects::GameObjectPtr obj)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToDelete.push_back(obj);
}
void GameObjectContainer::deleteObject(const QSFML::vector<Objects::GameObjectPtr>& objs)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    m_objectsToDelete.insert(m_objectsToRemove.end(), objs.begin(), objs.end());
}
void GameObjectContainer::clearObjects()
{
    m_allObjects->clearObjects();
    m_renderLayerGroup.clearObjects();
    m_parent->setRootGameObject(m_allObjects->getObjectsCount());
}
void GameObjectContainer::cleanup()
{
    if (!m_allObjects)
        return; // Already cleaned up
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);

    for (auto& group : m_threadGroups)
    {
        group->clearObjects();
        delete group;
    }
    m_allObjects->cleanup();
    delete m_allObjects;
    delete m_threadWorker;

    QSFML::vector<Objects::GameObjectPtr> objs = m_allObjects->getObjects();
    m_allObjects->clearObjects();
    m_renderLayerGroup.clearObjects();
    for (auto& obj : objs)
    {
        obj->setSceneParent(nullptr);
        delete obj;
    }
    m_allObjects = nullptr;
    m_threadWorker = nullptr;
    m_threadGroups.clear();
    m_parent->setRootGameObject(0);
}

void GameObjectContainer::applyObjectChanges()
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFML::vector<GameObjectPtr> objectsToAdd = m_objectsToAdd;
    QSFML::vector<GameObjectPtr> objectsToRemove = m_objectsToRemove;
    QSFML::vector<GameObjectPtr> objectsToDelete = m_objectsToDelete;
    QSFML::vector<GameObjectPtr> changedObjects;
    {
        std::unique_lock<std::mutex> lock(m_objectsWithChangesMutex);
        changedObjects = m_objectsWithChanges;
        m_objectsWithChanges.clear();
    }
    if (changedObjects.size())
    {
        for (size_t i = 0; i < changedObjects.size(); ++i)
        {
            changedObjects[i]->updateObjectChanges();
        }
    }
    m_renderLayerGroup.updateNewElements();
    if (objectsToAdd.empty() && objectsToRemove.empty() && objectsToDelete.empty())
        return;
    m_objectsToAdd.clear();
    m_objectsToRemove.clear();
    m_objectsToDelete.clear();
    objectsToRemove.insert(objectsToRemove.end(), objectsToDelete.begin(), objectsToDelete.end());


    for (auto& obj : objectsToRemove)
    {
        // Check if it is a camera
        Objects::CameraWindow* cam = dynamic_cast<Objects::CameraWindow*>(obj);

        if (cam)
        {
            m_parent->m_cameras.removeCamera(cam);
        }
        QSFML::vector< Objects::CameraWindow*> cams = obj->getChildsRecusrive<Objects::CameraWindow>();
        for (auto& cam1 : cams)
        {
            m_parent->m_cameras.removeCamera(cam1);
        }


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
        Objects::CameraWindow* cam = dynamic_cast<Objects::CameraWindow*>(obj);
        if (cam)
        {
            m_parent->m_cameras.removeCamera(cam);
        }
        QSFML::vector< Objects::CameraWindow*> cams = obj->getChildsRecusrive<Objects::CameraWindow>();
        for (auto& cam1 : cams)
        {
            m_parent->m_cameras.removeCamera(cam1);
        }
        Internal::LifetimeChecker::deleteSecured(obj);
    }

    m_allObjects->reserveObjectsCount(m_allObjects->getObjectsCount() + objectsToAdd.size());
    for (auto& obj : objectsToAdd)
    {
        if (obj->getSceneParent() != m_parent && obj->getSceneParent())
            obj->getSceneParent()->removeObject(obj);
        m_allObjects->addObject(obj);
        if (m_threadWorker)
        {
            m_threadGroups[m_currentThreadGroupInsertIndex]->addObject(obj);
            m_currentThreadGroupInsertIndex = (m_currentThreadGroupInsertIndex + 1) % m_threadGroupCount;
        }
        m_renderLayerGroup.addObject(obj, obj->getRenderLayer());

        obj->setSceneParent(m_parent);
    }
    if (m_parent)
        m_parent->setRootGameObject(m_allObjects->getObjectsCount());

    updateNewElements();
    for (auto& obj : objectsToAdd)
    {
        // Check if it is a camera
        Objects::CameraWindow* cam = dynamic_cast<Objects::CameraWindow*>(obj);
        if (cam && m_parent)
        {
            m_parent->m_cameras.addCamera(cam);
        }
        QSFML::vector< Objects::CameraWindow*> cams = obj->getChildsRecusrive<Objects::CameraWindow>();
        for (auto& cam1 : cams)
        {
            m_parent->m_cameras.addCamera(cam1);
        }
    }
}

void GameObjectContainer::reserveObjectsCount(size_t size)
{
    m_allObjects->reserveObjectsCount(size);
    m_renderLayerGroup.reserveObjectsCount(size);
}
size_t GameObjectContainer::getObjectsCount() const
{
    return m_allObjects->getObjectsCount();
}

const QSFML::vector<GameObjectPtr>& GameObjectContainer::getObjects() const
{
    return m_allObjects->getObjects();
}

bool GameObjectContainer::objectExists(GameObjectPtr obj)
{
    if (obj->getRenderLayer() < RenderLayer::count)
        return m_renderLayerGroup.objectExists(obj, obj->getRenderLayer());

    return m_allObjects->objectExists(obj);
}
size_t GameObjectContainer::getObjectIndex(GameObjectPtr obj)
{
    if (obj->getRenderLayer() < RenderLayer::count)
        return m_renderLayerGroup.getObjectIndex(obj, obj->getRenderLayer());

    return m_allObjects->getObjectIndex(obj);
}
Objects::GameObjectPtr GameObjectContainer::getFirstObject(const std::string& name)
{
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
            continue;
        if (objName == name)
            return obj;
    }
    return nullptr;
}
QSFML::vector<Objects::GameObjectPtr> GameObjectContainer::getAllObjects(const std::string& name)
{
    QSFML::vector<Objects::GameObjectPtr> results;
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
Objects::GameObjectPtr GameObjectContainer::getFirstObjectRecursive(const std::string& name)
{
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
        {
            Objects::GameObjectPtr child = obj->getFirstChildRecursive(name);
            if (child)
                return child;
            continue;
        }
        if (objName == name)
            return obj;
    }
    return nullptr;
}
QSFML::vector<Objects::GameObjectPtr> GameObjectContainer::getAllObjectsRecursive(const std::string& name)
{
    QSFML::vector<Objects::GameObjectPtr> results;
    results.reserve(50);
    size_t nameSize = name.size();
    for (auto& obj : m_allObjects->getObjects())
    {
        const std::string& objName = obj->getName();
        if (objName.size() != nameSize)
        {
            QSFML::vector<GameObjectPtr> childs = obj->getAllChildsRecursive(name);
            if (childs.size())
                results.insert(results.end(), childs.begin(), childs.end());
            continue;
        }
        if (objName == name)
            results.push_back(obj);
    }
    return results;
}

void GameObjectContainer::deleteLater(Objects::GameObjectPtr obj)
{
    m_allObjects->deleteLater(obj);
    if (obj->getRenderLayer() < RenderLayer::count)
        m_renderLayerGroup.removeObject(obj, obj->getRenderLayer());
}
void GameObjectContainer::renderLayerSwitch(Objects::GameObjectPtr obj, RenderLayer from, RenderLayer to)
{
    if (!obj)
        return;
    if (obj->m_sceneParent != m_parent)
        return; // not owner of this object
    if (from < RenderLayer::count)
        m_renderLayerGroup.removeObject(obj, from);
    if (to < RenderLayer::count)
        m_renderLayerGroup.addObject(obj, to);
}
void GameObjectContainer::setRenderLayer(Objects::GameObjectPtr obj, RenderLayer to)
{
    if (!obj)
        return;
    if (obj->m_sceneParent != m_parent)
        return; // not owner of this object
    m_renderLayerGroup.removeObject(obj);
    m_renderLayerGroup.addObject(obj, to);
}

std::string GameObjectContainer::getObjectsTreeString() const
{
    std::string str;
    for (const auto& obj : m_allObjects->getObjects())
    {
        str += obj->toString();
    }
    return str;
}


void GameObjectContainer::updateNewElements()
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);

    deleteObject_internal();

    QSFML::vector<Objects::GameObjectPtr> toAdd = m_allObjects->getObjectsToAdd();

    addObject_internal();
    m_allObjects->updateNewElements();
    // m_renderLayerGroup.updateNewElements();
    for (auto obj : toAdd)
        obj->inSceneAdded_internal();
}
void GameObjectContainer::sfEvent(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events)
{
    m_allObjects->sfEvent(events);
}
void GameObjectContainer::update()
{
    if (m_parent->m_settings.updateControlls.enableMultithreading)
    {
        if (!m_threadWorker)
            initializeThreads(m_parent->m_settings.updateControlls.threadSettings.threadCount);
        m_threadWorker->process();
    }
    else
    {
        m_allObjects->update();
    }
}
void GameObjectContainer::draw(sf::RenderWindow& window)
{
    size_t size = m_renderLayerGroup.size();
    for (size_t i = 0; i < size; ++i)
    {
        QSFMLP_SCENE_BLOCK("Draw layer", QSFML_COLOR_STAGE_1);
        m_renderLayerGroup[i].draw(window);
        QSFML_PROFILING_END_BLOCK;
    }
}
void GameObjectContainer::objectHasChanged(Objects::GameObjectPtr obj)
{
    std::unique_lock<std::mutex> lock(m_objectsWithChangesMutex);
    m_objectsWithChanges.push_back(obj);
}

