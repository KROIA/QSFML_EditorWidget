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

        void addObject(Objects::GameObject *obj);
        void addObject(const std::vector<Objects::GameObject*> &objs);

        void removeObject(Objects::GameObject *obj);
        void removeObject(const std::vector<Objects::GameObject*> &objs);
        void deleteObject(Objects::GameObject *obj);
        void deleteObject(const std::vector<Objects::GameObject*> &objs);
        void clearObjects();
        void cleanup(); // removes and deletes all objects

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount() const;
        const std::vector<Objects::GameObject*> &getObjects() const;
        const std::vector<Objects::GameObject*> &getObjectsToAdd() const;
        const std::vector<Objects::GameObject*> &getObjectsToDelete() const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        T* getFirstObject() const;

        bool objectExists(Objects::GameObject *obj);
        size_t getObjectIndex(Objects::GameObject *obj);


        void deleteLater(Objects::GameObject *obj);

        template<typename T>
        static bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        static size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        static void addObject(Objects::GameObject *obj,std::vector<T*> &list);

        template<typename T>
        static void removeObject(Objects::GameObject *obj,std::vector<T*> &list);

        const static size_t npos = -1;

        private:

        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update();
        void draw(sf::RenderWindow &window);



        void addObject_internal();
        void deleteObject_internal();

        // All objects will be contained in this list
        std::vector<Objects::GameObject*> m_container;
        std::vector<Objects::GameObject*> m_toAddContainer;

        std::vector<Objects::GameObject*> m_toDelete;



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
void GameObjectGroup::addObject(QSFML::Objects::GameObject *obj,std::vector<T*> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_3);
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        list.push_back(transformed);
    }
}

template<typename T>
void GameObjectGroup::removeObject(QSFML::Objects::GameObject *obj,std::vector<T*> &list)
{
    QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_4);
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        size_t index = getObjectIndex<T>(transformed,list);
        if(index != npos)
            list.erase(list.begin()+index);
    }
}


}
