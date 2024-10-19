#include "GameObject.h"

/*
 GameObject implementation of the template functions
*/

namespace QSFML
{
    namespace Objects
    {


template <typename T>
void GameObject::addComponent(T* component)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        m_componentsManagerData.toAdd.push_back(component);
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToAdd.push_back(component);
        onObjectsChanged();
    }
    else
    {
        static_assert(false, "GameObject::addComponent(): Unsupported type");
    }
}

template <typename T>
void GameObject::addComponents(const QSFML::vector<T*>& components)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        m_componentsManagerData.toAdd.insert(m_componentsManagerData.toAdd.end(), components.begin(), components.end());
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToAdd.insert(m_componentsManagerData.sfDrawableToAdd.end(), components.begin(), components.end());
        onObjectsChanged();
    }
    else
    {
        static_assert(false, "GameObject::addComponents(): Unsupported type");
    }
}
template <typename T>
void GameObject::removeComponent(T* component)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
		m_componentsManagerData.toRemove.push_back(component);
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
		m_componentsManagerData.sfDrawableToRemove.push_back(component);
        onObjectsChanged();
    }
    else
    {
        static_assert(false, "GameObject::removeComponent(): Unsupported type");
    }
}
template <typename T>
void GameObject::removeComponents(const QSFML::vector<T*>& components)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        m_componentsManagerData.toRemove.insert(m_componentsManagerData.toRemove.end(), components.begin(), components.end());
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToRemove.insert(m_componentsManagerData.sfDrawableToRemove.end(), components.begin(), components.end());
        onObjectsChanged();
    }
    else
    {
        static_assert(false, "GameObject::removeComponents(): Unsupported type");
    }
}



template<typename T>
void GameObject::removeChilds()
{
    m_childObjectManagerData.toRemove.reserve(m_childObjectManagerData.toRemove.size() + m_childObjectManagerData.objs.size());
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (dynamic_cast<T>(obj))
        {
            m_childObjectManagerData.toRemove.push_back(obj);
        }
    }
}

template<typename T>
T* GameObject::getFirstChild() const
{
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            return child;
        }
    }
    return nullptr;
}
template<typename T>
QSFML::vector<T*> GameObject::getChilds() const
{
    QSFML::vector<T*> childs;
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            childs.push_back(child);
        }
    }
    return childs;
}

template<typename T>
QSFML::vector<T*> GameObject::getChildsRecusrive() const
{
    QSFML::vector<T*> childs;
    for (auto& obj : m_childObjectManagerData.objs)
    {
        if (T* child = dynamic_cast<T*>(obj))
        {
            childs.push_back(child);
        }
        QSFML::vector<T*> childChilds = obj->getChildsRecusrive<T>();
        childs.insert(childs.end(), childChilds.begin(), childChilds.end());
    }
    return childs;
}


template <typename T>
void GameObject::removeComponents()
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.all.size());
        bool hasChanges = false;
        for (auto& comp : m_componentsManagerData.all)
        {
            if (dynamic_cast<T>(comp))
            {
                hasChanges = true;
                m_componentsManagerData.toRemove.push_back(component);
            }
        }
		if (hasChanges)
			onObjectsChanged();
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToRemove.reserve(m_componentsManagerData.sfDrawableToRemove.size() + m_componentsManagerData.sfDrawable.size());
        bool hasChanges = false;
        for (auto& comp : m_componentsManagerData.sfDrawable)
        {
            if (dynamic_cast<T>(comp))
            {
                hasChanges = true;
                m_componentsManagerData.sfDrawableToRemove.push_back(component);
            }
        }
        if (hasChanges)
            onObjectsChanged();
    }   
}
template <>
void GameObject::removeComponents<Components::Collider>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.colliders.size());
    for (size_t i = 0; i < m_componentsManagerData.colliders.size(); ++i)
    {
        Components::ComponentPtr component = static_cast<Components::Component*>(m_componentsManagerData.colliders[i]);
        removeComponent(component);
    }
}
template <>
void GameObject::removeComponents<Utilities::Updatable>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.updatables.size());
    for (auto& comp : m_componentsManagerData.updatables)
    {
        removeComponent(dynamic_cast<Components::Component*>(comp));
    }
}
template <>
void GameObject::removeComponents<Components::SfEventHandle>()
{
    m_componentsManagerData.toRemove.reserve(m_componentsManagerData.toRemove.size() + m_componentsManagerData.eventHandler.size());
    for (size_t i = 0; i < m_componentsManagerData.eventHandler.size(); ++i)
    {
        removeComponent(dynamic_cast<Components::Component*>(m_componentsManagerData.eventHandler[i]));
    }
}
template <>
void GameObject::removeComponents<Components::Transform>()
{
    removeComponent(static_cast<Components::Component*>(m_componentsManagerData.transform));
}

