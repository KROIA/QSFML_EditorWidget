#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

#include "components/base/Component.h"
#include "components/base/SfEventHandle.h"
#include "components/base/Drawable.h"
#include "components/physics/Collider.h"

#include "utilities/Stats.h"


#include <QDebug>
#include <typeinfo>
    


using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

OBJECT_IMPL(GameObject)

size_t GameObject::s_objNameCounter = 0;
GameObject::GameObject(const std::string &name, GameObject* parent)
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
   
    m_sceneParent = nullptr;
    m_parent = parent;
    m_rootParent = this;
    if(m_parent)
        m_rootParent = m_parent->m_rootParent;
    //m_objectsChanged = false;
    m_componentsManagerData.thisNeedsDrawUpdate = false;
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
    m_sceneParent = nullptr;
    m_parent = nullptr;
    m_rootParent = this;
    //m_objectsChanged = false;
    m_componentsManagerData.thisNeedsDrawUpdate = false;
    m_updateControlls = other.m_updateControlls;
    m_renderLayer = other.m_renderLayer;

    //m_childs.reserve(other.m_childs.size());
    for(size_t i=0; i<other.m_childObjectManagerData.objs.size(); ++i)
    {
        GameObjectPtr obj = other.m_childObjectManagerData.objs[i]->clone();
        addChild(obj);
    }
    for(size_t i=0; i<other.m_componentsManagerData.all.size(); ++i)
    {
        Component *obj = other.m_componentsManagerData.all[i]->clone();
        addComponent(obj);
    }
    updateNewElements();
}
GameObject::~GameObject()
{
    if (m_sceneParent)
    {
        m_sceneParent->removeGameObject();
        m_sceneParent->removeComponent(m_componentsManagerData.all.size());
        m_sceneParent->removeObject(this);
    }

    for(size_t i=0; i< m_childObjectManagerData.objs.size(); ++i)
        delete m_childObjectManagerData.objs[i];
    m_childObjectManagerData.objs.clear();

    
    for(auto &comp : m_componentsManagerData.all)
        delete comp;
    m_componentsManagerData.all.clear();
    m_componentsManagerData.toAdd.clear();
    m_componentsManagerData.updatables.clear();
    m_componentsManagerData.drawable.clear();
	//m_transformables.clear();
    m_childObjectManagerData.toRemove.clear();
    m_childObjectManagerData.toAdd.clear();
    delete m_selfOwnedLogObject;
}


