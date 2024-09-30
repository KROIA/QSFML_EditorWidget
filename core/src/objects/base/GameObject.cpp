#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

#include "components/base/Component.h"
#include "components/base/SfEventHandle.h"
#include "components/base/Drawable.h"
#include "components/physics/Collider.h"

#include "utilities/Stats.h"
#include "utilities/ObjectQuadTree.h"

#include <QDebug>
#include <typeinfo>
    


using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

OBJECT_IMPL(GameObject)

size_t GameObject::s_objNameCounter = 0;
GameObject::GameObject(const std::string &name, GameObject *parent)
    : Transformable()
    , Updatable()
    , DestroyEvent()
{
    m_name = name;
    if(m_name.size() == 0)
    {
        s_objNameCounter++;
        m_name = "GameObject_"+std::to_string(s_objNameCounter);
    }
    m_updateControlls.enableUpdateLoop = true;
    m_updateControlls.enableEventLoop = true;
    m_updateControlls.enablePaintLoop = true;
   
    m_SceneParent = nullptr;
    m_parent = parent;
    m_rootParent = this;
    if(m_parent)
        m_rootParent = m_parent->m_rootParent;
    m_objectsChanged = false;
    m_thisNeedsDrawUpdate = false;
    m_enabled = true;
    //m_position = sf::Vector2f(0, 0);
    m_renderLayer = RenderLayer::layer_0;
    
}
GameObject::GameObject(const GameObject &other)
    : Transformable()
    , Updatable()
    , DestroyEvent()
{
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    //m_position = other.m_position;
    m_SceneParent = nullptr;
    m_parent = nullptr;
    m_rootParent = this;
    m_objectsChanged = false;
    m_thisNeedsDrawUpdate = false;
    m_updateControlls = other.m_updateControlls;
    m_renderLayer = other.m_renderLayer;

    m_childs.reserve(other.m_childs.size());
    for(size_t i=0; i<other.m_childs.size(); ++i)
    {
        GameObject *obj = other.m_childs[i]->clone();
        addChild(obj);
    }
    m_components.reserve(other.m_components.size());
    for(size_t i=0; i<other.m_components.size(); ++i)
    {
        Component *obj = other.m_components[i]->clone();
        addComponent(obj);
    }
    updateNewElements();
}
GameObject::~GameObject()
{
    for(size_t i=0; i<m_childs.size(); ++i)
        delete m_childs[i];
    m_childs.clear();

    
    for(auto &comp : m_components)
        delete comp;
    m_components.clear();
    m_toAddComponents.clear();
    m_updatableComponents.clear();
    m_drawableComponents.clear();
	//m_transformables.clear();
    m_toRemoveComponents.clear();
    m_toDeleteChilds.clear();
    m_toRemoveChilds.clear();
    if (m_SceneParent)
    {
        m_SceneParent->removeGameObject();
        m_SceneParent->removeComponent(m_components.size());
        m_SceneParent->removeObject(this);   
    }
}


void GameObject::inSceneAdded()
{

}
void GameObject::setParent(GameObject *parent)
{
    if(parent == m_parent)
        return;
    if(m_parent)
    {
        m_parent->removeChild(this);
    }
    if(parent && parent != this)
    {
        parent->addChild_internal(this);
        return;
    }
}
void GameObject::setEnabled(bool enable)
{
    m_enabled = enable;
}
bool GameObject::isEnabled() const
{
    return m_enabled;
}
GameObject *GameObject::getParent() const
{
    return m_parent;
}
GameObject *GameObject::getRootParent() const
{
    return m_rootParent;
}

void GameObject::setName(const std::string &name)
{
    m_name = name;
}
const std::string &GameObject::getName() const
{
    return m_name;
}

double GameObject::getAge() const
{
    if (!m_SceneParent) return 0;
    return m_SceneParent->getElapsedTime() - m_birthTime;
}
double GameObject::getBirthTime() const
{
    return m_birthTime;
}
size_t GameObject::getAgeTicks() const
{
    if (!m_SceneParent) return 0;
    return m_SceneParent->getTick() - m_birthTick;
}
size_t GameObject::getBirthTick() const
{
    return m_birthTick;
}
double GameObject::getAgeFixed() const
{
    if (!m_SceneParent) return 0;
    return m_SceneParent->getFixedDeltaT() * (m_SceneParent->getTick() - m_birthTick);
}


