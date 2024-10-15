#include "objects/base/GameObject.h"
#include "utilities/LifetimeChecker.h"
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
    //: Transformable()
    : DestroyEvent()
	, m_birthTick(0)
	, m_birthTime(0)
{
    Internal::LifetimeChecker::add(this);
    m_name = name;
    m_parent = parent;
    if(m_name.size() == 0)
    {
        s_objNameCounter++;
        m_name = "GameObject_"+std::to_string(s_objNameCounter);
    }
    m_updateControlls.enableUpdateLoop = true;
    m_updateControlls.enableEventLoop = true;
    m_updateControlls.enablePaintLoop = true;

    m_eventOrder = {
        EventSequenceElement::components,
        EventSequenceElement::childs,
		EventSequenceElement::customEventFunctions
    };
    m_updateOrder = { 
        UpdateSequenceElement::thisUpdate, 
        UpdateSequenceElement::customUpdateFunctions,
        UpdateSequenceElement::components,
        UpdateSequenceElement::childs
    };
    m_drawOrder = {
        DrawSequenceElement::components,
        DrawSequenceElement::childs,
		DrawSequenceElement::customDrawFunctions
    };
   
    m_sceneParent = nullptr;
    
    m_rootParent = this;
    if(m_parent)
        m_rootParent = m_parent->m_rootParent;
    //m_objectsChanged = false;
    m_componentsManagerData.thisNeedsDrawUpdate = false;
    m_enabled = true;
    //m_position = sf::Vector2f(0, 0);
    m_renderLayer = RenderLayer::layer_0;

    Components::Transform* transform = new Components::Transform();
    m_componentsManagerData.transform = transform;
    addComponent(transform);

    updateObjectChanges();
}
GameObject::GameObject(const GameObject &other)
    : DestroyEvent()
    , m_birthTick(0)
    , m_birthTime(0)
{
    Internal::LifetimeChecker::add(this);
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    m_sceneParent = nullptr;
    m_parent = nullptr;
    m_rootParent = this;
    m_componentsManagerData.thisNeedsDrawUpdate = false;
    m_updateControlls = other.m_updateControlls;
    m_renderLayer = other.m_renderLayer;
	m_updateOrder = other.m_updateOrder;

	m_onDrawCallbacks = other.m_onDrawCallbacks;
	m_onEventCallbacks = other.m_onEventCallbacks;
	m_onUpdateCallbacks = other.m_onUpdateCallbacks;

    m_eventOrder = other.m_eventOrder;
    m_updateOrder = other.m_updateOrder;
	m_drawOrder = other.m_drawOrder;

	m_boundingBox = other.m_boundingBox;

	m_getCustomBoundingBoxFunction = other.m_getCustomBoundingBoxFunction;
	

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
    updateObjectChanges();
}
GameObject::~GameObject()
{
    Internal::LifetimeChecker::setDead(this);
    if (m_sceneParent)
    {
        m_sceneParent->removeGameObject();
        m_sceneParent->removeComponent(m_componentsManagerData.all.size());
        m_sceneParent->removeObject(this);
    }

    for (auto& child : m_childObjectManagerData.objs)
    {
        Internal::LifetimeChecker::deleteSecured(child);
    }
    m_childObjectManagerData.objs.clear();

    
    for (auto& comp : m_componentsManagerData.all)
    {
        Internal::LifetimeChecker::deleteSecured(comp);
    }
    m_componentsManagerData.all.clear();
    m_componentsManagerData.toAdd.clear();
    m_componentsManagerData.updatables.clear();
    m_componentsManagerData.drawable.clear();
	//m_transformables.clear();
    m_childObjectManagerData.toRemove.clear();
    m_childObjectManagerData.toAdd.clear();


    delete m_selfOwnedLogObject;
}

void GameObject::deleteObject(GameObjectPtr obj)
{
    if(!obj)
		return;
    if (obj->m_parent)
    {
        obj->m_parent->removeChild(obj);
    }
    else if (obj->m_sceneParent)
        obj->m_sceneParent->GameObjectContainer::deleteLater(obj);
    else
    {
        Internal::LifetimeChecker::deleteSecured(obj);
    }
}


void GameObject::onAwake()
{
    
}
void GameObject::onEnable()
{

}
void GameObject::onDisable()
{

}

