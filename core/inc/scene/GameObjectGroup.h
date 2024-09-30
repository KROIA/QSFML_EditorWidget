#pragma once

#include "QSFML_EditorWidget_base.h"

#include "SFML/Graphics.hpp"

namespace QSFML
{

class QSFML_EDITOR_WIDGET_EXPORT GameObjectGroup
{
        friend GameObjectContainer;
        friend SceneThreadWorker;
        friend GameObjectLayerGroup;
    public:
        GameObjectGroup(Scene *parent);
        GameObjectGroup(const GameObjectGroup &other) = delete;
        ~GameObjectGroup();

        GameObjectGroup &operator=(const GameObjectGroup&other) = delete;

        void addObject(Objects::GameObjectPtr obj);
        void addObject(const std::vector<Objects::GameObjectPtr> &objs);

        void removeObject(Objects::GameObjectPtr obj);
        void removeObject(const std::vector<Objects::GameObjectPtr> &objs);
        void deleteObject(Objects::GameObjectPtr obj);
        void deleteObject(const std::vector<Objects::GameObjectPtr> &objs);
        void clearObjects();
        void cleanup(); // removes and deletes all objects

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount() const;
        const std::vector<Objects::GameObjectPtr> &getObjects() const;
        const std::vector<Objects::GameObjectPtr> &getObjectsToAdd() const;
        const std::vector<Objects::GameObjectPtr> &getObjectsToDelete() const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        T* getFirstObject() const;

        bool objectExists(Objects::GameObjectPtr obj);
        size_t getObjectIndex(Objects::GameObjectPtr obj);


        void deleteLater(Objects::GameObjectPtr obj);

        template<typename T>
        static bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        static size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        static void addObject(Objects::GameObjectPtr obj,std::vector<std::shared_ptr<T>> &list);

        template<typename T>
        static void removeObject(Objects::GameObjectPtr obj,std::vector<std::shared_ptr<T>> &list);

        const static size_t npos = -1;

        private:

        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update();
        void draw(sf::RenderWindow &window);



        void addObject_internal();
        void deleteObject_internal();

        // All objects will be contained in this list
        std::vector<Objects::GameObjectPtr> m_container;
        std::vector<Objects::GameObjectPtr> m_toAddContainer;

        std::vector<Objects::GameObjectPtr> m_toDelete;



        Scene *m_parent;
};

template<typename T>
size_t GameObjectGroup::getObjectsCount() const
{
    size_t count = 0;
    for(size_t i=0; i<m_container.size(); ++i)
    {
        T* obj = dynamic_cast<T*>(m_container[i]);
        if(obj)
            ++count;
    }
    return count;
}
template<typename T>
std::vector<T*> GameObjectGroup::getObjects() const
{
    std::vector<T*> list;
    list.reserve(m_container.size());
    for(size_t i=0; i<m_container.size(); ++i)
    {
        T* obj = dynamic_cast<T*>(m_container[i]);
        if(obj)
            list.push_back(obj);
    }
    return list;
}
template<typename T>
T* GameObjectGroup::getFirstObject() const
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        T* obj = dynamic_cast<T*>(m_container[i]);
        if(obj)
            return obj;
    }
    return nullptr;
}
template<typename T>
bool GameObjectGroup::objectExists(T *obj,const std::vector<T*> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t num = list.size();
    if(!num) return false;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return true;
    }
    return false;
}

template<typename T>
size_t GameObjectGroup::getObjectIndex(T *obj,const std::vector<T*> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_2);
    size_t num = list.size();
    if(!num) return npos;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return it - list.begin();
    }
    return npos;
}
template<typename T>
void GameObjectGroup::addObject(QSFML::Objects::GameObjectPtr obj,std::vector<std::shared_ptr<T>> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_3);
    std::shared_ptr<T> transformed = std::dynamic_pointer_cast<T>(obj);
    if(transformed)
    {
        list.push_back(transformed);
    }
}

template<typename T>
void GameObjectGroup::removeObject(QSFML::Objects::GameObjectPtr obj,std::vector<std::shared_ptr<T>> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_4);
    std::shared_ptr<T> transformed = std::dynamic_pointer_cast<T>(obj);
    if(transformed)
    {
        size_t index = getObjectIndex<T>(transformed,list);
        if(index != npos)
            list.erase(list.begin()+index);
    }
}


}