/*void GameObject::setPositionRelative(const sf::Vector2f& pos)
{
    m_position = pos;
    for (size_t i = 0; i < m_colliders.size(); ++i)
        m_colliders[i]->setPos(m_position);
}
void GameObject::setPosition(const sf::Vector2f& pos)
{
    if (m_parent)
    {
        sf::Vector2f parentPos = m_parent->getPosition();
        m_position = pos - parentPos;
    }
    else
        m_position = pos;
    for (size_t i = 0; i < m_colliders.size(); ++i)
        m_colliders[i]->setPos(m_position);
}
const sf::Vector2f& GameObject::getPositionRelative() const
{
    return m_position;
}
sf::Vector2f GameObject::getPosition() const
{
    if (m_parent)
    {
        sf::Vector2f parentPos = m_parent->getPosition();
        return m_position + parentPos;
    }
    return m_position;
}*/
sf::Vector2f GameObject::getGlobalPosition() const
{
    sf::Vector2f pos = getPosition();
    GameObject *parent = m_parent;
    while (parent)
    {
        pos += parent->getPosition();
        parent = parent->m_parent;
    }
    return pos;
}
float GameObject::getGlobalRotation() const
{
	float rot = getRotation();
	GameObject *parent = m_parent;
	while (parent)
	{
		rot += parent->getRotation();
		parent = parent->m_parent;
	}
	return rot;
}

void GameObject::setRenderLayer(RenderLayer layer)
{
    if(m_renderLayer == layer)
        return;
    RenderLayer oldLayer = m_renderLayer;
    m_renderLayer = layer;
    if(m_SceneParent)
        m_SceneParent->renderLayerSwitch(this, oldLayer, m_renderLayer);
}

RenderLayer GameObject::getRenderLayer() const
{
    return m_renderLayer;
}

// Childs operations
void GameObject::addChild(GameObject *child)
{
    if(!child)return;

    m_toAddChilds.push_back(child);
    onObjectsChanged();

}
void GameObject::addChilds(const std::vector<GameObject*>& childs)
{
    for (size_t i = 0; i < childs.size(); ++i)
    {
        GameObject* child = childs[i];
        if(child)
            m_toAddChilds.push_back(child);
    }
    onObjectsChanged();
}

void GameObject::removeChild(GameObject *child)
{
    if(!child)return;
    size_t index = getChildIndex(child);
    if(index == npos) return;
    for(size_t i=0; i<m_toDeleteChilds.size(); ++i)
        if(m_toDeleteChilds[i] == child)
            return;
    onObjectsChanged();
    m_toRemoveChilds.push_back(child);
}
void GameObject::removeChilds(const std::vector<GameObject*>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        GameObject* child = childs[j];
        if (!child)
            continue;
        size_t index = getChildIndex(child);
        if (index == npos) 
            continue;
        for (size_t i = 0; i < m_toDeleteChilds.size(); ++i)
            if (m_toDeleteChilds[i] == child)
                break;
        
        m_toRemoveChilds.push_back(child);
    }
    onObjectsChanged();
}
void GameObject::removeChilds()
{
    m_toRemoveChilds = m_childs;
    m_toAddChilds.clear();
    onObjectsChanged();
}
void GameObject::removeChild_internal()
{
    for(size_t i=0; i<m_toRemoveChilds.size(); ++i)
    {
        size_t index = getChildIndex(m_toRemoveChilds[i]);
        if(index != npos)
            m_childs.erase(m_childs.begin() + index);
    }
    m_toRemoveChilds.clear();
}

void GameObject::deleteChild(GameObject *child)
{
    if(!child)return;

    for(size_t i=0; i<m_toDeleteChilds.size(); ++i)
        if(m_toDeleteChilds[i] == child)
            return;
    for(size_t i=0; i<m_toAddChilds.size(); ++i)
        if(m_toAddChilds[i] == child)
        {
            m_toAddChilds.erase(m_toAddChilds.begin() + i);
            break;
        }
    onObjectsChanged();
    m_toDeleteChilds.push_back(child);
}
void GameObject::deleteChilds(const std::vector<GameObject*>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        GameObject* child = childs[j];
        if (!child)
            continue;

        for (size_t i = 0; i < m_toDeleteChilds.size(); ++i)
            if (m_toDeleteChilds[i] == child)
                continue;
        for (size_t i = 0; i < m_toAddChilds.size(); ++i)
            if (m_toAddChilds[i] == child)
            {
                m_toAddChilds.erase(m_toAddChilds.begin() + i);
                break;
            }
        m_toDeleteChilds.push_back(child);
    }
    onObjectsChanged();
}
void GameObject::deleteChilds()
{
    m_toDeleteChilds = m_childs;
    m_toRemoveChilds.clear();
    m_toAddChilds.clear();
    onObjectsChanged();
}
void GameObject::deleteChild_internal()
{
    for(size_t i=0; i<m_toDeleteChilds.size(); ++i)
    {
        size_t index = getChildIndex(m_toDeleteChilds[i]);
        if(index != npos)
            m_childs.erase(m_childs.begin() + index);
        qDebug() << "GameObject::deleteChild_internal(): "<<m_toDeleteChilds[i]->getName().c_str() << " child of: "<<getName().c_str();
        delete m_toDeleteChilds[i];
    }
    m_toDeleteChilds.clear();
}
bool GameObject::childExists(GameObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return true;
    return false;
}
size_t GameObject::getChildIndex(GameObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return i;
    return npos;
}
const std::vector<GameObject*> &GameObject::getChilds() const
{
    return m_childs;
}
size_t GameObject::getChildCount() const
{
    return m_childs.size();
}

