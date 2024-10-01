#include "objects/base/GameObject.h"
#include "Scene/Scene.h"
#include "components/base/Component.h"
#include "components/Transform.h"

namespace QSFML
{
	namespace Objects
	{
		void GameObject::setRenderLayer(RenderLayer layer)
		{
			if (m_renderLayer == layer)
				return;
			RenderLayer oldLayer = m_renderLayer;
			m_renderLayer = layer;
			if (m_sceneParent)
				m_sceneParent->renderLayerSwitch(this, oldLayer, m_renderLayer);
		}
		double GameObject::getAge() const
		{
			if (!m_sceneParent) return 0;
			return m_sceneParent->getElapsedTime() - m_birthTime;
		}
		size_t GameObject::getAgeTicks() const
		{
			if (!m_sceneParent) return 0;
			return m_sceneParent->getTick() - m_birthTick;
		}
		double GameObject::getAgeFixed() const
		{
			if (!m_sceneParent) return 0;
			return m_sceneParent->getFixedDeltaT() * (m_sceneParent->getTick() - m_birthTick);
		}
		void GameObject::setSceneParent(Scene* parent)
		{
			if (m_sceneParent == parent)
				return;
			Scene* oldParent = m_sceneParent;
			m_sceneParent = parent;
			if (oldParent != nullptr)
			{
				oldParent->removeGameObject();
				oldParent->removeComponent(m_componentsManagerData.all.size());
			}
			if (!m_selfOwnedLogObject)
			{
				m_logObject = nullptr;
			}
			if (m_sceneParent != nullptr)
			{
				m_sceneParent->addGameObject();
				m_sceneParent->addComponent(m_componentsManagerData.all.size());

				// Set the birth time and tick
				m_birthTick = m_sceneParent->getTick();
				m_birthTime = m_sceneParent->getElapsedTime();
				if (m_selfOwnedLogObject)
				{
					m_selfOwnedLogObject->setParentID(m_sceneParent->getObjectLogger().getID());
				}
				else
				{
					m_logObject = &m_sceneParent->getObjectLogger();
				}
			}

			for (size_t i = 0; i < m_componentsManagerData.all.size(); ++i)
			{
				Components::Component* comp = m_componentsManagerData.all[i];
				comp->setSceneParent(m_sceneParent);
			}

			//for(size_t i=0; i<m_components.size(); ++i)
			//    m_components[i]->setParent(this);

			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
				m_childObjectManagerData.objs[i]->setSceneParent(parent);

			internalOnSceneParentChange(oldParent, m_sceneParent);
			onSceneParentChange(oldParent, m_sceneParent);
		}

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
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					return obj;
			}
			return nullptr;
		}
		std::vector<GameObjectPtr> GameObject::findAllChilds(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
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

			bool _needsDrawUpdate = false;
			bool _needsEventUpdate = false;
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
				obj->updateObjectChanges();
				_needsDrawUpdate |= obj->m_componentsManagerData.thisNeedsDrawUpdate;
				_needsEventUpdate |= obj->m_componentsManagerData.thisNeedsEventUpdate;
				++addCount;
				overjump:;
			}	
			if (_needsDrawUpdate)
				needsDrawUpdate(_needsDrawUpdate);
			if (_needsEventUpdate)
				needsEventUpdate(_needsEventUpdate);
			
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

		Objects::GameObjectPtr GameObject::findFirstObjectGlobal(const std::string& name)
		{
			if (!m_sceneParent) return nullptr;
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->findFirstObject(name);
		}
		std::vector<Objects::GameObjectPtr> GameObject::findAllObjectsGlobal(const std::string& name)
		{
			if (!m_sceneParent) return std::vector<GameObjectPtr>();
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->findAllObjects(name);
		}
		Objects::GameObjectPtr GameObject::findFirstObjectGlobalRecursive(const std::string& name)
		{
			if (!m_sceneParent) return nullptr;
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->findFirstObjectRecursive(name);
		}
		std::vector<Objects::GameObjectPtr> GameObject::findAllObjectsGlobalRecusive(const std::string& name)
		{
			if (!m_sceneParent) return std::vector<GameObjectPtr>();
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->findAllObjectsRecursive(name);
		}
	}
}