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

       

        void addObject(Objects::GameObjectPtr obj);
        void addObject(const std::vector<Objects::GameObjectPtr> &objs);

        void removeObject(Objects::GameObjectPtr obj);
        void removeObject(const std::vector<Objects::GameObjectPtr> &objs);
        void deleteObject(Objects::GameObjectPtr obj);
        void deleteObject(const std::vector<Objects::GameObjectPtr> &objs);
        void clearObjects();
        void cleanup();

        void applyObjectChanges();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T> size_t getObjectsCount() const;
        const std::vector<Objects::GameObjectPtr> &getObjects() const;
        template<typename T> std::vector<T*> getObjects() const;
        template<typename T> T* getFirstObject() const;

        bool objectExists(Objects::GameObjectPtr obj);
        size_t getObjectIndex(Objects::GameObjectPtr obj);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr findFirstObject(const std::string &name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> findAllObjects(const std::string &name);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr findFirstObjectRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        std::vector<Objects::GameObjectPtr> findAllObjectsRecursive(const std::string& name);

        template<typename T>
        T* findFirstObject()
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
					return casted;
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> findAllObject()
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            std::vector<T*> list;
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                    list.push_back(casted);
            }
            return list;
        }
        template<typename T>
        T* findFirstObjectRecursive()
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                    return casted;
                casted = obj->getFirstChild<T>();
                if (casted)
					return casted;
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> findAllObjectRecursive()
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            std::vector<T*> list;
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                    list.push_back(casted);
                std::vector<T*> subList = obj->getChilds<T>();
                if(subList.size() > 0)
                    list.insert(list.end(), subList.begin(), subList.end());
            }
            return list;
        }

        template<typename T>
        T* findFirstObject(const std::string &objName)
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                {
                    if (casted->getName() == objName)
                        return casted;
                }
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> findAllObject(const std::string& objName)
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            std::vector<T*> list;
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                {
                    if (casted->getName() == objName)
                        list.push_back(casted);
                }
            }
            return list;
        }
        template<typename T>
        T* findFirstObjectRecursive(const std::string& objName)
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                {
                    if (casted->getName() == objName)
                        return casted;
                }
                casted = obj->getFirstChild<T>();
                if (casted)
                {
                    if (casted->getName() == objName)
                        return casted;
                }
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> findAllObjectRecursive(const std::string& objName)
        {
            QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
            std::vector<T*> list;
            for (auto& obj : m_allObjects->getObjects())
            {
                T* casted = dynamic_cast<T*>(obj);
                if (casted)
                {
                    if (casted->getName() == objName)
                        list.push_back(casted);
                }
                std::vector<T*> subList = obj->getChilds<T>();
                if (subList.size() > 0)
                    list.insert(list.end(), subList.begin(), subList.end());
            }
            return list;
        }


        void deleteLater(Objects::GameObjectPtr obj);

        void renderLayerSwitch(Objects::GameObjectPtr obj, RenderLayer from, RenderLayer to);
        void setRenderLayer(Objects::GameObjectPtr obj, RenderLayer to);

        std::string getObjectsTreeString() const;

        const static size_t npos = -1;

        void draw(sf::RenderWindow& window);
    protected:
        void updateNewElements();
        void sfEvent(const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>&events);
        void update();
        

        void initializeThreads(size_t threadCount);
        void deinitializeThreads();
    private:

        


        void addObject_internal();
        void deleteObject_internal();

        std::vector<Objects::GameObjectPtr> m_objectsToAdd;
        std::vector<Objects::GameObjectPtr> m_objectsToDelete;
        std::vector<Objects::GameObjectPtr> m_objectsToRemove;


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