GameObject* GameObject::findFirstChild(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t nameSize = name.size();
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        const std::string& objName = m_childs[i]->getName();
        if (nameSize != objName.size())
            continue;
        if (objName == name)
        {
            return m_childs[i];
        }
        //found |= m_childs[i]->findAllChilds_internal(name, foundList);
    }
    return nullptr;
}
std::vector<GameObject*> GameObject::findAllChilds(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<GameObject*> results;
    results.reserve(m_childs.size());
    findAllChilds_internal(name, results);
    return results;
}

GameObject* GameObject::findFirstChildRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t nameSize = name.size();
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        const std::string& objName = m_childs[i]->getName();
        if (nameSize != objName.size())
            continue;
        if (objName == name)
        {
            return m_childs[i];
        }
        GameObject *obj = m_childs[i]->findFirstChildRecursive(name);
        if(obj)
            return obj;
    }
    return nullptr;
}
std::vector<GameObject*> GameObject::findAllChildsRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<GameObject*> results;
    results.reserve(m_childs.size()*2);
    findAllChildsRecursive_internal(name, results);
    return results;
}

void GameObject::addComponent(Component *comp)
{
    if(!comp)return;
    m_toAddComponents.push_back(comp);
    onObjectsChanged();
}
void GameObject::addComponents(const std::vector<Components::Component*>& components)
{
    for (size_t i = 0; i < components.size(); ++i)
    {
        Components::Component* comp = components[i];
        if (comp)
            m_toAddComponents.push_back(comp);
    }
    onObjectsChanged();
}
void GameObject::removeComponent(Component *comp)
{
    if(!comp)return;
    for(size_t i=0; i<m_toRemoveComponents.size(); ++i)
        if(m_toRemoveComponents[i] == comp)
            return;
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
        if(m_toAddComponents[i] == comp)
        {
            m_toAddComponents.erase(m_toAddComponents.begin() + i);
            break;
        }
    m_toRemoveComponents.push_back(comp);
    onObjectsChanged();
}
void GameObject::removeComponents(const std::vector<Components::Component*>& components)
{
    for (size_t j = 0; j < components.size(); ++j)
    {
        Components::Component* comp = components[j];
        for (size_t i = 0; i < m_toRemoveComponents.size(); ++i)
            if (m_toRemoveComponents[i] == comp)
                return;
        for (size_t i = 0; i < m_toAddComponents.size(); ++i)
            if (m_toAddComponents[i] == comp)
            {
                m_toAddComponents.erase(m_toAddComponents.begin() + i);
                break;
            }
        m_toRemoveComponents.push_back(comp);
    }
    onObjectsChanged();
}
void GameObject::removeComponent_internal()
{
    size_t removedCount = 0;
    for(size_t i=0; i<m_toRemoveComponents.size(); ++i)
    {
        Components::Component *toRemove = m_toRemoveComponents[i];
        size_t index = getComponentIndex(toRemove);
        if (index == npos)
            continue;
        m_components.erase(m_components.begin() + index);
        ++removedCount;

        // Check for sfEventHandles
        SfEventHandle *evComp = dynamic_cast<SfEventHandle*>(toRemove);
        if(evComp)
        {
            for(size_t j=0; j<m_eventComponents.size(); ++j)
                if(m_eventComponents[j] == evComp)
                {
                    m_eventComponents.erase(m_eventComponents.begin() + j);
                    break;
                }
            if(m_eventComponents.size() == 0)
            {
                needsEventUpdate(false);
            }
        }

        // Check for Drawables
        Drawable *drawComp = dynamic_cast<Drawable*>(toRemove);
        if(drawComp)
        {
            for(size_t j=0; j<m_drawableComponents.size(); ++j)
                if(m_drawableComponents[j] == drawComp)
                {
                    m_drawableComponents.erase(m_drawableComponents.begin() + j);
                    break;
                }
            if(m_drawableComponents.size() == 0)
            {
                needsDrawUpdate(false);
            }
        }

        // Check for Updatables
        Utilities::Updatable *updatable = dynamic_cast<Utilities::Updatable*>(toRemove);
        if(updatable)
        {
            for(size_t j=0; j<m_updatableComponents.size(); ++j)
                if(m_updatableComponents[j] == updatable)
                {
                    m_updatableComponents.erase(m_updatableComponents.begin() + j);
                    break;
                }
        }

        /*// Check for transformables
		Utilities::Transformable* transformable = dynamic_cast<Utilities::Transformable*>(toRemove);
        if (transformable)
        {
            for (size_t j = 0; j < m_transformables.size(); ++j)
                if (m_transformables[j] == transformable)
                {
                    m_transformables.erase(m_transformables.begin() + j);
                    break;
                }
        }*/
    }
    m_toRemoveComponents.clear();
    if(m_SceneParent)
        m_SceneParent->removeComponent(removedCount);

}
void GameObject::deleteComponent(Component *comp)
{
    if(!comp)return;


    for(size_t i=0; i<m_toAddComponents.size(); ++i)
        if(m_toAddComponents[i] == comp)
        {
            m_toAddComponents.erase(m_toAddComponents.begin() + i);
            break;
        }
    comp->setParent(nullptr);
    comp->setSceneParent(nullptr);

    size_t index = getComponentIndex(comp);
    if(index == npos) return;
    m_components.erase(m_components.begin() + index);
    if (m_SceneParent)
        m_SceneParent->removeComponent();

    // Check for sfEventHandles
    SfEventHandle *evComp = dynamic_cast<SfEventHandle*>(comp);
    if(evComp)
    {
        for(size_t j=0; j<m_eventComponents.size(); ++j)
            if(m_eventComponents[j] == evComp)
            {
                m_eventComponents.erase(m_eventComponents.begin() + j);
                break;
            }
        if(m_eventComponents.size() == 0)
        {
            needsEventUpdate(false);
        }
    }

    // Check for Drawables
    Drawable *drawComp = dynamic_cast<Drawable*>(comp);
    if(drawComp)
    {
        for(size_t j=0; j<m_drawableComponents.size(); ++j)
            if(m_drawableComponents[j] == drawComp)
            {
                m_drawableComponents.erase(m_drawableComponents.begin() + j);
                break;
            }
        if(m_drawableComponents.size() == 0)
        {
            needsDrawUpdate(false);
        }
    }

    // Check for Updatables
    Utilities::Updatable *updatable = dynamic_cast<Utilities::Updatable*>(comp);
    if(updatable)
    {
        for(size_t j=0; j<m_updatableComponents.size(); ++j)
            if(m_updatableComponents[j] == updatable)
            {
                m_updatableComponents.erase(m_updatableComponents.begin() + j);
                break;
            }
    }

    // Check for transformable

    delete comp;
}
void GameObject::deleteComponents(const std::vector<Components::Component*>& components)
{
    for (size_t j = 0; j < components.size(); ++j)
    {
        Components::Component* comp = components[j];
        deleteComponent(comp);
    }
}
void GameObject::addChild_internal()
{
    for(size_t i=0; i<m_toAddChilds.size(); ++i)
    {
        if(childExists(m_toAddChilds[i]))
            continue;
        addChild_internal(m_toAddChilds[i]);
    }
    m_toAddChilds.clear();
}
void GameObject::addChild_internal(GameObject *obj)
{
    m_childs.push_back(obj);
    obj->setParent_internal(this, m_rootParent, m_SceneParent);
}
void GameObject::setParent_internal(GameObject *parent,
                                      GameObject *rootParent,
                                      Scene *SceneParent)
{
    if(m_parent != nullptr && m_parent == this)
        return;
    if(m_parent)
    {
        size_t index = m_parent->getChildIndex(this);
        if(index == npos) return;
        m_parent->m_childs.erase(m_parent->m_childs.begin() + index);
    }
    GameObject *oldParent = m_parent;
    m_parent = parent;
    m_rootParent = rootParent;
    setSceneParent(SceneParent);
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setParent(this);
    }

    internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
