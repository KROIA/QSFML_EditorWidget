#include "Scene/GameObjectGroup.h"
#include "Scene/Scene.h"
#include "utilities/LifetimeChecker.h"

#include "objects/base/GameObject.h"


using namespace QSFML::Objects;
using namespace QSFML::Components;

namespace QSFML
{

    GameObjectGroup::GameObjectGroup(Scene* parent)
    {
        m_parent = parent;
    }
    GameObjectGroup::~GameObjectGroup()
    {
        cleanup();
    }

    void GameObjectGroup::addObject(GameObjectPtr obj)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);

        if (obj->getSceneParent() != m_parent && obj->getSceneParent())
            obj->getSceneParent()->removeObject(obj);
        m_toAddContainer.push_back(obj);
        obj->setSceneParent(m_parent);
    }
    void GameObjectGroup::addObject(const QSFML::vector<GameObjectPtr>& objs)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        for (size_t i = 0; i < objs.size(); ++i)
        {
            addObject(objs[i]);
        }
    }
    void GameObjectGroup::addObject_internal()
    {
        m_container.reserve(m_container.size() + m_toAddContainer.size());

        for(auto & obj : m_toAddContainer)
		{
			if (objectExists(obj))
				obj = nullptr;
		}
        for (auto& obj : m_toAddContainer)
        {
            if (obj)
                m_container.push_back(obj);
        }
        m_toAddContainer.clear();
    }
    void GameObjectGroup::deleteObject_internal()
    {
        for (size_t i = 0; i < m_toDelete.size(); ++i)
        {
            deleteObject(m_toDelete[i]);
        }
        m_toDelete.clear();
    }

    void GameObjectGroup::removeObject(GameObjectPtr obj)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        size_t index = getObjectIndex(obj);
        if (index == npos) return;
        obj->setSceneParent(nullptr);
        m_container.erase(m_container.begin() + index);
    }
    void GameObjectGroup::removeObject(const QSFML::vector<GameObjectPtr>& objs)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        for (size_t i = 0; i < objs.size(); ++i)
        {
            removeObject(objs[i]);
        }
    }
    void GameObjectGroup::deleteObject(Objects::GameObjectPtr obj)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        removeObject(obj);
        Internal::LifetimeChecker::deleteSecured(obj);
    }
    void GameObjectGroup::deleteObject(const QSFML::vector<Objects::GameObjectPtr>& objs)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        removeObject(objs);
        for (size_t i = 0; i < objs.size(); ++i)
        {
            Internal::LifetimeChecker::deleteSecured(objs[i]);
        }
    }
    void GameObjectGroup::clearObjects()
    {
        m_toAddContainer.clear();
        m_container.clear();
    }

    void GameObjectGroup::cleanup()
    {
        if(m_container.empty() && m_toAddContainer.empty() && m_toDelete.empty()) 
            return;
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        for (auto& obj : m_toAddContainer)
        {
            obj->setSceneParent(nullptr);
            Internal::LifetimeChecker::deleteSecured(obj);
        }
        m_toAddContainer.clear();
        for (auto& obj : m_container)
        {
            obj->setSceneParent(nullptr);
            Internal::LifetimeChecker::deleteSecured(obj);
        }
        m_container.clear();
        m_toDelete.clear();
    }

    void GameObjectGroup::reserveObjectsCount(size_t size)
    {
        m_container.reserve(size);
    }
    size_t GameObjectGroup::getObjectsCount() const
    {
        return m_container.size();
    }

    const QSFML::vector<GameObjectPtr>& GameObjectGroup::getObjects() const
    {
        return m_container;
    }
    const QSFML::vector<Objects::GameObjectPtr>& GameObjectGroup::getObjectsToAdd() const
    {
        return m_toAddContainer;
    }
    const QSFML::vector<Objects::GameObjectPtr>& GameObjectGroup::getObjectsToDelete() const
    {
        return m_toDelete;
    }

    bool GameObjectGroup::objectExists(GameObjectPtr obj)
    {
        return objectExists<Objects::GameObject>(obj, m_container);
    }
    size_t GameObjectGroup::getObjectIndex(GameObjectPtr obj)
    {
        return getObjectIndex<Objects::GameObject>(obj, m_container);
    }
    void GameObjectGroup::deleteLater(Objects::GameObjectPtr obj)
    {
        for (size_t i = 0; i < m_toAddContainer.size(); ++i)
        {
            if (m_toAddContainer[i] == obj)
            {
                m_toAddContainer.erase(m_toAddContainer.begin() + i);
                break;
            }
        }
        m_toDelete.push_back(obj);
    }

    void GameObjectGroup::updateNewElements()
    {
        deleteObject_internal();
        addObject_internal();
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            if (m_container[i]->objectsChanged())
                m_container[i]->updateObjectChanges();
        }
    }
    void GameObjectGroup::sfEvent(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            m_container[i]->sfEvent(events);
        }
    }
    void GameObjectGroup::update()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            m_container[i]->update_internal();
        }
    }
    void GameObjectGroup::draw(sf::RenderWindow& window)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            sf::RenderStates states;
            m_container[i]->draw(window, states);
        }
    }

}
