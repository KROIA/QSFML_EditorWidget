#include "objects/base/GameObject.h"
#include "Scene/Scene.h"
#include "utilities/ObjectQuadTree.h"
#include "utilities/LifetimeChecker.h"


namespace QSFML
{
	namespace Objects
	{
		void GameObject::addComponent(Components::ComponentPtr component)
		{
			m_componentsManagerData.toAdd.push_back(component);
			onObjectsChanged();
		}
		void GameObject::addComponents(const std::vector<Components::ComponentPtr>& components)
		{
			m_componentsManagerData.toAdd.insert(m_componentsManagerData.toAdd.end(), components.begin(), components.end());
			onObjectsChanged();
		}

		/*void GameObject::createTransform()
		{
			if (!m_componentsManagerData.transform)
			{
				Components::Transform* transform = new Components::Transform();
				m_componentsManagerData.transform = transform;
				addComponent(transform);
			}
		}*/

		void GameObject::removeComponent(Components::ComponentPtr component)
		{
			m_componentsManagerData.toRemove.push_back(component);
			onObjectsChanged();
		}
		void GameObject::removeComponents(const std::vector<Components::ComponentPtr>& components)
		{
			m_componentsManagerData.toRemove.insert(m_componentsManagerData.toRemove.end(), components.begin(), components.end());
			onObjectsChanged();
		}

		
		void GameObject::clearComponents()
		{
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove = m_componentsManagerData.all;
			onObjectsChanged();
		}

		void GameObject::deleteComponentLater(Components::ComponentPtr component)
		{
			m_componentsManagerData.toDelete.push_back(component); 
			onObjectsChanged();
		}

		/*Components::ComponentPtr GameObject::getComponent(const std::string& name) const
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}
			return nullptr;
		}*/
		const std::vector<Components::ComponentPtr>& GameObject::getComponents() const
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
		std::vector<Components::ComponentPtr> GameObject::getAllComponents(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			std::vector<Components::ComponentPtr> comps;
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
		std::vector<Components::ComponentPtr> GameObject::getAllComponentsRecursive(const std::string& name)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			std::vector<Components::ComponentPtr> comps;
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					comps.push_back(comp);
			}
			for (auto& obj : m_childObjectManagerData.objs)
			{
				std::vector<Components::ComponentPtr> childComps = obj->getAllComponentsRecursive(name);
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
			std::vector<Utilities::AABB> boxes;
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
	



		void GameObject::updateChanges_componentsManager()
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
			if (m_componentsManagerData.toAdd.size() == 0 &&
				m_componentsManagerData.toRemove.size() == 0 &&
				m_componentsManagerData.toDelete.size() == 0)
				return;

			std::vector<Components::ComponentPtr> newComps = m_componentsManagerData.toAdd;
			std::vector<Components::ComponentPtr> toRemoveComps = m_componentsManagerData.toRemove;
			std::vector<Components::ComponentPtr> toDeleteComps = m_componentsManagerData.toDelete;
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove.clear();
			m_componentsManagerData.toDelete.clear();

			toRemoveComps.insert(toRemoveComps.end(), toDeleteComps.begin(), toDeleteComps.end());


			// Delete components
			for (auto& comp : toDeleteComps)
			{
				if (std::find(m_componentsManagerData.all.begin(), m_componentsManagerData.all.end(), comp) != m_componentsManagerData.all.end())
					Internal::LifetimeChecker::deleteSecured(comp);
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

			
			// Add components
			m_componentsManagerData.all.reserve(m_componentsManagerData.all.size() + newComps.size());
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

			if (m_sceneParent)
			{
				m_sceneParent->removeComponent(removedCount);
				m_sceneParent->addComponent(addedCount);
			}
			needsEventUpdate(m_componentsManagerData.eventHandler.size() > 0 || m_onEventCallbacks.size() > 0);
			needsDrawUpdate(m_componentsManagerData.drawable.size() > 0 || m_onDrawCallbacks.size() > 0);
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

		void GameObject::addUpdateFunction(const std::function<void(GameObject&)>& func) 
		{ 
			needsEventUpdate(true); 
			m_onUpdateCallbacks.push_back(func); 
		}
		void GameObject::addDrawFunction(const std::function<void(const GameObject&, sf::RenderTarget&, sf::RenderStates)>& func)
		{ 
			needsDrawUpdate(true); 
			m_onDrawCallbacks.push_back(func);
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
			std::vector<Utilities::Collisioninfo> collisions;
			return checkCollision(other, collisions, true);
		}
		bool GameObject::checkCollision(const GameObjectPtr other,
			std::vector<Utilities::Collisioninfo>& collisions,
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
			const std::vector<Components::Collider*>& otherColliders = other->m_componentsManagerData.colliders;
			bool hasCollision = false;
			for (auto thisCollider : m_componentsManagerData.colliders)
			{
				hasCollision |= thisCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
			}
			return hasCollision;
		}
		void GameObject::checkCollision(const Utilities::ObjectQuadTree& tree,
			std::vector<Utilities::Collisioninfo>& collisions,
			bool onlyFirstCollision)
		{
			QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
			std::list<GameObjectPtr> objs;
			tree.search(getBoundingBox(), objs);
			for (auto& obj : objs)
			{			
				if (obj == this)
					continue;
				const std::vector<Components::Collider*>& otherColliders = obj->m_componentsManagerData.colliders;
				for (auto objCollider : m_componentsManagerData.colliders)
				{
					objCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
				}
			}
		}
	}
}