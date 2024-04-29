#pragma once

#include "QSFML_EditorWidget_base.h"
#include "RenderLayer.h"
#include "canvas/CanvasSettings.h"
#include "canvas/CanvasObjectLayerGroup.h"

#include "SFML/Graphics.hpp"
#include <vector>

namespace QSFML
{
class QSFML_EDITOR_WIDGET_EXPORT CanvasObjectContainer
{
    public:
        CanvasObjectContainer(Canvas *parent, CanvasSettings &settings);
        ~CanvasObjectContainer();

       

        void addObject(Objects::CanvasObject *obj);
        void addObject(const std::vector<Objects::CanvasObject*> &objs);

        void removeObject(Objects::CanvasObject *obj);
        void removeObject(const std::vector<Objects::CanvasObject*> &objs);
        void deleteObject(Objects::CanvasObject *obj);
        void deleteObject(const std::vector<Objects::CanvasObject*> &objs);
        void clearObjects();
        void cleanup();

        void applyObjectChanges();

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

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::CanvasObject* findFirstObject(const std::string &name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::CanvasObject*> findAllObjects(const std::string &name);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::CanvasObject* findFirstObjectRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::CanvasObject*> findAllObjectsRecursive(const std::string& name);

        void deleteLater(Objects::CanvasObject *obj);

        void renderLayerSwitch(Objects::CanvasObject *obj, RenderLayer from, RenderLayer to);
        void setRenderLayer(Objects::CanvasObject* obj, RenderLayer to);

        std::string getObjectsTreeString() const;

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

        std::vector<Objects::CanvasObject*> m_objectsToAdd;
        std::vector<Objects::CanvasObject*> m_objectsToDelete;
        std::vector<Objects::CanvasObject*> m_objectsToRemove;


        // All objects will be contained in this list
        CanvasObjectGroup *m_allObjects;
        size_t m_currentThreadGroupInsertIndex;
        size_t m_threadGroupCount;
        std::vector<CanvasObjectGroup*> m_threadGroups;
        //std::vector<CanvasObjectGroup*> m_renderLayerGroups;
        CanvasObjectLayerGroup m_renderLayerGroup;

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