void GameObject::addComponent_internal()
{
    size_t colliderCount = m_colliders.size();
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Components::Component *toAdd = m_toAddComponents[i];
        if(componentExists(toAdd))
            continue;

        toAdd->setParent(this);
        toAdd->setSceneParent(m_SceneParent);
        if (m_SceneParent)
            m_SceneParent->addComponent();
        m_components.push_back(toAdd);

        // Check for sfEventHandles
        SfEventHandle* eventComp = dynamic_cast<SfEventHandle*>(toAdd);
        if(eventComp)
        {
            m_eventComponents.push_back(eventComp);
            if(m_eventComponents.size() && !m_thisNeedsEventUpdate)
            {
                needsEventUpdate(true);
            }
        }

        // Check for Drawables
        Drawable* drawComp = dynamic_cast<Drawable*>(toAdd);
        if(drawComp)
        {
            m_drawableComponents.push_back(drawComp);
            if(m_drawableComponents.size() && !m_thisNeedsDrawUpdate)
            {
                needsDrawUpdate(true);
            }
        }

        // Check for Updatables
        Utilities::Updatable *updatable = dynamic_cast<Utilities::Updatable*>(toAdd);
        if(updatable)
        {
            m_updatableComponents.push_back(updatable);
        }

        // Check for Updatables
        Collider* collider = dynamic_cast<Collider*>(toAdd);
        if (collider)
        {
            m_colliders.push_back(collider);
            collider->setPos(getPosition());
        }
    }
    m_toAddComponents.clear();
    if (m_colliders.size() != colliderCount)
        updateBoundingBox();
}
void GameObject::onObjectsChanged()
{
    m_objectsChanged = true;
    if(m_parent)
    {
        m_parent->onObjectsChanged();
    }
}
void GameObject::deleteComponents()
{
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Component* comp = m_toAddComponents[i];
        comp->setParent(nullptr);
        comp->setSceneParent(nullptr);
        delete comp;
    }
    if (m_SceneParent)
        m_SceneParent->removeComponent(m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setParent(nullptr);
        comp->setSceneParent(nullptr);
        delete comp;
    }
    m_updatableComponents.clear();
    m_eventComponents.clear();    
    m_drawableComponents.clear();
    m_toAddComponents.clear();
    m_components.clear();
}
bool GameObject::componentExists(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return true;
    return false;
}
size_t GameObject::getComponentIndex(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return i;
    return npos;
}
const std::vector<Component*> &GameObject::getComponents() const
{
    return m_components;
}

