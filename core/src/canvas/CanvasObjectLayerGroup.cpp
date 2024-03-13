#include "Canvas/CanvasObjectLayerGroup.h"


namespace QSFML
{
    CanvasObjectLayerGroup::CanvasObjectLayerGroup(Canvas* parent, size_t layerCount)
        : m_parent(parent)
        , m_layerSize(layerCount)
    {
        m_container.reserve(m_layerSize);
        for (size_t i = 0; i < m_layerSize; ++i)
            m_container.push_back(new CanvasObjectGroup(m_parent));
    }
    CanvasObjectLayerGroup::~CanvasObjectLayerGroup()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            delete m_container[i];
    }

    size_t CanvasObjectLayerGroup::size() const
    {
        return m_container.size();
    }
    CanvasObjectGroup& CanvasObjectLayerGroup::operator[](size_t index)
    {
        return *m_container[index];
    }

    void CanvasObjectLayerGroup::addObject(Objects::CanvasObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->addObject(obj);
    }
    void CanvasObjectLayerGroup::addObject(const std::vector<Objects::CanvasObject*>& objs, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->addObject(objs);
    }

    void CanvasObjectLayerGroup::removeObject(Objects::CanvasObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->removeObject(obj);
    }
    void CanvasObjectLayerGroup::removeObject(const std::vector<Objects::CanvasObject*>& objs)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->removeObject(objs);
    }
    void CanvasObjectLayerGroup::removeObject(Objects::CanvasObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->removeObject(obj);
    }
    void CanvasObjectLayerGroup::removeObject(const std::vector<Objects::CanvasObject*>& objs, size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->removeObject(objs);
    }


    void CanvasObjectLayerGroup::deleteObject(Objects::CanvasObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject(obj);
    }
    void CanvasObjectLayerGroup::deleteObject(const std::vector<Objects::CanvasObject*>& objs)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject(objs);
    }


    void CanvasObjectLayerGroup::clearObjects(size_t layer)
    {
        if (layer > m_layerSize) return;
        m_container[layer]->clearObjects();
    }
    void CanvasObjectLayerGroup::clearObjects()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->clearObjects();
    }

    void CanvasObjectLayerGroup::reserveObjectsCount(size_t size)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->reserveObjectsCount(size);
    }
    size_t CanvasObjectLayerGroup::getObjectsCount() const
    {
        size_t count = 0;
        for (size_t i = 0; i < m_container.size(); ++i)
            count += m_container[i]->getObjectsCount();
        return count;
    }
    size_t CanvasObjectLayerGroup::getObjectsCount(size_t layer) const
    {
        if (layer > m_layerSize) return 0;
        return m_container[layer]->getObjectsCount();
    }


    const std::vector<Objects::CanvasObject*> CanvasObjectLayerGroup::getObjects() const
    {
        std::vector<Objects::CanvasObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::CanvasObject*>& o = m_container[i]->getObjects();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::CanvasObject*>&  CanvasObjectLayerGroup::getObjects(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::CanvasObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjects();
    }
    const std::vector<Objects::CanvasObject*> CanvasObjectLayerGroup::getObjectsToAdd() const
    {
        std::vector<Objects::CanvasObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::CanvasObject*>& o = m_container[i]->getObjectsToAdd();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::CanvasObject*>&  CanvasObjectLayerGroup::getObjectsToAdd(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::CanvasObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjectsToAdd();
    }
    const std::vector<Objects::CanvasObject*> CanvasObjectLayerGroup::getObjectsToDelete() const
    {
        std::vector<Objects::CanvasObject*> objs;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            const std::vector<Objects::CanvasObject*>& o = m_container[i]->getObjectsToDelete();
            objs.insert(objs.end(), o.begin(), o.end());
        }
        return objs;
    }
    const std::vector<Objects::CanvasObject*>&  CanvasObjectLayerGroup::getObjectsToDelete(size_t layer) const
    {
        if (layer > m_layerSize)
        {
            static std::vector<Objects::CanvasObject*> dummy;
            return dummy;
        }
        return m_container[layer]->getObjectsToDelete();
    }

    bool CanvasObjectLayerGroup::objectExists(Objects::CanvasObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            if (m_container[i]->objectExists(obj))
                return true;            
        }
        return false;
    }
    bool CanvasObjectLayerGroup::objectExists(Objects::CanvasObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return false;
        return m_container[layer]->objectExists(obj);
    }
    size_t CanvasObjectLayerGroup::getObjectIndex(Objects::CanvasObject* obj, size_t layer)
    {
        if (layer > m_layerSize) return 0;
        return m_container[layer]->getObjectIndex(obj);
    }


    void CanvasObjectLayerGroup::deleteLater(Objects::CanvasObject* obj)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteLater(obj);
    }

    void CanvasObjectLayerGroup::updateNewElements()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->updateNewElements();
    }
    void CanvasObjectLayerGroup::sfEvent(const std::vector<sf::Event>& events)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->sfEvent(events);
    }
    void CanvasObjectLayerGroup::update()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->update();
    }
    void CanvasObjectLayerGroup::draw(sf::RenderWindow& window)
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->draw(window);
    }



    void CanvasObjectLayerGroup::addObject_internal()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->addObject_internal();
    }
    void CanvasObjectLayerGroup::deleteObject_internal()
    {
        for (size_t i = 0; i < m_container.size(); ++i)
            m_container[i]->deleteObject_internal();
    }
}
