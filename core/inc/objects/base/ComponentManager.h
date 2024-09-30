#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "components/base/SfEventHandle.h

#include "components/physics/Collider.h"
#include "components/Transform.h"

#include "utilities/Updateable.h"

namespace QSFML
{
	namespace Objects
	{
		class GameObject;
		namespace Internal
		{
			class QSFML_EDITOR_WIDGET_EXPORT ComponentManager
			{
				friend GameObject;
				ComponentManager(GameObject &parent);
				virtual ~ComponentManager();
				public:	
				

				void addComponent(Components::ComponentPtr component);
				void addComponents(const std::vector<Components::ComponentPtr>& components);

				void removeComponent(Components::ComponentPtr component);
				void removeComponents(const std::vector<Components::ComponentPtr>& components);
				template <typename T> void removeComponents();
				void clearComponents();

				Components::ComponentPtr getComponent(const std::string& name) const;
				const std::vector<Components::ComponentPtr>& getComponents() const;
				template <typename T> std::vector<std::shared_ptr<T>> getComponents() const;

				bool hasComponent(const std::string& name) const;
				bool hasComponent(Components::ComponentPtr component) const;



				size_t getComponentCount() const { return m_components.size(); }
				size_t getComponentCountRecursive() const;
				template<typename T>
				size_t getComponentCount() const;
				template<typename T>
				size_t getComponentCountRecusrive() const;

				Components::ComponentPtr findFirstComponent(const std::string& name);
				std::vector<Components::ComponentPtr> findAllComponents(const std::string& name);

				Components::ComponentPtr findFirstComponentRecursive(const std::string& name);
				std::vector<Components::ComponentPtr> findAllComponentsRecursive(const std::string& name);





				bool hasEventHandlers() const { return !m_eventHandler.empty(); }
				bool hasColliders() const { return !m_colliders.empty(); }
				bool hasUpdatables() const { return !m_updatables.empty(); }
				bool hasTransform() const { return m_transform != nullptr; }

				private:
				void updateChanges();
				void sortComponent(Components::ComponentPtr comp);


				GameObject& m_parent;

				struct ComponentManagerData
				{
					std::vector<Components::ComponentPtr> all;

					std::vector<std::shared_ptr<Utilities::Updatable>> updatables;
					std::vector<std::shared_ptr<Components::Collider>> colliders;
					std::vector<std::shared_ptr<Components::SfEventHandle>> eventHandler;
					std::shared_ptr<Components::Transform> transform;

					std::vector<Components::ComponentPtr> toAdd;
					std::vector<Components::ComponentPtr> toRemove;
				}
				std::vector<Components::ComponentPtr> m_components;

				// Container for specializations
				std::vector<std::shared_ptr<Utilities::Updatable>> m_updatables;
				std::vector<std::shared_ptr<Components::Collider>> m_colliders;
				std::vector<std::shared_ptr<Components::SfEventHandle>> m_eventHandler;
				std::shared_ptr<Components::Transform> m_transform;


				std::vector<Components::ComponentPtr> m_toAddComponents;
				std::vector<Components::ComponentPtr> m_toRemoveComponents;
			};


			template <typename T> 
			void ComponentManager::removeComponents()
			{
				m_toRemoveComponents.reserve(m_toRemoveComponents.size() + m_components.size());
				for (auto& comp : m_components)
				{
					if (std::dynamic_pointer_cast<T>(comp))
					{
						removeComponent(comp);
					}
				}
			}
			template <>
			void ComponentManager::removeComponents<Components::Collider>()
			{
				m_toRemoveComponents.reserve(m_toRemoveComponents.size() + m_colliders.size());
				for (auto& comp : m_colliders)
				{
					removeComponent(std::dynamic_pointer_cast<Components::Component>(comp));
				}
			}
			template <>
			void ComponentManager::removeComponents<Components::Updateable>()
			{
				m_toRemoveComponents.reserve(m_toRemoveComponents.size() + m_updatables.size());
				for (auto& comp : m_updatables)
				{
					removeComponent(std::dynamic_pointer_cast<Components::Component>(comp));
				}
			}
			template <>
			void ComponentManager::removeComponents<Components::SfEventHandle>()
			{
				m_toRemoveComponents.reserve(m_toRemoveComponents.size() + m_eventHandler.size());
				for (auto& comp : m_eventHandler)
				{
					removeComponent(std::dynamic_pointer_cast<Components::Component>(comp));
				}
			}
			template <>
			void ComponentManager::removeComponents<Components::Transform>()
			{
				removeComponent(std::dynamic_pointer_cast<Components::Component>(m_transform));
			}


			template <typename T>
			std::vector<std::shared_ptr<T>> ComponentManager::getComponents()
			{
				std::vector<std::shared_ptr<T>> components;
				components.reserve(m_components.size());
				for (auto& comp : m_components)
				{
					if (std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(comp))
					{
						components.push_back(t);
					}
				}
				return components;
			}
			template <>
			std::vector<std::shared_ptr<Components::Collider>> ComponentManager::getComponents()
			{
				return m_colliders;
			}
			template <>
			std::vector<std::shared_ptr<Components::Updateable>> ComponentManager::getComponents()
			{
				return m_updatables;
			}
			template <>
			std::vector<std::shared_ptr<Components::SfEventHandle>> ComponentManager::getComponents()
			{
				return m_eventHandler;
			}
			template <>
			std::vector<std::shared_ptr<Components::Transform>> ComponentManager::getComponents()
			{
				return { m_transform };
			}

			template<typename T>
			size_t ComponentManager::getComponentCountRecusrive() const
			{
				size_t count = 0;
				for (auto& comp : m_components)
				{
					if (std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(comp))
					{
						++count;
					}
				}
				// <! ToDo recursive

				return count;
			}
		}
	}
}