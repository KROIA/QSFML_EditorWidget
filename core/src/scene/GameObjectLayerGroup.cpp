#include "Scene/GameObjectLayerGroup.h"


namespace QSFML
{
    GameObjectLayerGroup::GameObjectLayerGroup(Scene* parent, size_t layerCount)
        : m_parent(parent)
        , m_layerSize(layerCount)
    {
        m_container.reserve(m_layerSize);
        for (size_t i = 0; i < m_layerSize; ++i)
            m_container.push_back(new GameObjectGroup(m_parent));
    }
    GameObjectLayerGroup::~GameObjectLayerGroup()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            delete m_container[i];
    }

    size_t GameObjectLayerGroup::size() const
    {
        return m_container.size();
    }
    GameObjectGroup& GameObjectLayerGroup::operator[](size_t index)
    {
        return *m_container[index];
    }

    void GameObjectLayerGroup::addObject(Objects::GameObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->addObject(obj);
    }
    void GameObjectLayerGroup::addObject(const std::vector<Objects::GameObject*>& objs, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->addObject(objs);
    }

    void GameObjectLayerGroup::removeObject(Objects::GameObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->removeObject(obj);
    }
    void GameObjectLayerGroup::removeObject(const std::vector<Objects::GameObject*>& objs)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->removeObject(objs);
    }
    void GameObjectLayerGroup::removeObject(Objects::GameObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->removeObject(obj);
    }
    void GameObjectLayerGroup::removeObject(const std::vector<Objects::GameObject*>& objs, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->removeObject(objs);
    }


    void GameObjectLayerGroup::deleteObject(Objects::GameObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject(obj);
    }
    void GameObjectLayerGroup::deleteObject(const std::vector<Objects::GameObject*>& objs)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject(objs);
    }


    void GameObjectLayerGroup::clearObjects(size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->clearObjects();
    }
    void GameObjectLayerGroup::clearObjects()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->clearObjects();
    }

    void GameObjectLayerGroup::reserveObjectsCount(size_t size)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->reserveObjectsCount(size);
    }
    size_t GameObjectLayerGroup::getObjectsCount() const
    {
        size_t count = 0;
        for (size_t i = 0; i < m_container.size(); ++i)
            count += m_container[i]->getObjectsCount();
        return count;
    }
    size_t GameObjectLayerGroup::getObjectsCount(size_t layer) const
    {
        if (layer > m_layerSize) return 0;
        return m_container[layer]->getObjectsCount();
    }


    const std::vector<Objects::GameObject*> GameObjectLayerGroup::getObjects() const
    {
        std::vector<Objects::GameObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::GameObject*>& o = m_container[i]->getObjects();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::GameObject*>&  GameObjectLayerGroup::getObjects(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::GameObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjects();
    }
    const std::vector<Objects::GameObject*> GameObjectLayerGroup::getObjectsToAdd() const
    {
        std::vector<Objects::GameObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::GameObject*>& o = m_container[i]->getObjectsToAdd();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::GameObject*>&  GameObjectLayerGroup::getObjectsToAdd(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::GameObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjectsToAdd();
    }
    const std::vector<Objects::GameObject*> GameObjectLayerGroup::getObjectsToDelete() const
    {
        std::vector<Objects::GameObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::GameObject*>& o = m_container[i]->getObjectsToDelete();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::GameObject*>&  GameObjectLayerGroup::getObjectsToDelete(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::GameObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjectsToDelete();
    }

    bool GameObjectLayerGroup::objectExists(Objects::GameObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            if (m_container[i]->objectExists(obj))
                return true;            
        }
        return false;
    }
    bool GameObjectLayerGroup::objectExists(Objects::GameObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return false;
        return m_container[layer]->objectExists(obj);
    }
    size_t GameObjectLayerGroup::getObjectIndex(Objects::GameObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return 0;
        return m_container[layer]->getObjectIndex(obj);
    }


    void GameObjectLayerGroup::deleteLater(Objects::GameObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteLater(obj);
    }

    void GameObjectLayerGroup::updateNewElements()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->updateNewElements();
    }
    void GameObjectLayerGroup::sfEvent(const std::vector<sf::Event>& events)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->sfEvent(events);
    }
    void GameObjectLayerGroup::update()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->update();
    }
    void GameObjectLayerGroup::draw(sf::RenderWindow& window)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->draw(window);
    }



    void GameObjectLayerGroup::addObject_internal()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->addObject_internal();
    }
    void GameObjectLayerGroup::deleteObject_internal()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject_internal();
    }
}
