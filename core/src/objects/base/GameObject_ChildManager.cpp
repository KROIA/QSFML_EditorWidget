#include "objects/base/GameObject.h"

namespace QSFML
{
	namespace Objects
	{
		void GameObject::addChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toAdd.push_back(child);
		}
		void GameObject::addChilds(const std::vector<GameObjectPtr>& childs)
		{
			m_childObjectManagerData.toAdd.insert(m_childObjectManagerData.toAdd.end(), childs.begin(), childs.end());
		}

		void GameObject::removeChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toRemove.push_back(child);
		}
		void GameObject::removeChilds(const std::vector<GameObjectPtr>& childs)
		{
			m_childObjectManagerData.toRemove.insert(m_childObjectManagerData.toRemove.end(), childs.begin(), childs.end());
		}
		void GameObject::clearChilds()
		{
			m_childObjectManagerData.toAdd.clear();
			m_childObjectManagerData.toRemove = m_childObjectManagerData.objs;
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




		void GameObject::updateChanges_childObjectManager()
		{
			std::vector<GameObjectPtr> toRemove = m_childObjectManagerData.toRemove;
			std::vector<GameObjectPtr> toAdd = m_childObjectManagerData.toAdd;

			m_childObjectManagerData.toRemove.clear();
			m_childObjectManagerData.toAdd.clear();

			// Remove objects
			for (auto& obj : toRemove)
			{
				auto it = std::find(m_childObjectManagerData.objs.begin(), m_childObjectManagerData.objs.end(), obj);
				if (it != m_childObjectManagerData.objs.end())
					m_childObjectManagerData.objs.erase(it);
			}

			// Add objects
			for (auto& obj : toAdd)
			{
				m_childObjectManagerData.objs.push_back(obj);
			}				
		}
	}
}