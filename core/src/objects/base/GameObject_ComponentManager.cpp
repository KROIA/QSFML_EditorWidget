#include "objects/base/GameObject.h

namespace QSFML
{
	namespace Objects
	{
		void GameObject::addComponent(Components::ComponentPtr component)
		{
			m_componentsManagerData.toAdd.push_back(component);
		}
		void GameObject::addComponents(const std::vector<Components::ComponentPtr>& components)
		{
			m_componentsManagerData.toAdd.insert(m_componentsManagerData.toAdd.end(), components.begin(), components.end());
		}

		void GameObject::removeComponent(Components::ComponentPtr component)
		{
			m_componentsManagerData.toRemove.push_back(component);
		}
		void GameObject::removeComponents(const std::vector<Components::ComponentPtr>& components)
		{
			m_componentsManagerData.toRemove.insert(m_componentsManagerData.toRemove.end(), components.begin(), components.end());
		}
		void GameObject::clearComponents()
		{
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove = m_componentsManagerData.all;
		}

		Components::ComponentPtr GameObject::getComponent(const std::string& name) const
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}
			return nullptr;
		}
		const std::vector<Components::ComponentPtr>& GameObject::getComponents() const
		{
			return m_componentsManagerData.all;
		}

		bool GameObject::hasComponent(const std::string& name) const
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return true;
			}
			return false;
		}
		bool GameObject::hasComponent(Components::ComponentPtr component) const
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp == component)
					return true;
			}
			return false;
		}
		size_t GameObject::getComponentCountRecursive() const
		{
			size_t count = m_componentsManagerData.all.size();
			// <! ToDo recursive

			return count;
		}


		Components::ComponentPtr GameObject::findFirstComponent(const std::string& name)
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}
			return nullptr;
		}
		std::vector<Components::ComponentPtr> GameObject::findAllComponents(const std::string& name)
		{
			std::vector<Components::ComponentPtr> comps;
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					comps.push_back(comp);
			}
			return comps;
		}

		Components::ComponentPtr GameObject::findFirstComponentRecursive(const std::string& name)
		{
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					return comp;
			}

			// <! ToDo recursive
			return nullptr;
		}
		std::vector<Components::ComponentPtr> GameObject::findAllComponentsRecursive(const std::string& name)
		{
			std::vector<Components::ComponentPtr> comps;
			for (auto& comp : m_componentsManagerData.all)
			{
				if (comp->getName() == name)
					comps.push_back(comp);
			}

			// <! ToDo recursive
			return comps;
		}


		void GameObject::updateChanges_componentsManager()
		{
			std::vector<Components::ComponentPtr> newComps = m_componentsManagerData.toAdd;
			std::vector<Components::ComponentPtr> toRemoveComps = m_componentsManagerData.toRemove;
			m_componentsManagerData.toAdd.clear();
			m_componentsManagerData.toRemove.clear();

			// Remove components
			for (auto& comp : toRemoveComps)
			{
				auto it = std::find(m_componentsManagerData.all.begin(), m_componentsManagerData.all.end(), comp);
				if (it != m_componentsManagerData.all.end())
				{
					m_componentsManagerData.all.erase(it);
					if (m_componentsManagerData.transform == comp)
						m_componentsManagerData.transform = nullptr;
				    if (std::find(m_componentsManagerData.updatables.begin(), m_componentsManagerData.updatables.end(), comp) != m_componentsManagerData.updatables.end())
						m_componentsManagerData.updatables.erase(std::find(m_componentsManagerData.updatables.begin(), m_componentsManagerData.updatables.end(), comp));
					if (std::find(m_componentsManagerData.colliders.begin(), m_componentsManagerData.colliders.end(), comp) != m_componentsManagerData.colliders.end())
						m_componentsManagerData.colliders.erase(std::find(m_componentsManagerData.colliders.begin(), m_componentsManagerData.colliders.end(), comp));
					if (std::find(m_componentsManagerData.eventHandler.begin(), m_componentsManagerData.eventHandler.end(), comp) != m_componentsManagerData.eventHandler.end())
						m_componentsManagerData.eventHandler.erase(std::find(m_componentsManagerData.eventHandler.begin(), m_componentsManagerData.eventHandler.end(), comp));
				}
			}

			// Add components
			m_componentsManagerData.all.reserve(m_componentsManagerData.all.size() + newComps.size());
			for (auto& comp : newComps)
			{
				if (hasComponent(comp))
					continue;
				m_componentsManagerData.all.push_back(comp);
				sortComponent(comp);
			}
		}
		void GameObject::sortComponent(Components::ComponentPtr comp)
		{
			// Check the type and sort it to the lists
			if (std::shared_ptr<Components::Transform> transform = std::dynamic_pointer_cast<Components::Transform>(comp))
			{
				m_componentsManagerData.transform = transform;
			}
			if (std::shared_ptr<Utilities::Updatable> updatable = std::dynamic_pointer_cast<Utilities::Updatable>(comp))
			{
				m_componentsManagerData.updatables.push_back(updatable);
			}
			if (std::shared_ptr<Components::Collider> collider = std::dynamic_pointer_cast<Components::Collider>(comp))
			{
				m_componentsManagerData.colliders.push_back(collider);
			}
			if (std::shared_ptr<Components::SfEventHandle> eventHandler = std::dynamic_pointer_cast<Components::SfEventHandle>(comp))
			{
				m_componentsManagerData.eventHandler.push_back(eventHandler);
			}				
		}
	}
}