void GameObject::inSceneAdded()
{

}
void GameObject::setParent(GameObjectPtr parent)
{
    if(parent == m_parent)
        return;
    if(m_parent)
    {
        m_parent->removeChild(this);
    }
    if(parent && parent != this)
    {
        parent->addChild(this);
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
GameObjectPtr GameObject::getParent() const
{
    return m_parent;
}
GameObjectPtr GameObject::getRootParent() const
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
    if (!m_sceneParent) return 0;
    return m_sceneParent->getElapsedTime() - m_birthTime;
}
double GameObject::getBirthTime() const
{
    return m_birthTime;
}
size_t GameObject::getAgeTicks() const
{
    if (!m_sceneParent) return 0;
    return m_sceneParent->getTick() - m_birthTick;
}
size_t GameObject::getBirthTick() const
{
    return m_birthTick;
}
double GameObject::getAgeFixed() const
{
    if (!m_sceneParent) return 0;
    return m_sceneParent->getFixedDeltaT() * (m_sceneParent->getTick() - m_birthTick);
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
    GameObjectPtr parent = m_parent;
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
	GameObjectPtr parent = m_parent;
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
    if(m_sceneParent)
        m_sceneParent->renderLayerSwitch(this, oldLayer, m_renderLayer);
}

RenderLayer GameObject::getRenderLayer() const
{
    return m_renderLayer;
}

/*
// Childs operations
void GameObject::addChild(GameObjectPtr child)
{
    if(!child)return;

    m_toAddChilds.push_back(child);
    onObjectsChanged();

}
void GameObject::addChilds(const std::vector<GameObjectPtr>& childs)
{
    for (size_t i = 0; i < childs.size(); ++i)
    {
        GameObjectPtr child = childs[i];
        if(child)
            m_toAddChilds.push_back(child);
    }
    onObjectsChanged();
}

void GameObject::removeChild(GameObjectPtr child)
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
void GameObject::removeChilds(const std::vector<GameObjectPtr>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        GameObjectPtr child = childs[j];
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

void GameObject::deleteChild(GameObjectPtr child)
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
void GameObject::deleteChilds(const std::vector<GameObjectPtr>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        GameObjectPtr child = childs[j];
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
bool GameObject::childExists(GameObjectPtr child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return true;
    return false;
}
size_t GameObject::getChildIndex(GameObjectPtr child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return i;
    return npos;
}
const std::vector<GameObjectPtr> &GameObject::getChilds() const
{
    return m_childs;
}
size_t GameObject::getChildCount() const
{
    return m_childs.size();
}*/

/*
GameObjectPtr GameObject::findFirstChild(const std::string& name)
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
std::vector<GameObjectPtr> GameObject::findAllChilds(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<GameObjectPtr> results;
    results.reserve(m_childs.size());
    findAllChilds_internal(name, results);
    return results;
}

GameObjectPtr GameObject::findFirstChildRecursive(const std::string& name)
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
        GameObjectPtr obj = m_childs[i]->findFirstChildRecursive(name);
        if(obj)
            return obj;
    }
    return nullptr;
}
std::vector<GameObjectPtr> GameObject::findAllChildsRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<GameObjectPtr> results;
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
}*/
/*
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

        
    }
    m_toRemoveComponents.clear();
    if(m_sceneParent)
        m_sceneParent->removeComponent(removedCount);

}*/
/*
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
    if (m_sceneParent)
        m_sceneParent->removeComponent();

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
*/
/*
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
void GameObject::addChild_internal(GameObjectPtr obj)
{
    m_childs.push_back(obj);
    obj->setParent_internal(this, m_rootParent, m_sceneParent);
}*/
void GameObject::setParent_internal(GameObjectPtr parent,
                                      GameObjectPtr rootParent,
                                      Scene *SceneParent)
{
    if(m_parent != nullptr && m_parent == this)
        return;
    if(m_parent)
    {
        size_t index = m_parent->getChildIndex(this);
        if(index == npos) return;
        m_parent->m_childObjectManagerData.objs.erase(m_parent->m_childObjectManagerData.objs.begin() + index);
    }
    GameObjectPtr oldParent = m_parent;
    m_parent = parent;
    m_rootParent = rootParent;
    setSceneParent(SceneParent);
    for (size_t i = 0; i < m_componentsManagerData.all.size(); ++i)
    {
        Component* comp = m_componentsManagerData.all[i];
        comp->setParent(this);
    }

    internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
/*
void GameObject::addComponent_internal()
{
    size_t colliderCount = m_colliders.size();
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Components::Component *toAdd = m_toAddComponents[i];
        if(componentExists(toAdd))
            continue;

        toAdd->setParent(this);
        toAdd->setSceneParent(m_sceneParent);
        if (m_sceneParent)
            m_sceneParent->addComponent();
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
}*/

/*/
void GameObject::deleteComponents()
{
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Component* comp = m_toAddComponents[i];
        comp->setParent(nullptr);
        comp->setSceneParent(nullptr);
        delete comp;
    }
    if (m_sceneParent)
        m_sceneParent->removeComponent(m_components.size());
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
*/

/*
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
}*/


/*
bool GameObject::checkCollision(const GameObjectPtr other) const
{
    std::vector<Utilities::Collisioninfo> collisions;
    return checkCollision(other, collisions);
}
bool GameObject::checkCollision(const GameObjectPtr other, 
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
        GameObjectPtr obj = objStruct.obj;
        std::list< QSFML::Objects::GameObjectPtr> possibleColliders;
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
}*/


/*void GameObject::solveCollision(GameObjectPtr other)
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        m_colliders[i]->resolveCollision(other->getCollider());
    }
}*/




sf::Vector2i GameObject::getMousePosition() const
{
    if (!m_sceneParent) return sf::Vector2i(0, 0);
    return m_sceneParent->getMousePosition();
}
sf::Vector2f GameObject::getMouseWorldPosition() const
{
    if (!m_sceneParent) return sf::Vector2f(0, 0);
    return m_sceneParent->getMouseWorldPosition();
}
sf::Vector2f GameObject::getMouseObjectPosition() const
{
    return getMouseWorldPosition() - getGlobalPosition();
}

sf::Vector2f GameObject::getInWorldSpace(const sf::Vector2i& pixelSpace) const
{
    if (!m_sceneParent) return sf::Vector2f(0, 0);
    return m_sceneParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i GameObject::getInScreenSpace(const sf::Vector2f& worldSpace) const
{
    if (!m_sceneParent) return sf::Vector2i(0, 0);
    return m_sceneParent->getInScreenSpace(worldSpace);
}
const sf::View GameObject::getCameraView() const
{
    static const sf::View dummy;
    if(!m_sceneParent) return dummy;
    return m_sceneParent->getCameraView();
}
const sf::View &GameObject::getDefaultCameraView() const
{
    static const sf::View dummy;
    if(!m_sceneParent) return dummy;
    return m_sceneParent->getDefaultCameraView();
}
Utilities::AABB GameObject::getCameraViewRect() const
{
    if (!m_sceneParent) return Utilities::AABB();
    return m_sceneParent->getCameraViewRect();
}
sf::Vector2u GameObject::getSceneSize() const
{
    if(!m_sceneParent) return sf::Vector2u(0,0);
    return m_sceneParent->getSceneSize();
}
sf::Vector2u GameObject::getOldSceneSize() const
{
    if(!m_sceneParent) return sf::Vector2u(0,0);
    return m_sceneParent->getOldSceneSize();
}

const sf::Font &GameObject::getDefaultTextFont() const
{
    return Scene::getDefaultTextFont();
}
size_t GameObject::getTick() const
{
    if(!m_sceneParent) return 0;
    return m_sceneParent->getTick();
}
double GameObject::getDeltaT() const
{
    if(!m_sceneParent) return 0;
    return m_sceneParent->getDeltaT();
}
double GameObject::getFixedDeltaT() const
{
    if (!m_sceneParent) return 0;
    return m_sceneParent->getFixedDeltaT();
}
double GameObject::getElapsedTime() const
{
    if(!m_sceneParent) return 0;
    return m_sceneParent->getElapsedTime();
}
double GameObject::getFixedElapsedTime() const
{
    if (!m_sceneParent) return 0;
	return m_sceneParent->getFixedElapsedTime();
}


const SceneSettings::UpdateControlls &GameObject::getUpdateControlls() const
{
    return m_updateControlls;
}
void GameObject::setUpdateControlls(const SceneSettings::UpdateControlls &controlls)
{
    m_updateControlls = controlls;
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
    return m_sceneParent;
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


void GameObject::createLogger()
{
    if (!m_selfOwnedLogObject)
    {
        m_selfOwnedLogObject = new Log::LogObject(getName());
    }
    // Search nearest parent with self owned logger to set it as parent
    GameObjectPtr parent = m_parent;
    bool parentSet = false;
    while (parent)
	{
		if (parent->m_selfOwnedLogObject)
		{
            m_selfOwnedLogObject->setParentID(parent->m_selfOwnedLogObject->getID());
            parentSet = true;
			break;
		}
		parent = parent->m_parent;
	}
    if (!parentSet && m_sceneParent)
    {
        m_selfOwnedLogObject->setParentID(m_sceneParent->getObjectLogger().getID());
    }
    m_logObject = m_selfOwnedLogObject;
}

void GameObject::log(const Log::Message& msg) const
{
    if (m_logObject)
        m_logObject->log(msg);
}

void GameObject::log(const std::string& msg) const
{
    if (m_logObject)
        m_logObject->log(msg);
}
void GameObject::log(const std::string& msg, Log::Level level) const
{
    if (m_logObject)
        m_logObject->log(msg, level);
}
void GameObject::log(const std::string& msg, Log::Level level, const Log::Color& col) const
{
    if (m_logObject)
        m_logObject->log(msg, level, col);
}

void GameObject::logDebug(const std::string& msg) const
{
    if (m_logObject)
        m_logObject->logDebug(msg);
}
void GameObject::logInfo(const std::string& msg) const
{
    if (m_logObject)
        m_logObject->logInfo(msg);
}
void GameObject::logWarning(const std::string& msg) const
{
    if (m_logObject)
        m_logObject->logWarning(msg);
}
void GameObject::logError(const std::string& msg) const
{
    if (m_logObject)
        m_logObject->logError(msg);
}



void GameObject::update()
{

}
std::vector<std::string> GameObject::toStringInternal(const std::string &preStr) const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    using std::string;
    std::vector<string> lines;
    const GameObject* t = this;

    auto en = [](bool enabled) {
        return enabled?"[Enabled]":"[Disabled]";
      };

    const std::type_info &type = typeid(*t);
    std::string name = type.name();
    lines.push_back(preStr+" | "+string(typeid(*t).name())+": \""+m_name + "\" "+en(t->isEnabled()));
    lines.push_back(preStr+" |  Components:");
    for(size_t i=0; i< m_componentsManagerData.all.size(); ++i)
    {
        const auto &comp = m_componentsManagerData.all[i];
        lines.push_back(preStr+  " |  ["+std::to_string(i)+"] " + 
            string(typeid(*comp).name()) + ": \"" + comp->getName() + "\" " + en(comp->isEnabled()));
    }
    lines.push_back(preStr+" |  Childs:");
    for(size_t i=0; i< m_childObjectManagerData.objs.size(); ++i)
    {

        std::vector<std::string> subList = m_childObjectManagerData.objs[i]->toStringInternal(preStr+ " |  ");
        lines.insert(lines.end(), subList.begin(), subList.end());
    }
    lines.push_back(preStr+" ----");
    return lines;
}



void GameObject::onSceneParentChange(Scene *oldParent, Scene *newParent) 
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::internalOnSceneParentChange(Scene *oldParent, Scene *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void GameObject::internalOnParentChange(GameObjectPtr oldParent, GameObjectPtr newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}


void GameObject::deleteLater()
{
    if(m_parent)
    {
       // m_parent->deleteChild(this);
        m_parent->removeChild(this);
    }
    else if(m_sceneParent)
        m_sceneParent->GameObjectContainer::deleteLater(this);

}

//void GameObject::positionChanged(const sf::Vector2f& oldPosition, const sf::Vector2f& newPosition)
//{
//    QSFML_UNUSED(oldPosition);
//    QSFML_UNUSED(newPosition);
//    sf::Vector2f globalPos = getGlobalPosition();
//    for (auto &collider : m_colliders)
//        collider->setPos(globalPos);
//}









void GameObject::setSceneParent(Scene *parent)
{
    if(m_sceneParent == parent)
        return;
    Scene *oldParent = m_sceneParent;
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
        if(m_selfOwnedLogObject)
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
        Component* comp = m_componentsManagerData.all[i];
        comp->setSceneParent(m_sceneParent);
    }

    //for(size_t i=0; i<m_components.size(); ++i)
    //    m_components[i]->setParent(this);

    for(size_t i=0; i< m_childObjectManagerData.objs.size(); ++i)
        m_childObjectManagerData.objs[i]->setSceneParent(parent);

    internalOnSceneParentChange(oldParent, m_sceneParent);
    onSceneParentChange(oldParent, m_sceneParent);
}

void GameObject::updateNewElements()
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    //removeChild_internal();
    //deleteChild_internal();
    //removeComponent_internal();
    //addChild_internal();
    //addComponent_internal();

    //for (size_t i = 0; i < m_childs.size(); ++i)
    //    m_childs[i]->updateNewElements();
    //m_objectsChanged = false;
    m_childObjectManagerData.objectsChanged = false;
    updateChanges_componentsManager();
    updateChanges_childObjectManager();
}
void GameObject::sfEvent(const std::vector<sf::Event>& events)
{
    if (!m_enabled || !m_updateControlls.enableEventLoop || !m_componentsManagerData.thisNeedsEventUpdate) return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_BLOCK("Components event", QSFML_COLOR_STAGE_2);
    for (auto component : m_componentsManagerData.eventHandler)
    {
        if (!component->isEnabled())
            continue;

        for (const auto &event : events)
            component->sfEvent(event);

    }
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs event", QSFML_COLOR_STAGE_3);
    for (auto obj : m_childObjectManagerData.objs)
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
    for (size_t i = 0; i < m_componentsManagerData.updatables.size(); ++i)
    {
        Utilities::Updatable* comp = m_componentsManagerData.updatables[i];
        Components::Component* comp1 = dynamic_cast<Components::Component*>(comp);
        if (!comp1->isEnabled())
            continue;
        comp->emitUpdate();
    }
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs update", QSFML_COLOR_STAGE_4);
    for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
    {
        GameObjectPtr obj = m_childObjectManagerData.objs[i];
        if (obj->m_enabled)
            obj->update_internal();
    }
    QSFMLP_OBJECT_END_BLOCK;
}

void GameObject::draw(sf::RenderWindow& window, sf::RenderStates states) const
{
    if (!m_enabled || !m_updateControlls.enablePaintLoop || !m_componentsManagerData.thisNeedsDrawUpdate)
        return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    states.transform.translate(getPosition());
    if (m_componentsManagerData.drawable.size())
    {
        QSFMLP_OBJECT_BLOCK("Components draw", QSFML_COLOR_STAGE_2);
        for (auto& comp : m_componentsManagerData.drawable)
        {
            if (!comp->isEnabled())
                continue;
            window.draw(*comp, states);
        }
        QSFMLP_OBJECT_END_BLOCK;
    }

    if (m_childObjectManagerData.objs.size())
    {
        QSFMLP_OBJECT_BLOCK("Childs draw", QSFML_COLOR_STAGE_3);
        for (auto& child : m_childObjectManagerData.objs)
        {
            if (child->m_enabled)
                child->draw(window, states);
        }
        QSFMLP_OBJECT_END_BLOCK;
    }
}