Components::Component* GameObject::findFirstComponent(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t nameSize = name.size();
    for (const auto &comp : m_components)
    {
		const std::string& objName = comp->getName();
		if (nameSize != objName.size())
			continue;
        if (objName == name)
			return comp;
	}
	return nullptr;
}
std::vector<Components::Component*> GameObject::findAllComponents(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
	std::vector<Components::Component*> results;
	results.reserve(m_components.size());
    size_t nameSize = name.size();
    for (const auto& comp : m_components)
    {
		const std::string& objName = comp->getName();
        if (nameSize != objName.size())
            continue;
		if (objName == name)
			results.push_back(comp);
	}
	return results;
}

Components::Component* GameObject::findFirstComponentRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    size_t nameSize = name.size();
    for (const auto& comp : m_components)
    {
        const std::string& objName = comp->getName();
        if (nameSize != objName.size())
            continue;
        if (objName == name)
            return comp;
    }
    for (const auto &obj : m_childs)
    {
		Components::Component* comp = obj->findFirstComponentRecursive(name);
		if (comp)
			return comp;
	}
    return nullptr;
}
std::vector<Components::Component*> GameObject::findAllComponentsRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<Components::Component*> results;
    results.reserve(m_components.size());
    findAllComponentsRecursive_internal(name, results);
    return results;
}
bool GameObject::findAllComponentsRecursive_internal(const std::string& name, std::vector<Components::Component*>& foundList)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    bool found = false;
    size_t nameSize = name.size();
    for (const auto& comp : m_components)
    {
        const std::string& objName = comp->getName();
        if (nameSize != objName.size())
            continue;
        if (objName == name)
            foundList.push_back(comp);
    }
    for (const auto& obj : m_childs)
    {
		found |= obj->findAllComponentsRecursive_internal(name, foundList);
	}
    return found;
}


const std::vector<Components::Collider*> &GameObject::getCollider() const
{
    return m_colliders;
}
bool GameObject::checkCollision(const GameObject* other) const
{
    std::vector<Utilities::Collisioninfo> collisions;
    return checkCollision(other, collisions);
}
bool GameObject::checkCollision(const GameObject* other, 
    std::vector<Utilities::Collisioninfo>& collisions,
    bool onlyFirstCollision) const
{
    // Check if bounding box intersects
    const Utilities::AABB &otherBox = other->getBoundingBox();
    if (!m_boundingBox.intersects(otherBox))
		return false;

    // Check for collisions
    const std::vector<Components::Collider*> &otherColliders = other->getCollider();
    bool hasCollision = false;
    for (auto thisCollider : m_colliders)
    {
        hasCollision |= thisCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
    }
    return hasCollision;
}
void GameObject::checkCollision(const Utilities::ObjectQuadTree& tree, 
                                  std::vector<Utilities::Collisioninfo>& collisions,
                                  bool onlyFirstCollision)
{
    std::list<Utilities::ObjectQuadTree::TreeItem> objs = tree.getAllItems();
    for (auto &objStruct : objs)
    {
        GameObject* obj = objStruct.obj;
        std::list< QSFML::Objects::GameObject*> possibleColliders;
        tree.search(obj->getBoundingBox(), possibleColliders);
        for (auto it : possibleColliders)
        {
            if (obj == it)
                continue;

            const std::vector<Components::Collider*> &otherColliders = it->getCollider();
            for (auto objCollider : obj->m_colliders)
            {
                objCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
            }            
        }
    }
}
/*void GameObject::solveCollision(GameObject* other)
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        m_colliders[i]->resolveCollision(other->getCollider());
    }
}*/


