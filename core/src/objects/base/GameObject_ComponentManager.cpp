#include "objects/base/GameObject.h"
#include "Scene/Scene.h"
#include "utilities/ObjectQuadTree.h"
#include "utilities/LifetimeChecker.h"


namespace QSFML
{
	namespace Objects
	{
		


		
		void GameObject::clearComponents()
		{
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove = m_componentsManagerData.all;
			m_componentsManagerData.sfDrawableToAdd.clear();
			m_componentsManagerData.sfDrawableToRemove = m_componentsManagerData.sfDrawable;
			onObjectsChanged();
		}

		void GameObject::deleteComponentLater(Components::ComponentPtr component)
		{
			m_componentsManagerData.toDelete.push_back(component); 
			onObjectsChanged();
		}

		const QSFML::vector<Components::ComponentPtr>& GameObject::getComponents() const
		{
			return m_componentsManagerData.all;
		}

		


		bool GameObject::hasComponent(const std::string& name) const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFMLP_OBJECT_TEXT("Checking for component", name);
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return true;
			}
			return false;
		}
		bool GameObject::hasComponent(Components::ComponentPtr component) const
		{
			if (!component)
				return false;
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFMLP_OBJECT_TEXT("Checking for component", component->getName());
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp == component)
					return true;
			}
			return false;
		}
		size_t GameObject::getComponentCountRecursive() const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			size_t count = m_componentsManagerData.all.size();
			for (auto& obj : m_childObjectManagerData.objs)
			{
				count += obj->getComponentCountRecursive();
			}
			return count;
		}


		Components::ComponentPtr GameObject::getFirstComponent(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}
			return nullptr;
		}
		QSFML::vector<Components::ComponentPtr> GameObject::getAllComponents(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFML::vector<Components::ComponentPtr> comps;
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					comps.push_back(comp);
			}
			return comps;
		}

		Components::ComponentPtr GameObject::getFirstComponentRecursive(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}
			for (auto& obj : m_childObjectManagerData.objs)
			{
				Components::ComponentPtr comp = obj->getFirstComponentRecursive(name);
				if (comp)
					return comp;
			}
			return nullptr;
		}
		QSFML::vector<Components::ComponentPtr> GameObject::getAllComponentsRecursive(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFML::vector<Components::ComponentPtr> comps;
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					comps.push_back(comp);
			}
			for (auto& obj : m_childObjectManagerData.objs)
			{
				QSFML::vector<Components::ComponentPtr> childComps = obj->getAllComponentsRecursive(name);
				comps.insert(comps.end(), childComps.begin(), childComps.end());
			}
			return comps;
		}

		const Utilities::AABB& GameObject::getBoundingBox() const
		{
			//QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			
			if (m_componentsManagerData.transform)
			{
				if (m_componentsManagerData.transform->isDirty())
				{
					updateTransformInternal();
				}
				//updated = true;
			}
			else
			{
				if (isColliderDirty())
					updateColliderData();
			}
			return m_boundingBox;
		}
		void GameObject::updateBoundingBox() const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_4);
			QSFML::vector<Utilities::AABB> boxes;
			boxes.reserve(m_componentsManagerData.colliders.size()+
			m_childObjectManagerData.objs.size());

			// get bounding boxes from child objects
			for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
			{
				m_childObjectManagerData.objs[i]->updateBoundingBox();
				boxes.push_back(m_childObjectManagerData.objs[i]->getBoundingBox());
			}
			
			for (size_t i = 0; i < m_componentsManagerData.colliders.size(); ++i)
			{
				boxes.push_back(m_componentsManagerData.colliders[i]->getBoundingBox());
			}
			if(m_getCustomBoundingBoxFunction)
				boxes.push_back((*m_getCustomBoundingBoxFunction)());
			if (boxes.size() == 0)
				m_boundingBox = Utilities::AABB();
			else
				m_boundingBox = Utilities::AABB::getFrame(boxes);
		}

		//void GameObject::add(GameObjectPtr child)
		//{
		//	addChild(child);
		//}
		//void GameObject::add(Components::ComponentPtr component)
		//{
		//	addComponent(component);
		//}
			 
		//void GameObject::remove(GameObjectPtr child)
		//{
		//	removeChild(child);
		//}
		//void GameObject::remove(Components::ComponentPtr component)
		//{
		//	removeComponent(component);
		//}
		//
		//void GameObject::deleteLater(GameObjectPtr child)
		//{
		//	deleteChildLater(child);
		//}
		//void GameObject::deleteLater(Components::ComponentPtr component)
		//{
		//	deleteComponentLater(component);
		//}

		
	



		void GameObject::updateChanges_componentsManager()
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			if (m_componentsManagerData.toAdd.size() == 0 &&
				m_componentsManagerData.sfDrawableToAdd.size() == 0 &&
				m_componentsManagerData.toRemove.size() == 0 &&
				m_componentsManagerData.sfDrawableToRemove.size() == 0 &&
				m_componentsManagerData.toDelete.size() == 0 &&
				m_componentsManagerData.sfDrawableToDelete.size() == 0)
				return;

			QSFML::vector<Components::ComponentPtr> newComps = m_componentsManagerData.toAdd;
			QSFML::vector<Components::ComponentPtr> toRemoveComps = m_componentsManagerData.toRemove;
			QSFML::vector<Components::ComponentPtr> toDeleteComps = m_componentsManagerData.toDelete;
			QSFML::vector<sf::Drawable*> sfDrawableNewComps = m_componentsManagerData.sfDrawableToAdd;
			QSFML::vector<sf::Drawable*> sfDrawableToRemoveComps = m_componentsManagerData.sfDrawableToRemove;
			QSFML::vector<sf::Drawable*> sfDrawableToDeleteComps = m_componentsManagerData.sfDrawableToDelete;
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove.clear();
			m_componentsManagerData.toDelete.clear();
			m_componentsManagerData.sfDrawableToAdd.clear();
			m_componentsManagerData.sfDrawableToRemove.clear();
			m_componentsManagerData.sfDrawableToDelete.clear();

			toRemoveComps.insert(toRemoveComps.end(), toDeleteComps.begin(), toDeleteComps.end());
			sfDrawableToRemoveComps.insert(sfDrawableToRemoveComps.end(), sfDrawableToDeleteComps.begin(), sfDrawableToDeleteComps.end());


			// Delete components
			for (auto& comp : toDeleteComps)
			{
				if (std::find(m_componentsManagerData.all.begin(), m_componentsManagerData.all.end(), comp) != m_componentsManagerData.all.end())
					Internal::LifetimeChecker::deleteSecured(comp);
			}
			for (auto& drawable : sfDrawableToDeleteComps)
			{
				auto it = std::find(m_componentsManagerData.sfDrawable.begin(), m_componentsManagerData.sfDrawable.end(), drawable);
				if (it != m_componentsManagerData.sfDrawable.end())
				{
					delete drawable;
				}
			}

			// Remove components
			size_t removedCount = 0;
			size_t addedCount = 0;
			for (auto& comp : toRemoveComps)
			{
				auto it = std::find(m_componentsManagerData.all.begin(), m_componentsManagerData.all.end(), comp);
				if (it != m_componentsManagerData.all.end())
				{
					++removedCount;
					if (Internal::LifetimeChecker::isAlive(comp))
					{
						if (comp->getParent())
						{
							comp->setParent(nullptr);
							comp->setSceneParent(nullptr);
						}
					}
					m_componentsManagerData.all.erase(it);
					if (m_componentsManagerData.transform == comp)
						m_componentsManagerData.transform = nullptr;

				
					auto itUpdatable = std::find(m_componentsManagerData.updatables.begin(), m_componentsManagerData.updatables.end(), dynamic_cast<Utilities::Updatable*>(comp));
					auto itColliders = std::find(m_componentsManagerData.colliders.begin(), m_componentsManagerData.colliders.end(), dynamic_cast<Components::Collider*>(comp));
					auto itEventHandler = std::find(m_componentsManagerData.eventHandler.begin(), m_componentsManagerData.eventHandler.end(), dynamic_cast<Components::SfEventHandle*>(comp));
					auto itDrawables = std::find(m_componentsManagerData.drawable.begin(), m_componentsManagerData.drawable.end(), dynamic_cast<Components::Drawable*>(comp));

				    if (itUpdatable != m_componentsManagerData.updatables.end())
						m_componentsManagerData.updatables.erase(itUpdatable);
					if (itColliders != m_componentsManagerData.colliders.end())
						m_componentsManagerData.colliders.erase(itColliders);
					if (itEventHandler != m_componentsManagerData.eventHandler.end())
						m_componentsManagerData.eventHandler.erase(itEventHandler);
					if (itDrawables != m_componentsManagerData.drawable.end())
						m_componentsManagerData.drawable.erase(itDrawables);
				}
			}
			for (auto& drawable : sfDrawableToRemoveComps)
			{
				auto it = std::find(m_componentsManagerData.sfDrawable.begin(), m_componentsManagerData.sfDrawable.end(), drawable);
				if (it != m_componentsManagerData.sfDrawable.end())
				{
					++removedCount;
					m_componentsManagerData.sfDrawable.erase(it);
				}
			}
			
			// Add components
			m_componentsManagerData.all.reserve(m_componentsManagerData.all.size() + newComps.size());
			m_componentsManagerData.sfDrawable.reserve(m_componentsManagerData.sfDrawable.size() + sfDrawableNewComps.size());
			for (auto& comp : newComps)
			{
				if (hasComponent(comp) || !comp)
					continue;
				++addedCount;
				m_componentsManagerData.all.push_back(comp);
				comp->setParent(this);
				comp->setSceneParent(m_sceneParent);

				// Check the type and sort it to the lists
				if (Components::Transform* transform = dynamic_cast<Components::Transform*>(comp))
				{
					m_componentsManagerData.transform = transform;
					markTransformDirty();
				}
				if (Utilities::Updatable* updatable = dynamic_cast<Utilities::Updatable*>(comp))
				{
					m_componentsManagerData.updatables.push_back(updatable);
				}
				if (Components::Collider* collider = dynamic_cast<Components::Collider*>(comp))
				{
					m_componentsManagerData.colliders.push_back(collider);
				}
				if (Components::SfEventHandle* eventHandler = dynamic_cast<Components::SfEventHandle*>(comp))
				{
					m_componentsManagerData.eventHandler.push_back(eventHandler);
				}
				if (Components::Drawable* drawable = dynamic_cast<Components::Drawable*>(comp))
				{
					m_componentsManagerData.drawable.push_back(drawable);
				}
			}
			for (auto& drawable : sfDrawableNewComps)
			{
				if (std::find(m_componentsManagerData.sfDrawable.begin(), m_componentsManagerData.sfDrawable.end(), drawable) != m_componentsManagerData.sfDrawable.end())
					continue;
				++addedCount;
				m_componentsManagerData.sfDrawable.push_back(drawable);
			}

			if (m_sceneParent)
			{
				m_sceneParent->removeComponent(removedCount);
				m_sceneParent->addComponent(addedCount);
			}
			needsEventUpdate(m_componentsManagerData.eventHandler.size() > 0 || m_onEventCallbacks.size() > 0);
			needsDrawUpdate(m_componentsManagerData.drawable.size() > 0 || 
							m_onDrawCallbacks.size() > 0 || 
							m_componentsManagerData.sfDrawable.size() ||
							m_enableDrawGizmos);
		}


		void GameObject::needsEventUpdateChanged(bool needsEventUpdate)
		{
			if (!m_componentsManagerData.thisNeedsEventUpdate && needsEventUpdate)
			{
				this->needsEventUpdate(needsEventUpdate);
				return;
			}
			if (m_componentsManagerData.thisNeedsEventUpdate && !needsEventUpdate)
			{
				if (m_componentsManagerData.eventHandler.size() == 0)
				{
					bool needsUpdate = false;
					for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
					{
						if (m_childObjectManagerData.objs[i]->m_componentsManagerData.thisNeedsEventUpdate)
						{
							needsUpdate = true;
							break;
						}
					}
					if (!needsUpdate)
					{
						this->needsEventUpdate(needsEventUpdate);
						return;
					}
				}
			}
		}
		void GameObject::needsEventUpdate(bool needsEventUpdate)
		{
			if (m_componentsManagerData.thisNeedsEventUpdate == needsEventUpdate)
				return;
			m_componentsManagerData.thisNeedsEventUpdate = needsEventUpdate;
			if (m_parent)
				m_parent->needsEventUpdateChanged(m_componentsManagerData.thisNeedsEventUpdate);
		}
		void GameObject::needsDrawUpdateChanged(bool needsDrawUpdate)
		{
			if (!m_componentsManagerData.thisNeedsDrawUpdate && needsDrawUpdate)
			{
				this->needsDrawUpdate(needsDrawUpdate);
				return;
			}
			if (m_componentsManagerData.thisNeedsDrawUpdate && !needsDrawUpdate)
			{
				if (m_componentsManagerData.drawable.size() == 0)
				{
					bool needsUpdate = false;
					for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
					{
						if (m_childObjectManagerData.objs[i]->m_componentsManagerData.thisNeedsDrawUpdate)
						{
							needsUpdate = true;
							break;
						}
					}
					if (!needsUpdate)
					{
						this->needsDrawUpdate(needsDrawUpdate);
						return;
					}
				}
			}
		}
		void GameObject::needsDrawUpdate(bool needsDrawUpdate)
		{
			//if (m_componentsManagerData.thisNeedsDrawUpdate == needsDrawUpdate)
			//	return;
			//if (getName() == "RuntimeInfo")
			//	logInfo("RuntimeInfo needs draw update: " + std::to_string(needsDrawUpdate));
			m_componentsManagerData.thisNeedsDrawUpdate = needsDrawUpdate;
			if (m_parent)
				m_parent->needsDrawUpdateChanged(m_componentsManagerData.thisNeedsDrawUpdate);
		}
		void GameObject::addEventFunction(const std::function<void(GameObject&, const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>>&)>& func) 
		{ 
			m_onEventCallbacks.push_back(func);
			needsEventUpdate(true);
		}

		void GameObject::addUpdateFunction(const std::function<void(GameObject&)>& func) 
		{ 
			//needsEventUpdate(true); 
			m_onUpdateCallbacks.push_back(func); 
		}
		void GameObject::addDrawFunction(const std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)>& func)
		{ 
			needsDrawUpdate(true); 
			m_onDrawCallbacks.push_back(func);
		}

		void GameObject::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_3);
			//states.transform *= getTransform();
			for (auto& comp : m_componentsManagerData.all)
			{
				comp->drawGizmos(target, states);
			}
			for (auto& obj : m_childObjectManagerData.objs)
			{
				obj->drawGizmos(target, states);
			}
		}

		bool GameObject::isColliderDirty() const
		{
			for (const auto& collider : m_componentsManagerData.colliders)
			{
				if (collider->isDirty())
				{
					return true;
				}
			}
			return false;
		}
		void GameObject::setCustomBoundingBoxFunction(const std::function<Utilities::AABB()>& func)
		{
			removeCustomBoundingBoxFunction();
			m_getCustomBoundingBoxFunction = new std::function<Utilities::AABB()>(func);
		}
		void GameObject::removeCustomBoundingBoxFunction()
		{
			std::function<Utilities::AABB()> *tmp = m_getCustomBoundingBoxFunction;
			m_getCustomBoundingBoxFunction = nullptr;
			delete tmp;
		}
		void GameObject::updateColliderData() const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			for (const auto& collider : m_componentsManagerData.colliders)
			{
				collider->updateColliderData();
			}
			updateBoundingBox();
		}
		bool GameObject::checkCollision(const GameObjectPtr other) const
		{
			QSFML::vector<Utilities::Collisioninfo> collisions;
			return checkCollision(other, collisions, true);
		}
		bool GameObject::checkCollision(const GameObjectPtr other,
			QSFML::vector<Utilities::Collisioninfo>& collisions,
			bool onlyFirstCollision) const
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			if (!other)
				return false;

			if(isColliderDirty())
				updateColliderData();
			if (other->isColliderDirty())
				other->updateColliderData();
			
			// Check if bounding box intersects
			const Utilities::AABB& otherBox = other->getBoundingBoxNoUpdate();
			if (!m_boundingBox.intersects(otherBox))
				return false;

			// Check for collisions
			const QSFML::vector<Components::Collider*>& otherColliders = other->m_componentsManagerData.colliders;
			bool hasCollision = false;
			for (auto thisCollider : m_componentsManagerData.colliders)
			{
				hasCollision |= thisCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
			}
			return hasCollision;
		}
		void GameObject::checkCollision(const Utilities::ObjectQuadTree& tree,
			QSFML::vector<Utilities::Collisioninfo>& collisions,
			bool onlyFirstCollision)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFML::list<GameObjectPtr> objs;
			tree.search(getBoundingBox(), objs);
			for (auto& obj : objs)
			{			
				if (obj == this)
					continue;
				const QSFML::vector<Components::Collider*>& otherColliders = obj->m_componentsManagerData.colliders;
				for (auto objCollider : m_componentsManagerData.colliders)
				{
					objCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
				}
			}
		}
		void GameObject::enableDrawGizmos(bool enable)
		{
			m_enableDrawGizmos = enable; 
			m_enableDrawGizmosRecursive = false; 
			needsDrawUpdate(m_componentsManagerData.drawable.size() > 0 ||
				m_onDrawCallbacks.size() > 0 ||
				m_componentsManagerData.sfDrawable.size() ||
				m_enableDrawGizmos);
		}
		void GameObject::enableDrawGizmosRecursive(bool enable)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			m_enableDrawGizmos = enable;
			m_enableDrawGizmosRecursive = enable;
			for (auto& obj : m_childObjectManagerData.objs)
			{
				obj->enableDrawGizmosRecursive(enable);
			}
			needsDrawUpdate(m_componentsManagerData.drawable.size() > 0 ||
				m_onDrawCallbacks.size() > 0 ||
				m_componentsManagerData.sfDrawable.size() ||
				m_enableDrawGizmos);
		}
	}
}