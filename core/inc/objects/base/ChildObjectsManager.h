#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Objects
	{
		class GameObject;
		typedef std::shared_ptr<GameObject> GameObjectPtr;
        namespace Internal
        {
            class QSFML_EDITOR_WIDGET_EXPORT ChildObjectsManager
            {
                friend GameObject;
                ChildObjectsManager(GameObject& parent);
                virtual ~ChildObjectsManager();
                public:


                void addChild(GameObjectPtr child);
                void addChilds(const std::vector<GameObjectPtr>& childs);

                void removeChild(GameObjectPtr child);
                void removeChilds(const std::vector<GameObjectPtr>& childs);
                void clearChilds();
                template<typename T>
                void removeChilds();

                bool hasChild(GameObjectPtr child) const;
                size_t getChildIndex(GameObjectPtr child) const;
                const std::vector<GameObjectPtr>& getChilds() const { return m_childObjects; }

                template<typename T>
                std::vector<std::shared_ptr<T>> getChilds() const;

                template<typename T>
                std::vector<std::shared_ptr<T>> getChildsRecusrive() const;

                size_t getChildCount() const;

                template<typename T>
                size_t getChildCount() const;

                template<typename T>
                size_t getChildCountRecusrive() const;

                GameObjectPtr findFirstChild(const std::string& name);
                std::vector<GameObjectPtr> findAllChilds(const std::string& name);

                GameObjectPtr findFirstChildRecursive(const std::string& name);
                std::vector<GameObjectPtr> findAllChildsRecursive(const std::string& name);

                private:
                void updateChanges();
                GameObject& m_parent;

                struct ChildObjectManagerData 
                {
                    std::vector<GameObjectPtr> m_childObjects;

                    std::vector<GameObjectPtr> m_toAddChildObjects;
                    std::vector<GameObjectPtr> m_toRemoveChildObjects;
                };
            };

            template<typename T>
            void ChildObjectsManager::removeChilds()
            {
				m_toRemoveChildObjects.reserve(m_toRemoveChildObjects.size() + m_childObjects.size());
				for (auto& obj : m_childObjects)
				{
					if (std::dynamic_pointer_cast<T>(obj))
					{
                        m_toRemoveChildObjects.push_back(obj);
					}
				}
            }

            template<typename T>
            std::vector<std::shared_ptr<T>> ChildObjectsManager::getChilds() const
            {
                std::vector<std::shared_ptr<T>> childs;
                for (auto& obj : m_childObjects)
                {
                    if (std::shared_ptr<T> child = std::dynamic_pointer_cast<T>(obj))
                    {
                        childs.push_back(child);
                    }
                }
                return childs;
            }

            template<typename T>
            std::vector<std::shared_ptr<T>> ChildObjectsManager::getChildsRecusrive() const
            {
				std::vector<std::shared_ptr<T>> childs;
				for (auto& obj : m_childObjects)
				{
					if (std::shared_ptr<T> child = std::dynamic_pointer_cast<T>(obj))
					{
						childs.push_back(child);
					}
					if (std::shared_ptr<ChildObjectsManager> childManager = std::dynamic_pointer_cast<ChildObjectsManager>(obj->getComponent<ChildObjectsManager>()))
					{
						std::vector<std::shared_ptr<T>> childChilds = childManager->getChildsRecusrive<T>();
						childs.insert(childs.end(), childChilds.begin(), childChilds.end());
					}
				}

				// <! ToDo recursive
				return childs;
            }
        }
	}
}