size_t GameObject::getComponentCount() const
{
    return m_components.size();
}

sf::Vector2i GameObject::getMousePosition() const
{
    if (!m_SceneParent) return sf::Vector2i(0, 0);
    return m_SceneParent->getMousePosition();
}
sf::Vector2f GameObject::getMouseWorldPosition() const
{
    if (!m_SceneParent) return sf::Vector2f(0, 0);
    return m_SceneParent->getMouseWorldPosition();
}
sf::Vector2f GameObject::getMouseObjectPosition() const
{
    return getMouseWorldPosition() - getGlobalPosition();
}

sf::Vector2f GameObject::getInWorldSpace(const sf::Vector2i& pixelSpace) const
{
    if (!m_SceneParent) return sf::Vector2f(0, 0);
    return m_SceneParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i GameObject::getInScreenSpace(const sf::Vector2f& worldSpace) const
{
    if (!m_SceneParent) return sf::Vector2i(0, 0);
    return m_SceneParent->getInScreenSpace(worldSpace);
}
const sf::View GameObject::getCameraView() const
{
    static const sf::View dummy;
    if(!m_SceneParent) return dummy;
    return m_SceneParent->getCameraView();
}
const sf::View &GameObject::getDefaultCameraView() const
{
    static const sf::View dummy;
    if(!m_SceneParent) return dummy;
    return m_SceneParent->getDefaultCameraView();
}
Utilities::AABB GameObject::getCameraViewRect() const
{
    if (!m_SceneParent) return Utilities::AABB();
    return m_SceneParent->getCameraViewRect();
}
sf::Vector2u GameObject::getSceneSize() const
{
    if(!m_SceneParent) return sf::Vector2u(0,0);
    return m_SceneParent->getSceneSize();
}
sf::Vector2u GameObject::getOldSceneSize() const
{
    if(!m_SceneParent) return sf::Vector2u(0,0);
    return m_SceneParent->getOldSceneSize();
}

const sf::Font &GameObject::getDefaultTextFont() const
{
    return Scene::getDefaultTextFont();
}
size_t GameObject::getTick() const
{
    if(!m_SceneParent) return 0;
    return m_SceneParent->getTick();
}
double GameObject::getDeltaT() const
{
    if(!m_SceneParent) return 0;
    return m_SceneParent->getDeltaT();
}
double GameObject::getFixedDeltaT() const
{
    if (!m_SceneParent) return 0;
    return m_SceneParent->getFixedDeltaT();
}
double GameObject::getElapsedTime() const
{
    if(!m_SceneParent) return 0;
    return m_SceneParent->getElapsedTime();
}
double GameObject::getFixedElapsedTime() const
{
    if (!m_SceneParent) return 0;
	return m_SceneParent->getFixedElapsedTime();
}


const SceneSettings::UpdateControlls &GameObject::getUpdateControlls() const
{
    return m_updateControlls;
}
void GameObject::setUpdateControlls(const SceneSettings::UpdateControlls &controlls)
{
    m_updateControlls = controlls;
}

const Utilities::AABB &GameObject::getBoundingBox() const
{
    return m_boundingBox;
}
void GameObject::updateBoundingBox()
{
    std::vector<Utilities::AABB> boxes;
    boxes.reserve(m_colliders.size());
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        boxes.push_back(m_colliders[i]->getBoundingBox());
    }
    m_boundingBox = Utilities::AABB::getFrame(boxes);
}


std::string GameObject::toString() const
{
    using std::string;
    std::vector<string> lines = toStringInternal("");
    string msg;
    for(size_t i=0; i<lines.size(); ++i)
    {
        msg += lines[i]+"\n";
    }

    return msg;
}
Scene* GameObject::getSceneParent() const
{
    return m_SceneParent;
}

Objects::GameObject* GameObject::findFirstObjectGlobal(const std::string& name)
{
    if (!m_SceneParent) return nullptr;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_SceneParent->findFirstObject(name);
}
std::vector<Objects::GameObject*> GameObject::findAllObjectsGlobal(const std::string& name)
{
    if (!m_SceneParent) return std::vector<GameObject*>();
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_SceneParent->findAllObjects(name);
}
Objects::GameObject* GameObject::findFirstObjectGlobalRecursive(const std::string& name)
{
    if (!m_SceneParent) return nullptr;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_SceneParent->findFirstObjectRecursive(name);
}
std::vector<Objects::GameObject*> GameObject::findAllObjectsGlobalRecusive(const std::string& name)
{
    if (!m_SceneParent) return std::vector<GameObject*>();
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
	return m_SceneParent->findAllObjectsRecursive(name);
}