template <typename T, typename>
void GameObject::deleteComponentLater(T sfDrawable)
{
	// Overload for sf::Drawable* types, but exclude Component* types
	m_componentsManagerData.sfDrawableToDelete.push_back(sfDrawable);
	onObjectsChanged();
}

template <typename T>
T* GameObject::getFirstComponent() const
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        for (auto& comp : m_componentsManagerData.all)
        {
            if (T* t = dynamic_cast<T*>(comp))
            {
                return t;
            }
        }
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
		if (m_componentsManagerData.sfDrawable.size() > 0)
			return m_componentsManagerData.sfDrawable[0];
	}
    
    return nullptr;
}

template <>
Components::Collider* GameObject::getFirstComponent<Components::Collider>() const
{
    if (m_componentsManagerData.colliders.size() > 0)
        return m_componentsManagerData.colliders[0];
    return nullptr;
}
template <>
Utilities::Updatable* GameObject::getFirstComponent<Utilities::Updatable>() const
{
    if (m_componentsManagerData.updatables.size() > 0)
        return m_componentsManagerData.updatables[0];
    return nullptr;
}
template <>
Components::SfEventHandle* GameObject::getFirstComponent<Components::SfEventHandle>() const
{
    if (m_componentsManagerData.eventHandler.size() > 0)
        return m_componentsManagerData.eventHandler[0];
    return nullptr;
}
template <>
Components::Transform* GameObject::getFirstComponent<Components::Transform>() const
{
    return m_componentsManagerData.transform;
}


template <typename T>
QSFML::vector<T*> GameObject::getComponents() const
{
    QSFML::vector<T*> components;
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        components.reserve(m_componentsManagerData.all.size());
        for (auto& comp : m_componentsManagerData.all)
        {
            if (T* t = dynamic_cast<T*>(comp))
            {
                components.push_back(t);
            }
        }
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
		components = m_componentsManagerData.sfDrawable;
    }
    return components;
}
template <>
QSFML::vector<Components::Collider*> GameObject::getComponents<Components::Collider>() const
{
    return m_componentsManagerData.colliders;
}
template <>
QSFML::vector<Utilities::Updatable*> GameObject::getComponents<Utilities::Updatable>() const
{
    return m_componentsManagerData.updatables;
}
template <>
QSFML::vector<Components::SfEventHandle*> GameObject::getComponents<Components::SfEventHandle>() const
{
    return m_componentsManagerData.eventHandler;
}
template <>
QSFML::vector<Components::Transform*> GameObject::getComponents<Components::Transform>() const
{
    return { m_componentsManagerData.transform };
}

template <typename T>
QSFML::vector<T*> GameObject::getComponentsRecursive() const
{
    QSFML::vector<T*> comps = getComponents<T>();
    for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
    {
        QSFML::vector<T*> comps2 = m_childObjectManagerData.objs[i]->getComponentsRecursive<T>();
        comps.insert(comps.end(), comps2.begin(), comps2.end());
    }
    return comps;
}


template<typename T>
size_t GameObject::getComponentCount() const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t count = 0;
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        for (auto& comp : m_componentsManagerData.all)
        {
            if (T* t = dynamic_cast<T*>(comp))
            {
                ++count;
            }
        }
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        count = m_componentsManagerData.sfDrawable.size();
    }
   
    return count;
}
template<typename T>
size_t GameObject::getComponentCountRecusrive() const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t count = getComponentCount<T>();
    for (auto& obj : m_childObjectManagerData.objs)
    {
        count += obj->getComponentCountRecusrive<T>();
    }
    return count;
}
template <typename T>
void GameObject::add(T* item)
{
	if constexpr (std::is_base_of<Components::Component, T>::value)
	{
		addComponent(item);
	}
	else if constexpr (std::is_base_of<sf::Drawable, T>::value)
	{
        m_componentsManagerData.sfDrawableToAdd.push_back(item);
        onObjectsChanged();
	}
	else if constexpr (std::is_base_of<GameObject, T>::value)
	{
		addChild(item);
	}  
    else
	{
		static_assert(false, "GameObject::add(): Unsupported type");
	}
}

template <typename T>
void GameObject::remove(T* item)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        removeComponent(item);
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToRemove.push_back(item);
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<GameObject, T>::value)
    {
        removeChild(item);
    }
    else
    {
        static_assert(false, "GameObject::remove(): Unsupported type");
    }
}

template <typename T>
void GameObject::deleteLater(T* item)
{
    if constexpr (std::is_base_of<Components::Component, T>::value)
    {
        deleteComponentLater(item);
    }
    else if constexpr (std::is_base_of<sf::Drawable, T>::value)
    {
        m_componentsManagerData.sfDrawableToDelete.push_back(item);
        onObjectsChanged();
    }
    else if constexpr (std::is_base_of<GameObject, T>::value)
    {
        deleteChildLater(item);
    }
    else
    {
        static_assert(false, "GameObject::deleteLater(): Unsupported type");
    }
}

}
}