/*void GameObject::setParent(GameObjectPtr parent)
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
}*/
void GameObject::setEnabled(bool enable)
{
    m_enabled = enable;
	if (m_enabled)
		onEnable();
	else
		onDisable();
}
void GameObject::setName(const std::string &name)
{
    m_name = name;
    if(m_selfOwnedLogObject)
		m_selfOwnedLogObject->setName(name);
}

void GameObject::setParent_internal(GameObjectPtr parent,
                                      GameObjectPtr rootParent,
                                      Scene *sceneParent)
{
	QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());
	//QSFMLP_OBJECT_BLOCK("GameObject::setParent_internal: " + getName(), QSFML_COLOR_STAGE_1);
    if(m_parent == this)
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
    setSceneParent(sceneParent);
    if (parent, parent->m_logObject)
        setLogParent_internal(parent->m_logObject);
    //else if (sceneParent)
    //    setLogParent_internal(&sceneParent->getObjectLogger());
    else
        setLogParent_internal(nullptr);

    for (size_t i = 0; i < m_componentsManagerData.all.size(); ++i)
    {
        Component* comp = m_componentsManagerData.all[i];
        comp->setParent(this);
    }

    //internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
void GameObject::setLogParent_internal(Log::LogObject* parent)
{
    if (m_selfOwnedLogObject)
    {
        Log::LoggerID parentID = 0;
        if(parent)
			parentID = parent->getID();
		m_selfOwnedLogObject->setParentID(parentID);
	}
    else
    {
        m_logObject = parent;
    }
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

sf::Image GameObject::captureScreen()
{
	if (m_sceneParent)
		return m_sceneParent->captureScreen();
	return sf::Image();
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
	//QSFMLP_OBJECT_BLOCK("GameObject::toStringInternal: " + getName(), QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());
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
//void GameObject::internalOnSceneParentChange(Scene *oldParent, Scene *newParent)
//{
//    QSFML_UNUSED(oldParent);
//    QSFML_UNUSED(newParent);
//}
//void GameObject::internalOnParentChange(GameObjectPtr oldParent, GameObjectPtr newParent)
//{
//    QSFML_UNUSED(oldParent);
//    QSFML_UNUSED(newParent);
//}


void GameObject::deleteLater()
{
	deleteObject(this);
}

//void GameObject::positionChanged(const sf::Vector2f& oldPosition, const sf::Vector2f& newPosition)
//{
//    QSFML_UNUSED(oldPosition);
//    QSFML_UNUSED(newPosition);
//    sf::Vector2f globalPos = getGlobalPosition();
//    for (auto &collider : m_colliders)
//        collider->setPos(globalPos);
//}










void GameObject::updateObjectChanges()
{
	//QSFMLP_OBJECT_BLOCK("GameObject::updateObjectChanges: " + getName(), QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());
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

    if (this == m_rootParent)
    {
        updateTransformInternal();
    }
}

void GameObject::sfEvent(const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>& events)
{
    if (!m_enabled || !m_updateControlls.enableEventLoop || !m_componentsManagerData.thisNeedsEventUpdate) return;
    //QSFMLP_OBJECT_BLOCK("GameObject::sfEvent: count="+std::to_string(events.size())+" " + getName(), QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());

    for (const auto& seq : m_eventOrder)
    {
        switch (seq)
        {
            case EventSequenceElement::childs:
            {
                if (m_childObjectManagerData.objs.size())
                {
                    QSFMLP_OBJECT_BLOCK("Childs event", QSFML_COLOR_STAGE_3);
                    for (auto& child : m_childObjectManagerData.objs)
                    {
                        if (child->m_enabled)
                            child->sfEvent(events);
                    }
                    QSFMLP_OBJECT_END_BLOCK;
                }
                break;
            }
            case EventSequenceElement::components:
            {
                if (m_componentsManagerData.eventHandler.size())
                {
                    QSFMLP_OBJECT_BLOCK("Components event", QSFML_COLOR_STAGE_2);
                    for (auto& comp : m_componentsManagerData.eventHandler)
                    {
                        if (!comp->isEnabled())
                            continue;
                        {
                            QSFMLP_COMPONENT_BLOCK("Component event", QSFML_COLOR_STAGE_1);
                            QSFMLP_OBJECT_TEXT("Name", comp->getName());
                            comp->sfEvent_internal(events);
                        }
                    }
                    QSFMLP_OBJECT_END_BLOCK;
                }
                break;
            }
            case EventSequenceElement::customEventFunctions:
            {
                if (m_onEventCallbacks.size() > 0)
                {
                    QSFMLP_OBJECT_BLOCK("Custom event functions", QSFML_COLOR_STAGE_2);
                    for (size_t i = 0; i < m_onEventCallbacks.size(); ++i)
                    {
                        m_onEventCallbacks[i](*this, events);
                    }
                    QSFMLP_OBJECT_END_BLOCK;
                }
                break;
            }
        }
    }
}
void GameObject::update_internal()
{
    if (!m_enabled || !m_updateControlls.enableUpdateLoop) return;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());
    for (const auto& seq : m_updateOrder)
    {
        switch (seq)
        {            
            case UpdateSequenceElement::childs:
            {
                QSFMLP_OBJECT_BLOCK("Childs update", QSFML_COLOR_STAGE_4);
                for (size_t i = 0; i < m_childObjectManagerData.objs.size(); ++i)
                {
                    GameObjectPtr obj = m_childObjectManagerData.objs[i];
                    if (obj->m_enabled)
                        obj->update_internal();
                }
                QSFMLP_OBJECT_END_BLOCK;
                break;
            }
            case UpdateSequenceElement::components:
            {
                QSFMLP_OBJECT_BLOCK("Components update", QSFML_COLOR_STAGE_2);
                for (size_t i = 0; i < m_componentsManagerData.updatables.size(); ++i)
                {
                    Utilities::Updatable* comp = m_componentsManagerData.updatables[i];
                    Components::Component* comp1 = dynamic_cast<Components::Component*>(comp);

                    if (!comp1->isEnabled())
                        continue;
                    {
                        QSFMLP_COMPONENT_BLOCK("Component update", QSFML_COLOR_STAGE_1);
                        QSFMLP_OBJECT_TEXT("Name", comp1->getName());
                        comp->emitUpdate();
                    }
                }
                QSFMLP_OBJECT_END_BLOCK;
                break;
            }
            case UpdateSequenceElement::thisUpdate:
            {
                QSFMLP_OBJECT_BLOCK("Object update", QSFML_COLOR_STAGE_2);
                update();
                QSFMLP_OBJECT_END_BLOCK;
                break;
            }
            case UpdateSequenceElement::customUpdateFunctions:
            {
				if (m_onUpdateCallbacks.size() == 0)
					break;
                QSFMLP_OBJECT_BLOCK("Custom update functions", QSFML_COLOR_STAGE_2);
                for (size_t i = 0; i < m_onUpdateCallbacks.size(); ++i)
                {
                    m_onUpdateCallbacks[i](*this);
                }
                QSFMLP_OBJECT_END_BLOCK;
                break;
            }
            default:
            {
				std::string typeName = typeid(*this).name();
				const std::string& name = getName();
                logError("update_internal(): Unknown sequence element: " + 
                         std::to_string(static_cast<int>(seq))+ 
                " for object of type: "+typeName + " name: "+name);
            }
        }
    }
}

void GameObject::draw(sf::RenderWindow& window, sf::RenderStates states) const
{
    if (!m_enabled || !m_updateControlls.enablePaintLoop || !m_componentsManagerData.thisNeedsDrawUpdate)
    {
        return;
    }
	//QSFMLP_OBJECT_BLOCK("GameObject::draw: " + getName(), QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_TEXT("Name", getName());
    states.transform *= getTransform();
    //states.transform = sf::Transform();

    for (const auto& seq : m_drawOrder)
    {
        switch (seq)
        {
            case DrawSequenceElement::childs:
            {
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
                break;
            }
            case DrawSequenceElement::components:
            {
                if (m_componentsManagerData.drawable.size())
                {
                    QSFMLP_OBJECT_BLOCK("Components draw", QSFML_COLOR_STAGE_2);
                    for (auto& comp : m_componentsManagerData.drawable)
                    {
                        if (!comp->isEnabled())
                            continue;
                        {
							comp->draw(window, states);
                            //window.draw(*comp, states);
                        }
                    }
                    QSFMLP_OBJECT_END_BLOCK;
                }
                break;
            }
            case DrawSequenceElement::customDrawFunctions:
            {
                if (m_onDrawCallbacks.size() > 0)
                {
                    QSFMLP_OBJECT_BLOCK("Custom draw functions", QSFML_COLOR_STAGE_2);
                    for (size_t i = 0; i < m_onDrawCallbacks.size(); ++i)
                    {
                        m_onDrawCallbacks[i](*this, window, states);
                    }
                    QSFMLP_OBJECT_END_BLOCK;
                }
            }
        }
    }
}



