#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

namespace QSFML
{
	namespace Objects
	{
		void GameObject::addChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toAdd.push_back(child);
			onObjectsChanged();
		}
		void GameObject::addChilds(const std::vector<GameObjectPtr>& childs)
		{
			m_childObjectManagerData.toAdd.insert(m_childObjectManagerData.toAdd.end(), childs.begin(), childs.end());
			onObjectsChanged();
		}

		void GameObject::removeChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toRemove.push_back(child);
			onObjectsChanged();
		}
		void GameObject::removeChilds(const std::vector<GameObjectPtr>& childs)
		{
			m_childObjectManagerData.toRemove.insert(m_childObjectManagerData.toRemove.end(), childs.begin(), childs.end());
			onObjectsChanged();
		}
		void GameObject::clearChilds()
		{
			m_childObjectManagerData.toAdd.clear();
			m_childObjectManagerData.toRemove = m_childObjectManagerData.objs;
			onObjectsChanged();
		}


		bool GameObject::hasChild(GameObjectPtr child) const
		{
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj == child)
					return true;
			}
			return false;
		}
		size_t GameObject::getChildIndex(GameObjectPtr child) const
		{
			const auto it = std::find(m_childObjectManagerData.objs.begin(), m_childObjectManagerData.objs.end(), child);
			if (it != m_childObjectManagerData.objs.end())
				return std::distance(m_childObjectManagerData.objs.begin(), it);
			return -1;
		}
	

		GameObjectPtr GameObject::findFirstChild(const std::string& name)
		{
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					return obj;
			}
			return nullptr;
		}
		std::vector<GameObjectPtr> GameObject::findAllChilds(const std::string& name)
		{
			std::vector<GameObjectPtr> result;
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					result.push_back(obj);
			}
			return result;
		}

		GameObjectPtr GameObject::findFirstChildRecursive(const std::string& name)
		{
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					return obj;
			}
			// <! ToDo recursive
			return nullptr;
		}
		std::vector<GameObjectPtr> GameObject::findAllChildsRecursive(const std::string& name)
		{
			std::vector<GameObjectPtr> result;
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					result.push_back(obj);
			}
			// <! ToDo recursive
			return result;
		}
		bool GameObject::findAllChilds_internal(const std::string& name, std::vector<GameObjectPtr>& foundList)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			size_t nameSize = name.size();
			bool found = false;
			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
			{
				const std::string& objName = m_childObjectManagerData.objs[i]->getName();
				if (nameSize != objName.size())
					continue;
				if (objName == name)
				{
					foundList.push_back(m_childObjectManagerData.objs[i]);
					found = true;
				}
				//found |= m_childs[i]->findAllChilds_internal(name, foundList);
			}
			return found;
		}
		bool GameObject::findAllChildsRecursive_internal(const std::string& name, std::vector<GameObjectPtr>& foundList)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			size_t nameSize = name.size();
			bool found = false;
			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
			{
				const std::string& objName = m_childObjectManagerData.objs[i]->getName();
				if (nameSize != objName.size())
					continue;
				if (objName == name)
				{
					foundList.push_back(m_childObjectManagerData.objs[i]);
					found = true;
				}
				found |= m_childObjectManagerData.objs[i]->findAllChilds_internal(name, foundList);
			}
			return found;
		}




		void GameObject::updateChanges_childObjectManager()
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			std::vector<GameObjectPtr> toRemove = m_childObjectManagerData.toRemove;
			std::vector<GameObjectPtr> toAdd = m_childObjectManagerData.toAdd;

			m_childObjectManagerData.toRemove.clear();
			m_childObjectManagerData.toAdd.clear();

			size_t removeCount = 0;
			size_t addCount = 0;

			// Remove objects
			for (auto& obj : toRemove)
			{
				auto it = std::find(m_childObjectManagerData.objs.begin(), m_childObjectManagerData.objs.end(), obj);
				if (it != m_childObjectManagerData.objs.end())
				{
					m_childObjectManagerData.objs.erase(it);
					++removeCount;
				}
			}

			// Add objects
			for (auto& obj : toAdd)
			{
				if (!obj)
					continue;
				for (auto& inList : m_childObjectManagerData.objs)
				{
					if (inList == obj)
						goto overjump;
				}
				m_childObjectManagerData.objs.push_back(obj);
				obj->setParent_internal(this, m_rootParent, m_sceneParent);
				obj->updateNewElements();
				++addCount;
				overjump:;
			}			
			m_childObjectManagerData.objectsChanged = false;

			if (m_sceneParent)
			{
				m_sceneParent->removeGameObject(removeCount);
				m_sceneParent->addGameObject(addCount);
			}
		}

		void GameObject::onObjectsChanged()
		{
			m_childObjectManagerData.objectsChanged = true;
			if (m_parent)
			{
				m_parent->onObjectsChanged();
			}
		}


		void GameObject::inSceneAdded_internal()
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFMLP_OBJECT_BLOCK("Object inSceneAdded", QSFML_COLOR_STAGE_2);
			inSceneAdded();
			QSFMLP_OBJECT_END_BLOCK;

			QSFMLP_OBJECT_BLOCK("Childs inSceneAdded", QSFML_COLOR_STAGE_4);
			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
			{
				m_childObjectManagerData.objs[i]->inSceneAdded_internal();
			}
			QSFMLP_OBJECT_END_BLOCK;
		}

		
	}
}