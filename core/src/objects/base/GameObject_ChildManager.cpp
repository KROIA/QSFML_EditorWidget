#include "objects/base/GameObject.h"
#include "Scene/Scene.h"
#include "components/base/Component.h"
#include "components/Transform.h"
#include "utilities/LifetimeChecker.h"

namespace QSFML
{
	namespace Objects
	{
		void GameObject::setRenderLayer(RenderLayer layer)
		{
			if (m_renderLayer == layer)
				return;
			if (m_parent)
			{
				m_parent->setRenderLayer(layer);
			}
			else if(this == m_rootParent)
			{
				RenderLayer oldLayer = m_renderLayer;
				m_renderLayer = layer;
				if (m_sceneParent)
					m_sceneParent->renderLayerSwitch(this, oldLayer, m_renderLayer);
			}
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
			//if (!m_selfOwnedLogObject)
			//{
			//	m_logObject = nullptr;
			//}
			if (m_sceneParent != nullptr)
			{
				m_sceneParent->addGameObject();
				m_sceneParent->addComponent(m_componentsManagerData.all.size());

				// Set the birth time and tick
				m_birthTick = m_sceneParent->getTick();
				m_birthTime = m_sceneParent->getElapsedTime();

				
			    setLogParent_internal(&m_sceneParent->getObjectLogger());
				//if (m_selfOwnedLogObject)
				//{
				//	m_selfOwnedLogObject->setParentID(m_sceneParent->getObjectLogger().getID());
				//}
				//else
				//{
				//	m_logObject = &m_sceneParent->getObjectLogger();
				//}
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

			//internalOnSceneParentChange(oldParent, m_sceneParent);
			onSceneParentChange(oldParent, m_sceneParent);
		}

		void GameObject::addChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toAdd.push_back(child);
			onObjectsChanged();
		}
		void GameObject::addChilds(const QSFML::vector<GameObjectPtr>& childs)
		{
			m_childObjectManagerData.toAdd.insert(m_childObjectManagerData.toAdd.end(), childs.begin(), childs.end());
			onObjectsChanged();
		}

		void GameObject::removeChild(GameObjectPtr child)
		{
			m_childObjectManagerData.toRemove.push_back(child);
			onObjectsChanged();
		}
		void GameObject::removeChilds(const QSFML::vector<GameObjectPtr>& childs)
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

		void GameObject::deleteChildLater(GameObjectPtr child)
		{
			m_childObjectManagerData.toDelete.push_back(child);
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
	

		GameObjectPtr GameObject::getFirstChild(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					return obj;
			}
			return nullptr;
		}
		QSFML::vector<GameObjectPtr> GameObject::getAllChilds(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFML::vector<GameObjectPtr> result;
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					result.push_back(obj);
			}
			return result;
		}

		GameObjectPtr GameObject::getFirstChildRecursive(const std::string& name)
		{
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					return obj;
				GameObjectPtr child = obj->getFirstChildRecursive(name);
				if (child)
					return child;
			}
			return nullptr;
		}
		QSFML::vector<GameObjectPtr> GameObject::getAllChildsRecursive(const std::string& name)
		{
			QSFML::vector<GameObjectPtr> result;
			for (auto& obj : m_childObjectManagerData.objs)
			{
				if (obj->getName() == name)
					result.push_back(obj);

				QSFML::vector<GameObjectPtr> childs = obj->getAllChildsRecursive(name);
				result.insert(result.end(), childs.begin(), childs.end());
			}
			return result;
		}
		size_t GameObject::getChildCountRecursive() const
		{
			size_t count = m_childObjectManagerData.objs.size();
			for (auto& obj : m_childObjectManagerData.objs)
			{
				count += obj->getChildCountRecursive();
			}
			return count;
		}
		bool GameObject::getAllChilds_internal(const std::string& name, QSFML::vector<GameObjectPtr>& foundList)
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
		bool GameObject::getAllChildsRecursive_internal(const std::string& name, QSFML::vector<GameObjectPtr>& foundList)
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
				found |= m_childObjectManagerData.objs[i]->getAllChilds_internal(name, foundList);
			}
			return found;
		}

		


		void GameObject::updateChanges_childObjectManager()
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			if (m_childObjectManagerData.toAdd.size() == 0 &&
				m_childObjectManagerData.toRemove.size() == 0 &&
				m_childObjectManagerData.toDelete.size() == 0)
				return;

			QSFML::vector<GameObjectPtr> toRemove = m_childObjectManagerData.toRemove;
			QSFML::vector<GameObjectPtr> toDelete = m_childObjectManagerData.toDelete;
			QSFML::vector<GameObjectPtr> toAdd = m_childObjectManagerData.toAdd;

			m_childObjectManagerData.toRemove.clear();
			m_childObjectManagerData.toDelete.clear();
			m_childObjectManagerData.toAdd.clear();

			size_t removeCount = 0;
			size_t addCount = 0;

			// Delete objects
			for (auto& obj : toDelete)
			{
				auto it = std::find(m_childObjectManagerData.objs.begin(), m_childObjectManagerData.objs.end(), obj);
				if (it != m_childObjectManagerData.objs.end())
				{
					m_childObjectManagerData.objs.erase(it);
					++removeCount;
					Internal::LifetimeChecker::deleteSecured(obj);
				}
			}

			// Remove objects
			for (auto& obj : toRemove)
			{
				auto it = std::find(m_childObjectManagerData.objs.begin(), m_childObjectManagerData.objs.end(), obj);
				if (it != m_childObjectManagerData.objs.end())
				{
					if (Internal::LifetimeChecker::isAlive(obj))
					{
						obj->setParent_internal(nullptr, nullptr, nullptr);
					}
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
				if(m_enableDrawGizmosRecursive)
					obj->enableDrawGizmosRecursive(true);
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
			if (addCount > 0)
				markTransformDirty();
		}

		void GameObject::onObjectsChanged()
		{
			m_childObjectManagerData.objectsChanged = true;
			if (m_parent)
			{
				m_parent->onObjectsChanged();
			}
			if (m_sceneParent)
			{
				m_sceneParent->GameObjectContainer::objectHasChanged(this);
			}
		}


		void GameObject::inSceneAdded_internal()
		{
			markTransformDirty();
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFMLP_OBJECT_BLOCK("Object onAwake", QSFML_COLOR_STAGE_2);
			onAwake();
			QSFMLP_OBJECT_END_BLOCK;

			QSFMLP_OBJECT_BLOCK("Childs onAwake", QSFML_COLOR_STAGE_4);
			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
			{
				m_childObjectManagerData.objs[i]->inSceneAdded_internal();
			}
			QSFMLP_OBJECT_END_BLOCK;
		}

		Objects::GameObjectPtr GameObject::getFirstObjectGlobal(const std::string& name)
		{
			if (!m_sceneParent) return nullptr;
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->getFirstObject(name);
		}
		QSFML::vector<Objects::GameObjectPtr> GameObject::getAllObjectsGlobal(const std::string& name)
		{
			if (!m_sceneParent) return QSFML::vector<GameObjectPtr>();
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->getAllObjects(name);
		}
		Objects::GameObjectPtr GameObject::getFirstObjectGlobalRecursive(const std::string& name)
		{
			if (!m_sceneParent) return nullptr;
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->getFirstObjectRecursive(name);
		}
		QSFML::vector<Objects::GameObjectPtr> GameObject::getAllObjectsGlobalRecusive(const std::string& name)
		{
			if (!m_sceneParent) return QSFML::vector<GameObjectPtr>();
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			return m_sceneParent->getAllObjectsRecursive(name);
		}
	}
}