void GameObject::update()
{

}
std::vector<std::string> GameObject::toStringInternal(const std::string &preStr) const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    using std::string;
    std::vector<string> lines;
    const GameObject *t = this;

    auto en = [](bool enabled) {
        return enabled?"[Enabled]":"[Disabled]";
      };

    const std::type_info &type = typeid(*t);
    std::string name = type.name();
    lines.push_back(preStr+" | "+string(typeid(*t).name())+": \""+m_name + "\" "+en(t->isEnabled()));
    lines.push_back(preStr+" |  Components:");
    for(size_t i=0; i<m_components.size(); ++i)
    {
        const auto &comp = m_components[i];
        lines.push_back(preStr+  " |  ["+std::to_string(i)+"] " + 
            string(typeid(*comp).name()) + ": \"" + comp->getName() + "\" " + en(comp->isEnabled()));
    }
    lines.push_back(preStr+" |  Childs:");
    for(size_t i=0; i<m_childs.size(); ++i)
    {

        std::vector<std::string> subList = m_childs[i]->toStringInternal(preStr+ " |  ");
        lines.insert(lines.end(), subList.begin(), subList.end());
    }
    lines.push_back(preStr+" ----");
    return lines;
}
bool GameObject::findAllChilds_internal(const std::string& name, std::vector<GameObject*>& foundList)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
    size_t nameSize = name.size();
    bool found = false;
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        const std::string &objName = m_childs[i]->getName();
        if(nameSize != objName.size())
			continue;
        if (objName == name)
        {
			foundList.push_back(m_childs[i]);
            found = true;
		}
		//found |= m_childs[i]->findAllChilds_internal(name, foundList);
	}
    return found;
}
bool GameObject::findAllChildsRecursive_internal(const std::string& name, std::vector<GameObject*>& foundList)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_2);
    size_t nameSize = name.size();
    bool found = false;
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        const std::string& objName = m_childs[i]->getName();
        if (nameSize != objName.size())
            continue;
        if (objName == name)
        {
            foundList.push_back(m_childs[i]);
            found = true;
        }
        found |= m_childs[i]->findAllChilds_internal(name, foundList);
    }
    return found;
}

