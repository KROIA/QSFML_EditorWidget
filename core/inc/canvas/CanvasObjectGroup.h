#pragma once

#include "CanvasForwardDeclaration.h"
#include "objects/CanvasObject.h"

namespace QSFML
{

class CanvasObjectGroup
{
        friend CanvasObjectContainer;
        friend CanvasThreadWorker;
    public:
        CanvasObjectGroup(Canvas *parent);
        CanvasObjectGroup(const CanvasObjectGroup &other) = delete;
        ~CanvasObjectGroup();

        CanvasObjectGroup &operator=(const CanvasObjectGroup&other) = delete;

        void addObject(Objects::CanvasObject *obj);
        void addObject(const std::vector<Objects::CanvasObject*> &objs);

        void removeObject(Objects::CanvasObject *obj);
        void removeObject(const std::vector<Objects::CanvasObject*> &objs);
        void deleteObject(Objects::CanvasObject *obj);
        void deleteObject(const std::vector<Objects::CanvasObject*> &objs);
        void clearObjects();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount() const;
        const std::vector<Objects::CanvasObject*> &getObjects() const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        T* getFirstObject() const;

        bool objectExists(Objects::CanvasObject *obj);
        size_t getObjectIndex(Objects::CanvasObject *obj);


        void deleteLater(Objects::CanvasObject *obj);

        template<typename T>
        static bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        static size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        static void addObject(Objects::CanvasObject *obj,std::vector<T*> &list);

        template<typename T>
        static void removeObject(Objects::CanvasObject *obj,std::vector<T*> &list);

        const static size_t npos = -1;

        private:

        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update();
        void draw(sf::RenderWindow &window);



        void addObject_internal();
        void deleteObject_internal();

        // All objects will be contained in this list
        std::vector<Objects::CanvasObject*> m_container;
        std::vector<Objects::CanvasObject*> m_toAddContainer;

        std::vector<Objects::CanvasObject*> m_toDelete;



        Canvas *m_parent;
};

template<typename T>
size_t CanvasObjectGroup::getObjectsCount() const
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
std::vector<T*> CanvasObjectGroup::getObjects() const
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
T* CanvasObjectGroup::getFirstObject() const
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
bool CanvasObjectGroup::objectExists(T *obj,const std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange100));
    size_t num = list.size();
    if(!num) return false;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return true;
    }
    return false;
}

template<typename T>
size_t CanvasObjectGroup::getObjectIndex(T *obj,const std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange200));
    size_t num = list.size();
    if(!num) return npos;

    for(auto it = list.begin(); it != list.end(); ++it) {
        if(obj == *it) return it - list.begin();
    }
    return npos;
}
template<typename T>
void CanvasObjectGroup::addObject(QSFML::Objects::CanvasObject *obj,std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange300));
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        list.push_back(transformed);
    }
}

template<typename T>
void CanvasObjectGroup::removeObject(QSFML::Objects::CanvasObject *obj,std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange400));
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        size_t index = getObjectIndex<T>(transformed,list);
        if(index != npos)
            list.erase(list.begin()+index);
    }
}


}
