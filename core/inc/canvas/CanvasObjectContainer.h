#pragma once

#include <vector>
#include "objects/CanvasObject.h"
#include "components/Drawable.h"
#include "objects/CameraController.h"
#include "CanvasObjectGroup.h"
#include "QSFML_debugSettings.h"
#include "CanvasThreadWorker.h"
#include "QSFML_global.h"

namespace QSFML
{
class QSFML_EDITOR_WIDGET_EXPORT CanvasObjectContainer
{
    public:
        CanvasObjectContainer(Canvas *parent, const CanvasSettings &settings);
        ~CanvasObjectContainer();

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

        void renderLayerSwitch(Objects::CanvasObject *obj, RenderLayer from, RenderLayer to);

        size_t getUpdateCount() const;

        const static size_t npos = -1;

    protected:
        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update();
        void draw(sf::RenderWindow &window);
    private:

        void setupThreads(size_t threadCount);


        /*template<typename T>
        bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        void addObject(Objects::CanvasObject *obj,std::vector<T*> &list);

        template<typename T>
        void removeObject(Objects::CanvasObject *obj,std::vector<T*> &list);*/

        void addObject_internal();
        void deleteObject_internal();

        // All objects will be contained in this list
        CanvasObjectGroup *m_allObjects;
        size_t m_currentThreadGroupInsertIndex;
        size_t m_threadGroupCount;
        std::vector<CanvasObjectGroup*> m_threadGroups;
        std::vector<CanvasObjectGroup*> m_renderLayerGroups;


        Canvas *m_parent;

        CanvasThreadWorker *m_threadWorker;
        size_t m_updateCount;
};
template<typename T>
size_t CanvasObjectContainer::getObjectsCount() const
{
    return m_allObjects->getObjectsCount<T>();
}
template<typename T>
std::vector<T*> CanvasObjectContainer::getObjects() const
{
    return m_allObjects->getObjects<T>();
}
template<typename T>
T* CanvasObjectContainer::getFirstObject() const
{
    return m_allObjects->getFirstObject<T>();
}
/*template<typename T>
bool CanvasObjectContainer::objectExists(T *obj,const std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange100));
    return m_allObjects->objectExists<T>(obj, list);
}

template<typename T>
size_t CanvasObjectContainer::getObjectIndex(T *obj,const std::vector<T*> &list)
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
void CanvasObjectContainer::addObject(QSFML::Objects::CanvasObject *obj,std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange300));
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        list.push_back(transformed);
    }
}

template<typename T>
void CanvasObjectContainer::removeObject(QSFML::Objects::CanvasObject *obj,std::vector<T*> &list)
{
    QSFML_PROFILE_CANVAS(EASY_FUNCTION(profiler::colors::Orange400));
    T* transformed = dynamic_cast<T*>(obj);
    if(transformed)
    {
        size_t index = getObjectIndex<T>(transformed,list);
        if(index != npos)
            list.erase(list.begin()+index);
    }
}*/


}
