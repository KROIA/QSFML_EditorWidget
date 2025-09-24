#pragma once

#include "QSFML_EditorWidget_base.h"
#include "RenderLayer.h"
#include "scene/SceneSettings.h"
#include "scene/GameObjectLayerGroup.h"
#include "objects/base/GameObject.h"

#include "SFML/Graphics.hpp"
#include <vector>

namespace QSFML
{
    class QSFML_EDITOR_WIDGET_API GameObjectContainer
    {
    public:
        GameObjectContainer(Scene* parent, SceneSettings& settings);
        ~GameObjectContainer();



        void addObject(Objects::GameObjectPtr obj);
        void add(Objects::GameObjectPtr obj) { addObject(obj); }
        void addObject(const QSFML::vector<Objects::GameObjectPtr>& objs);
        void add(const QSFML::vector<Objects::GameObjectPtr>& objs) { addObject(objs); }

        void removeObject(Objects::GameObjectPtr obj);
        void remove(Objects::GameObjectPtr obj) { removeObject(obj); }
        void removeObject(const QSFML::vector<Objects::GameObjectPtr>& objs);
        void remove(const QSFML::vector<Objects::GameObjectPtr>& objs) { removeObject(objs); }
        void deleteObject(Objects::GameObjectPtr obj);
        void del(Objects::GameObjectPtr obj) { deleteObject(obj); }
        void deleteObject(const QSFML::vector<Objects::GameObjectPtr>& objs);
        void del(const QSFML::vector<Objects::GameObjectPtr>& objs) { deleteObject(objs); }
        void clearObjects();
        void cleanup();

        void applyObjectChanges();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T> size_t getObjectsCount() const;
        const QSFML::vector<Objects::GameObjectPtr>& getObjects() const;
        template<typename T> QSFML::vector<T*> getObjects() const;
        template<typename T> T* getFirstObject() const;

        bool objectExists(Objects::GameObjectPtr obj);
        size_t getObjectIndex(Objects::GameObjectPtr obj);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr getFirstObject(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches only the top layer objects.
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        QSFML::vector<Objects::GameObjectPtr> getAllObjects(const std::string& name);

        /// <summary>
        /// Searches the object with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>pointer to the object, or nullptr if not found</returns>
        Objects::GameObjectPtr getFirstObjectRecursive(const std::string& name);

        /// <summary>
        /// Searches all objects with the given name in the object list.
        /// Searches all object tree hirarchies
        /// </summary>
        /// <param name="name">name of the object</param>
        /// <returns>a list of found objects</returns>
        QSFML::vector<Objects::GameObjectPtr> getAllObjectsRecursive(const std::string& name);

        template<typename T>
        T* getFirstObjectRecursive();

        template<typename T>
        QSFML::vector<T*> getObjectsRecursive();

        template<typename T>
        T* getFirstObject(const std::string& objName);

        template<typename T>
        QSFML::vector<T*> getObjects(const std::string& objName);

        template<typename T>
        T* getFirstObjectRecursive(const std::string& objName);

        template<typename T>
        QSFML::vector<T*> getObjectsRecursive(const std::string& objName);


        void deleteLater(Objects::GameObjectPtr obj);

        void renderLayerSwitch(Objects::GameObjectPtr obj, RenderLayer from, RenderLayer to);
        void setRenderLayer(Objects::GameObjectPtr obj, RenderLayer to);

        std::string getObjectsTreeString() const;

        const static size_t npos = -1;

        void draw(sf::RenderWindow& window);
        void objectHasChanged(Objects::GameObjectPtr obj);
    protected:
        void updateNewElements();
        void sfEvent(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events);
        void update();


        void initializeThreads(size_t threadCount);
        void deinitializeThreads();
    private:




        void addObject_internal();
        void deleteObject_internal();

        QSFML::vector<Objects::GameObjectPtr> m_objectsToAdd;
        QSFML::vector<Objects::GameObjectPtr> m_objectsToDelete;
        QSFML::vector<Objects::GameObjectPtr> m_objectsToRemove;

        std::mutex m_objectsWithChangesMutex;
        QSFML::vector<Objects::GameObjectPtr> m_objectsWithChanges;


        // All objects will be contained in this list
        GameObjectGroup* m_allObjects;
        size_t m_currentThreadGroupInsertIndex;
        size_t m_threadGroupCount;
        QSFML::vector<GameObjectGroup*> m_threadGroups;
        //QSFML::vector<GameObjectGroup*> m_renderLayerGroups;
        GameObjectLayerGroup m_renderLayerGroup;

        Scene* m_parent;

        SceneThreadWorker* m_threadWorker;

        SceneSettings& m_settings;

    };


    template<typename T>
    size_t GameObjectContainer::getObjectsCount() const
    {
        return m_allObjects->getObjectsCount<T>();
    }
    template<typename T>
    QSFML::vector<T*> GameObjectContainer::getObjects() const
    {
        return m_allObjects->getObjects<T>();
    }
    template<typename T>
    T* GameObjectContainer::getFirstObject() const
    {
        return m_allObjects->getFirstObject<T>();
    }
    template<typename T>
    T* GameObjectContainer::getFirstObjectRecursive()
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
    QSFML::vector<T*> GameObjectContainer::getObjectsRecursive()
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        QSFML::vector<T*> list;
        for (auto& obj : m_allObjects->getObjects())
        {
            T* casted = dynamic_cast<T*>(obj);
            if (casted)
                list.push_back(casted);
            QSFML::vector<T*> subList = obj->getChilds<T>();
            if (subList.size() > 0)
                list.insert(list.end(), subList.begin(), subList.end());
        }
        return list;
    }

    template<typename T>
    T* GameObjectContainer::getFirstObject(const std::string& objName)
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
    QSFML::vector<T*> GameObjectContainer::getObjects(const std::string& objName)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        QSFML::vector<T*> list;
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
    T* GameObjectContainer::getFirstObjectRecursive(const std::string& objName)
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
    QSFML::vector<T*> GameObjectContainer::getObjectsRecursive(const std::string& objName)
    {
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);
        QSFML::vector<T*> list;
        for (auto& obj : m_allObjects->getObjects())
        {
            T* casted = dynamic_cast<T*>(obj);
            if (casted)
            {
                if (casted->getName() == objName)
                    list.push_back(casted);
            }
            QSFML::vector<T*> subList = obj->getChilds<T>();
            if (subList.size() > 0)
                list.insert(list.end(), subList.begin(), subList.end());
        }
        return list;
    }



}