void GameObject::onSceneParentChange(Scene *oldParent, Scene *newParent) 
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::onParentChange(GameObject *oldParent, GameObject *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::internalOnSceneParentChange(Scene *oldParent, Scene *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::internalOnParentChange(GameObject *oldParent, GameObject *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}


void GameObject::deleteThis()
{
    if(m_parent)
    {
       // m_parent->deleteChild(this);
        m_parent->removeChild(this);
    }
    else if(m_SceneParent)
        m_SceneParent->GameObjectContainer::deleteLater(this);

}

//void GameObject::positionChanged(const sf::Vector2f& oldPosition, const sf::Vector2f& newPosition)
//{
//    QSFML_UNUSED(oldPosition);
//    QSFML_UNUSED(newPosition);
//    sf::Vector2f globalPos = getGlobalPosition();
//    for (auto &collider : m_colliders)
//        collider->setPos(globalPos);
//}




void GameObject::needsEventUpdateChanged(bool needsEventUpdate)
{
    if(!m_thisNeedsEventUpdate && needsEventUpdate)
    {
        this->needsEventUpdate(needsEventUpdate);
        return;
    }
    if(m_thisNeedsEventUpdate && !needsEventUpdate)
    {
        if(m_eventComponents.size() == 0)
        {
            bool needsUpdate = false;
            for(size_t i=0; i<m_childs.size(); ++i)
            {
                if(m_childs[i]->m_thisNeedsEventUpdate)
                {
                    needsUpdate = true;
                    break;
                }
            }
            if(!needsUpdate)
            {
                this->needsEventUpdate(needsEventUpdate);
                return;
            }
        }
    }
}
void GameObject::needsEventUpdate(bool needsEventUpdate)
{
    if(m_thisNeedsEventUpdate == needsEventUpdate)
        return;
    m_thisNeedsEventUpdate = needsEventUpdate;
    if(m_parent)
        m_parent->needsEventUpdateChanged(m_thisNeedsEventUpdate);
}

void GameObject::needsDrawUpdateChanged(bool needsDrawUpdate)
{
    if(!m_thisNeedsDrawUpdate && needsDrawUpdate)
    {
        this->needsDrawUpdate(needsDrawUpdate);
        return;
    }
    if(m_thisNeedsDrawUpdate && !needsDrawUpdate)
    {
        if(m_drawableComponents.size() == 0)
        {
            bool needsUpdate = false;
            for(size_t i=0; i<m_childs.size(); ++i)
            {
                if(m_childs[i]->m_thisNeedsDrawUpdate)
                {
                    needsUpdate = true;
                    break;
                }
            }
            if(!needsUpdate)
            {
                this->needsDrawUpdate(needsDrawUpdate);
                return;
            }
        }
    }
}
void GameObject::needsDrawUpdate(bool needsDrawUpdate)
{
    if(m_thisNeedsDrawUpdate == needsDrawUpdate)
        return;
    m_thisNeedsDrawUpdate = needsDrawUpdate;
    if(m_parent)
        m_parent->needsDrawUpdateChanged(m_thisNeedsDrawUpdate);
}

void GameObject::setSceneParent(Scene *parent)
{
    if(m_SceneParent == parent)
        return;
    Scene *oldParent = m_SceneParent;
    m_SceneParent = parent;
    if (oldParent != nullptr)
    {
        oldParent->removeGameObject();
        oldParent->removeComponent(m_components.size());
    }
    if (m_SceneParent != nullptr)
    {
        m_SceneParent->addCanvesObject();
        m_SceneParent->addComponent(m_components.size());

        // Set the birth time and tick
        m_birthTick = m_SceneParent->getTick();
        m_birthTime = m_SceneParent->getElapsedTime();
    }

    for (size_t i = 0; i < m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setSceneParent(m_SceneParent);
    }

    //for(size_t i=0; i<m_components.size(); ++i)
    //    m_components[i]->setParent(this);

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->setSceneParent(parent);

    internalOnSceneParentChange(oldParent, m_SceneParent);
    onSceneParentChange(oldParent, m_SceneParent);
}

void GameObject::updateNewElements()
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    removeChild_internal();
    deleteChild_internal();
    removeComponent_internal();
    addChild_internal();
    addComponent_internal();

    for (size_t i = 0; i < m_childs.size(); ++i)
        m_childs[i]->updateNewElements();
    m_objectsChanged = false;
}
void GameObject::sfEvent(const std::vector<sf::Event>& events)
{
    if (!m_enabled || !m_updateControlls.enableEventLoop || !m_thisNeedsEventUpdate) return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_BLOCK("Components event", QSFML_COLOR_STAGE_2);
    for (auto component : m_eventComponents)
    {
        if (!component->isEnabled())
            continue;

        for (const auto &event : events)
            component->sfEvent(event);

    }
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs event", QSFML_COLOR_STAGE_3);
    for (auto obj : m_childs)
    {
        if (obj->m_enabled)
            obj->sfEvent(events);
    }
    QSFMLP_OBJECT_END_BLOCK;
}
void GameObject::update_internal()
{
    if (!m_enabled || !m_updateControlls.enableUpdateLoop) return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_BLOCK("Object update", QSFML_COLOR_STAGE_2);
    Updatable::emitUpdate();
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Components update", QSFML_COLOR_STAGE_2);
    for (size_t i = 0; i < m_updatableComponents.size(); ++i)
    {
        Utilities::Updatable* comp = m_updatableComponents[i];
        Components::Component* comp1 = dynamic_cast<Components::Component*>(comp);
        if (!comp1->isEnabled())
            continue;
        comp->emitUpdate();
    }
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs update", QSFML_COLOR_STAGE_4);
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        GameObject* obj = m_childs[i];
        if (obj->m_enabled)
            obj->update_internal();
    }
    QSFMLP_OBJECT_END_BLOCK;
}
void GameObject::inSceneAdded_internal()
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_BLOCK("Object inSceneAdded", QSFML_COLOR_STAGE_2);
    inSceneAdded();
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs inSceneAdded", QSFML_COLOR_STAGE_4);
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        m_childs[i]->inSceneAdded_internal();
    }
    QSFMLP_OBJECT_END_BLOCK;
}
void GameObject::draw(sf::RenderWindow& window, sf::RenderStates states) const
{
    if (!m_enabled || !m_updateControlls.enablePaintLoop || !m_thisNeedsDrawUpdate)
        return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    states.transform.translate(getPosition());
    if (m_drawableComponents.size())
    {
        QSFMLP_OBJECT_BLOCK("Components draw", QSFML_COLOR_STAGE_2);
        for (auto& comp : m_drawableComponents)
        {
            if (!comp->isEnabled())
                continue;
            window.draw(*comp, states);
        }
        QSFMLP_OBJECT_END_BLOCK;
    }

    if (m_childs.size())
    {
        QSFMLP_OBJECT_BLOCK("Childs draw", QSFML_COLOR_STAGE_3);
        for (auto& child : m_childs)
        {
            if (child->m_enabled)
                child->draw(window, states);
        }
        QSFMLP_OBJECT_END_BLOCK;
    }
}



