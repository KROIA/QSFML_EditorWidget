#pragma once

#include "QSFML_base.h"
#include "CanvasObjectGroup.h"


namespace QSFML
{
    class QSFML_EDITOR_WIDGET_EXPORT CanvasObjectLayerGroup
    {
        friend CanvasObjectContainer;
        friend Canvas;
    public:
        CanvasObjectLayerGroup(Canvas* parent, size_t layerCount);
        CanvasObjectLayerGroup(const CanvasObjectLayerGroup& other) = delete;
        ~CanvasObjectLayerGroup();

        CanvasObjectLayerGroup& operator=(const CanvasObjectLayerGroup& other) = delete;

        size_t size() const;
        CanvasObjectGroup& operator[](size_t index);

        void addObject(Objects::CanvasObject* obj, size_t layer);
        void addObject(const std::vector<Objects::CanvasObject*>& objs, size_t layer);

        void removeObject(Objects::CanvasObject* obj);
        void removeObject(const std::vector<Objects::CanvasObject*>& objs);
        void removeObject(Objects::CanvasObject* obj, size_t layer);
        void removeObject(const std::vector<Objects::CanvasObject*>& objs, size_t layer);


        void deleteObject(Objects::CanvasObject* obj);
        void deleteObject(const std::vector<Objects::CanvasObject*>& objs);

        void clearObjects(size_t layer);
        void clearObjects();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        size_t getObjectsCount(size_t layer) const;
        template<typename T>
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount(size_t layer) const;
        const std::vector<Objects::CanvasObject*> getObjects() const;
        const std::vector<Objects::CanvasObject*>& getObjects(size_t layer) const;
        const std::vector<Objects::CanvasObject*> getObjectsToAdd() const;
        const std::vector<Objects::CanvasObject*>& getObjectsToAdd(size_t layer) const;
        const std::vector<Objects::CanvasObject*> getObjectsToDelete() const;
        const std::vector<Objects::CanvasObject*>& getObjectsToDelete(size_t layer) const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        std::vector<T*> getObjects(size_t layer) const;
        template<typename T>
        T* getFirstObject() const;
        template<typename T>
        T* getFirstObject(size_t layer) const;

        bool objectExists(Objects::CanvasObject* obj);
        bool objectExists(Objects::CanvasObject* obj, size_t layer);
        size_t getObjectIndex(Objects::CanvasObject* obj, size_t layer);


        void deleteLater(Objects::CanvasObject* obj);

        template<typename T>
        static bool objectExists(T* obj, const std::vector<T*>& list);

        template<typename T>
        static size_t getObjectIndex(T* obj, const std::vector<T*>& list);

        template<typename T>
        static void addObject(Objects::CanvasObject* obj, std::vector<T*>& list);

        template<typename T>
        static void removeObject(Objects::CanvasObject* obj, std::vector<T*>& list);

        const static size_t npos = -1;


    private:

        void updateNewElements();
        void sfEvent(const std::vector<sf::Event>& events);
        void update();
        void draw(sf::RenderWindow& window);



        void addObject_internal();
        void deleteObject_internal();


        Canvas* m_parent;
        size_t m_layerSize;
        std::vector<CanvasObjectGroup*> m_container;

       // std::vector<Objects::CanvasObject*> m_toDelete;
    };

    template<typename T>
    size_t CanvasObjectLayerGroup::getObjectsCount() const
    {
        size_t count = 0;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            count += m_container[i].getObjectsCount<T>();
        }
        return count;
    }
    template<typename T>
    size_t CanvasObjectLayerGroup::getObjectsCount(size_t layer) const
    {
        if (layer > m_layerSize)
            return 0;
        return m_container[layer].getObjectsCount<T>();
    }
    template<typename T>
    std::vector<T*> CanvasObjectLayerGroup::getObjects() const
    {
        std::vector<T*> list;
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            std::vector<T*>buff = m_container[i].getObjects();
            list.insert(list.end, buff.begin(), buff.end());
        }
        return list;
    }
    template<typename T>
    std::vector<T*> CanvasObjectLayerGroup::getObjects(size_t layer) const
    {
        if (layer > m_layerSize)
            return std::vector<T*>();

        return m_container[layer].getObjects<T>();
    }
    template<typename T>
    T* CanvasObjectLayerGroup::getFirstObject() const
    {
        for (size_t i = 0; i < m_container.size(); ++i)
        {
            T* obj = m_container[i].getFirstObject<T>();
            if (obj)
                return obj;
        }
        return nullptr;
    }
    template<typename T>
    T* CanvasObjectLayerGroup::getFirstObject(size_t layer) const
    {
        if (layer > m_layerSize)
            return nullptr;
        return m_container[layer].getFirstObject<T>();
    }
    template<typename T>
    bool CanvasObjectLayerGroup::objectExists(T* obj, const std::vector<T*>& list)
    {
        QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
        size_t num = list.size();
        if (!num) return false;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (obj == *it) return true;
        }
        return false;
    }

    template<typename T>
    size_t CanvasObjectLayerGroup::getObjectIndex(T* obj, const std::vector<T*>& list)
    {
        QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_2);
        size_t num = list.size();
        if (!num) return npos;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (obj == *it) return it - list.begin();
        }
        return npos;
    }
    template<typename T>
    void CanvasObjectLayerGroup::addObject(QSFML::Objects::CanvasObject* obj, std::vector<T*>& list)
    {
        QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_3);
        T* transformed = dynamic_cast<T*>(obj);
        if (transformed)
        {
            list.push_back(transformed);
        }
    }

    template<typename T>
    void CanvasObjectLayerGroup::removeObject(QSFML::Objects::CanvasObject* obj, std::vector<T*>& list)
    {
        QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_4);
        T* transformed = dynamic_cast<T*>(obj);
        if (transformed)
        {
            size_t index = getObjectIndex<T>(transformed, list);
            if (index != npos)
                list.erase(list.begin() + index);
        }
    }

}