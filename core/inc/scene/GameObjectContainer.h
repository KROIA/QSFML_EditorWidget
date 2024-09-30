#pragma once

#include "QSFML_EditorWidget_base.h"
#include "RenderLayer.h"
#include "scene/SceneSettings.h"
#include "scene/GameObjectLayerGroup.h"

#include "SFML/Graphics.hpp"
#include <vector>

namespace QSFML
{
class QSFML_EDITOR_WIDGET_EXPORT GameObjectContainer
{
    public:
        GameObjectContainer(Scene *parent, SceneSettings &settings);
        ~GameObjectContainer();

       

        void addObject(Objects::GameObject *obj);
        void addObject(const std::vector<Objects::GameObject*> &objs);

        void removeObject(Objects::GameObject *obj);
        void removeObject(const std::vector<Objects::GameObject*> &objs);
        void deleteObject(Objects::GameObject *obj);
        void deleteObject(const std::vector<Objects::GameObject*> &objs);
        void clearObjects();
        void cleanup();

        void applyObjectChanges();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount() const;
        const std::vector<Objects::GameObject*> &getObjects() const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        T* getFirstObject() const;

        bool objectExists(Objects::GameObject *obj);
        size_t getObjectIndex(Objects::GameObject *obj);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObject* findFirstObject(const std::string &name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObject*> findAllObjects(const std::string &name);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObject* findFirstObjectRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObject*> findAllObjectsRecursive(const std::string& name);

        void deleteLater(Objects::GameObject *obj);

        void renderLayerSwitch(Objects::GameObject *obj, RenderLayer from, RenderLayer to);
        void setRenderLayer(Objects::GameObject* obj, RenderLayer to);

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

        std::vector<Objects::GameObject*> m_objectsToAdd;
        std::vector<Objects::GameObject*> m_objectsToDelete;
        std::vector<Objects::GameObject*> m_objectsToRemove;


        // All objects will be contained in this list
        GameObjectGroup *m_allObjects;
        size_t m_currentThreadGroupInsertIndex;
        size_t m_threadGroupCount;
        std::vector<GameObjectGroup*> m_threadGroups;
        //std::vector<GameObjectGroup*> m_renderLayerGroups;
        GameObjectLayerGroup m_renderLayerGroup;

        Scene *m_parent;

        SceneThreadWorker *m_threadWorker;

        SceneSettings& m_settings;
        
};
template<typename T>
size_t GameObjectContainer::getObjectsCount() const
{
    return m_allObjects->getObjectsCount<T>();
}
template<typename T>
std::vector<T*> GameObjectContainer::getObjects() const
{
    return m_allObjects->getObjects<T>();
}
template<typename T>
T* GameObjectContainer::getFirstObject() const
{
    return m_allObjects->getFirstObject<T>();
}



}
