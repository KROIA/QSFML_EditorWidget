#pragma once

#include "QSFML_base.h"
#include "RenderLayer.h"
#include "canvas/CanvasSettings.h"

#include "SFML/Graphics.hpp"
#include <vector>

namespace QSFML
{
class QSFML_EDITOR_WIDGET_EXPORT CanvasObjectContainer
{
    public:
        CanvasObjectContainer(Canvas *parent, CanvasSettings &settings);
        ~CanvasObjectContainer();

        void applyObjectChanges(); // Will be called automaticly if not manual

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

        

        const static size_t npos = -1;

    protected:
        void updateNewElements();
        void sfEvent(const std::vector<sf::Event> &events);
        void update();
        void draw(sf::RenderWindow &window);

        void initializeThreads(size_t threadCount);
        void deinitializeThreads();
    private:

        


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

        CanvasSettings& m_settings;
        
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



}
