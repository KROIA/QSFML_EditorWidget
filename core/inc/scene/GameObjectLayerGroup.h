#pragma once

#include "QSFML_EditorWidget_base.h"
#include "GameObjectGroup.h"

#include <vector>

namespace QSFML
{
    class QSFML_EDITOR_WIDGET_API GameObjectLayerGroup
    {
        friend GameObjectContainer;
        friend Scene;
    public:
        GameObjectLayerGroup(Scene* parent, size_t layerCount);
        GameObjectLayerGroup(const GameObjectLayerGroup& other) = delete;
        ~GameObjectLayerGroup();

        GameObjectLayerGroup& operator=(const GameObjectLayerGroup& other) = delete;

        size_t size() const;
        GameObjectGroup& operator[](size_t index);

        void addObject(Objects::GameObjectPtr obj, size_t layer);
        void addObject(const QSFML::vector<Objects::GameObjectPtr>& objs, size_t layer);

        void removeObject(Objects::GameObjectPtr obj);
        void removeObject(const QSFML::vector<Objects::GameObjectPtr>& objs);
        void removeObject(Objects::GameObjectPtr obj, size_t layer);
        void removeObject(const QSFML::vector<Objects::GameObjectPtr>& objs, size_t layer);


        void deleteObject(Objects::GameObjectPtr obj);
        void deleteObject(const QSFML::vector<Objects::GameObjectPtr>& objs);

        void clearObjects(size_t layer);
        void clearObjects();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        size_t getObjectsCount(size_t layer) const;
        template<typename T>
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount(size_t layer) const;
        const QSFML::vector<Objects::GameObjectPtr> getObjects() const;
        const QSFML::vector<Objects::GameObjectPtr>& getObjects(size_t layer) const;
        const QSFML::vector<Objects::GameObjectPtr> getObjectsToAdd() const;
        const QSFML::vector<Objects::GameObjectPtr>& getObjectsToAdd(size_t layer) const;
        const QSFML::vector<Objects::GameObjectPtr> getObjectsToDelete() const;
        const QSFML::vector<Objects::GameObjectPtr>& getObjectsToDelete(size_t layer) const;
        template<typename T>
        QSFML::vector<T*> getObjects() const;
        template<typename T>
        QSFML::vector<T*> getObjects(size_t layer) const;
        template<typename T>
        std::shared_ptr<T> getFirstObject() const;
        template<typename T>
        std::shared_ptr<T> getFirstObject(size_t layer) const;

        bool objectExists(Objects::GameObjectPtr obj);
        bool objectExists(Objects::GameObjectPtr obj, size_t layer);
        size_t getObjectIndex(Objects::GameObjectPtr obj, size_t layer);


        void deleteLater(Objects::GameObjectPtr obj);

        template<typename T>
        static bool objectExists(std::shared_ptr<T> obj, const QSFML::vector<std::shared_ptr<T>>& list);

        template<typename T>
        static size_t getObjectIndex(std::shared_ptr<T> obj, const QSFML::vector<std::shared_ptr<T>>& list);

        template<typename T>
        static void addObject(Objects::GameObjectPtr obj, QSFML::vector<std::shared_ptr<T>>& list);

        template<typename T>
        static void removeObject(Objects::GameObjectPtr obj, QSFML::vector<std::shared_ptr<T>>& list);

        const static size_t npos = -1;


    private:

        void updateNewElements();
        void sfEvent(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events);
        void update();
        void draw(sf::RenderWindow& window);



        void addObject_internal();
        void deleteObject_internal();


        Scene* m_parent;
        size_t m_layerSize;
        QSFML::vector<GameObjectGroup*> m_container;

       // QSFML::vector<Objects::GameObjectPtr> m_toDelete;
    };

    template<typename T>
    size_t GameObjectLayerGroup::getObjectsCount() const
    {
        size_t count = 0;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            count += m_container[i]->getObjectsCount<T>();
        }
        return count;
    }
    template<typename T>
    size_t GameObjectLayerGroup::getObjectsCount(size_t layer) const
    {
        if (layer > m_layerSize)
            return 0;
        return m_container[layer]->getObjectsCount<T>();
    }
    template<typename T>
    QSFML::vector<T*> GameObjectLayerGroup::getObjects() const
    {
        QSFML::vector<T*> list;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            QSFML::vector<T*>buff = m_container[i]->getObjects();
            list.insert(list.end, buff.begin(), buff.end());
        }
        return list;
    }
    template<typename T>
    QSFML::vector<T*> GameObjectLayerGroup::getObjects(size_t layer) const
    {
        if (layer > m_layerSize)
            return QSFML::vector<T*>();

        return m_container[layer]->getObjects<T>();
    }
    template<typename T>
    std::shared_ptr<T> GameObjectLayerGroup::getFirstObject() const
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            T* obj = m_container[i]->getFirstObject<T>();
            if (obj)
                return obj;
        }
        return nullptr;
    }
    template<typename T>
    std::shared_ptr<T> GameObjectLayerGroup::getFirstObject(size_t layer) const
    {
        if (layer > m_layerSize)
            return nullptr;
        return m_container[layer]->getFirstObject<T>();
    }
    template<typename T>
    bool GameObjectLayerGroup::objectExists(std::shared_ptr<T> obj, const QSFML::vector<std::shared_ptr<T>>& list)
    {
        QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
        size_t num = list.size();
        if (!num) return false;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (obj == *it) return true;
        }
        return false;
    }

    template<typename T>
    size_t GameObjectLayerGroup::getObjectIndex(std::shared_ptr<T> obj, const QSFML::vector<std::shared_ptr<T>>& list)
    {
        QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_2);
        size_t num = list.size();
        if (!num) return npos;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (obj == *it) return it - list.begin();
        }
        return npos;
    }
    template<typename T>
    void GameObjectLayerGroup::addObject(QSFML::Objects::GameObjectPtr obj, QSFML::vector<std::shared_ptr<T>>& list)
    {
        QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_3);
        T* transformed = dynamic_cast<T*>(obj);
        if (transformed)
        {
            list.push_back(transformed);
        }
    }

    template<typename T>
    void GameObjectLayerGroup::removeObject(QSFML::Objects::GameObjectPtr obj, QSFML::vector<std::shared_ptr<T>>& list)
    {
        QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_4);
        T* transformed = dynamic_cast<T*>(obj);
        if (transformed)
        {
            size_t index = getObjectIndex<T>(transformed, list);
            if (index != npos)
                list.erase(list.begin() + index);
        }
    }

}
