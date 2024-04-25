#include "objects/base/CanvasObject.h"
#include "canvas/Canvas.h"

#include "components/base/Component.h"
#include "components/base/SfEventHandle.h"
#include "components/base/Drawable.h"
#include "components/physics/Collider.h"

#include "utilities/Stats.h"
#include "utilities/ObjectQuadTree.h"

#include <QDebug>


using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

OBJECT_IMPL(CanvasObject)

size_t CanvasObject::s_objNameCounter = 0;
CanvasObject::CanvasObject(const std::string &name, CanvasObject *parent)
    : Transformable()
    , Updatable()
    , DestroyEvent()
{
    m_name = name;
    if(m_name.size() == 0)
    {
        s_objNameCounter++;
        m_name = "CanvasObject_"+std::to_string(s_objNameCounter);
    }
    m_updateControlls.enableUpdateLoop = true;
    m_updateControlls.enableEventLoop = true;
    m_updateControlls.enablePaintLoop = true;
   
    m_canvasParent = nullptr;
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
CanvasObject::CanvasObject(const CanvasObject &other)
    : Transformable()
    , Updatable()
    , DestroyEvent()
{
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    //m_position = other.m_position;
    m_canvasParent = nullptr;
    m_parent = nullptr;
    m_rootParent = this;
    m_objectsChanged = false;
    m_thisNeedsDrawUpdate = false;
    m_updateControlls = other.m_updateControlls;
    m_renderLayer = other.m_renderLayer;

    m_childs.reserve(other.m_childs.size());
    for(size_t i=0; i<other.m_childs.size(); ++i)
    {
        CanvasObject *obj = other.m_childs[i]->clone();
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
CanvasObject::~CanvasObject()
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
    m_toRemoveComponents.clear();
    m_toDeleteChilds.clear();
    m_toRemoveChilds.clear();
    if (m_canvasParent)
    {
        m_canvasParent->removeCanvasObject();
        m_canvasParent->removeComponent(m_components.size());
        m_canvasParent->removeObject(this);   
    }
}


void CanvasObject::inCanvasAdded()
{

}
void CanvasObject::setParent(CanvasObject *parent)
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
void CanvasObject::setEnabled(bool enable)
{
    m_enabled = enable;
}
bool CanvasObject::isEnabled() const
{
    return m_enabled;
}
CanvasObject *CanvasObject::getParent() const
{
    return m_parent;
}
CanvasObject *CanvasObject::getRootParent() const
{
    return m_rootParent;
}

void CanvasObject::setName(const std::string &name)
{
    m_name = name;
}
const std::string &CanvasObject::getName() const
{
    return m_name;
}

double CanvasObject::getAge() const
{
    if (!m_canvasParent) return 0;
    return m_canvasParent->getElapsedTime() - m_birthTime;
}
double CanvasObject::getBirthTime() const
{
    return m_birthTime;
}
size_t CanvasObject::getAgeTicks() const
{
    if (!m_canvasParent) return 0;
    return m_canvasParent->getTick() - m_birthTick;
}
size_t CanvasObject::getBirthTick() const
{
    return m_birthTick;
}
double CanvasObject::getAgeFixed() const
{
    if (!m_canvasParent) return 0;
    return m_canvasParent->getFixedDeltaT() * (m_canvasParent->getTick() - m_birthTick);
}


/*void CanvasObject::setPositionRelative(const sf::Vector2f& pos)
{
    m_position = pos;
    for (size_t i = 0; i < m_colliders.size(); ++i)
        m_colliders[i]->setPos(m_position);
}
void CanvasObject::setPosition(const sf::Vector2f& pos)
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
const sf::Vector2f& CanvasObject::getPositionRelative() const
{
    return m_position;
}
sf::Vector2f CanvasObject::getPosition() const
{
    if (m_parent)
    {
        sf::Vector2f parentPos = m_parent->getPosition();
        return m_position + parentPos;
    }
    return m_position;
}*/
sf::Vector2f CanvasObject::getGlobalPosition() const
{
    sf::Vector2f pos = getPosition();
    CanvasObject *parent = m_parent;
    while (parent)
    {
        pos += parent->getPosition();
        parent = parent->m_parent;
    }
    return pos;
}
float CanvasObject::getGlobalRotation() const
{
	float rot = getRotation();
	CanvasObject *parent = m_parent;
	while (parent)
	{
		rot += parent->getRotation();
		parent = parent->m_parent;
	}
	return rot;
}

void CanvasObject::setRenderLayer(RenderLayer layer)
{
    if(m_renderLayer == layer)
        return;
    RenderLayer oldLayer = m_renderLayer;
    m_renderLayer = layer;
    if(m_canvasParent)
        m_canvasParent->renderLayerSwitch(this, oldLayer, m_renderLayer);
}

RenderLayer CanvasObject::getRenderLayer() const
{
    return m_renderLayer;
}

// Childs operations
void CanvasObject::addChild(CanvasObject *child)
{
    if(!child)return;

    m_toAddChilds.push_back(child);
    onObjectsChanged();

}
void CanvasObject::addChilds(const std::vector<CanvasObject*>& childs)
{
    for (size_t i = 0; i < childs.size(); ++i)
    {
        CanvasObject* child = childs[i];
        if(child)
            m_toAddChilds.push_back(child);
    }
    onObjectsChanged();
}

void CanvasObject::removeChild(CanvasObject *child)
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
void CanvasObject::removeChilds(const std::vector<CanvasObject*>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        CanvasObject* child = childs[j];
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
void CanvasObject::removeChilds()
{
    m_toRemoveChilds = m_childs;
    m_toAddChilds.clear();
    onObjectsChanged();
}
void CanvasObject::removeChild_internal()
{
    for(size_t i=0; i<m_toRemoveChilds.size(); ++i)
    {
        size_t index = getChildIndex(m_toRemoveChilds[i]);
        if(index != npos)
            m_childs.erase(m_childs.begin() + index);
    }
    m_toRemoveChilds.clear();
}

void CanvasObject::deleteChild(CanvasObject *child)
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
void CanvasObject::deleteChilds(const std::vector<CanvasObject*>& childs)
{
    for (size_t j = 0; j < childs.size(); ++j)
    {
        CanvasObject* child = childs[j];
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
void CanvasObject::deleteChilds()
{
    m_toDeleteChilds = m_childs;
    m_toRemoveChilds.clear();
    m_toAddChilds.clear();
    onObjectsChanged();
}
void CanvasObject::deleteChild_internal()
{
    for(size_t i=0; i<m_toDeleteChilds.size(); ++i)
    {
        size_t index = getChildIndex(m_toDeleteChilds[i]);
        if(index != npos)
            m_childs.erase(m_childs.begin() + index);
        qDebug() << "CanvasObject::deleteChild_internal(): "<<m_toDeleteChilds[i]->getName().c_str() << " child of: "<<getName().c_str();
        delete m_toDeleteChilds[i];
    }
    m_toDeleteChilds.clear();
}
bool CanvasObject::childExists(CanvasObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return true;
    return false;
}
size_t CanvasObject::getChildIndex(CanvasObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return i;
    return npos;
}
const std::vector<CanvasObject*> &CanvasObject::getChilds() const
{
    return m_childs;
}
size_t CanvasObject::getChildCount() const
{
    return m_childs.size();
}

CanvasObject* CanvasObject::findFirstChild(const std::string& name)
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
std::vector<CanvasObject*> CanvasObject::findAllChilds(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<CanvasObject*> results;
    results.reserve(m_childs.size());
    findAllChilds_internal(name, results);
    return results;
}

CanvasObject* CanvasObject::findFirstChildRecursive(const std::string& name)
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
        CanvasObject *obj = m_childs[i]->findFirstChildRecursive(name);
        if(obj)
            return obj;
    }
    return nullptr;
}
std::vector<CanvasObject*> CanvasObject::findAllChildsRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<CanvasObject*> results;
    results.reserve(m_childs.size()*2);
    findAllChildsRecursive_internal(name, results);
    return results;
}

void CanvasObject::addComponent(Component *comp)
{
    if(!comp)return;
    m_toAddComponents.push_back(comp);
    onObjectsChanged();
}
void CanvasObject::addComponents(const std::vector<Components::Component*>& components)
{
    for (size_t i = 0; i < components.size(); ++i)
    {
        Components::Component* comp = components[i];
        if (comp)
            m_toAddComponents.push_back(comp);
    }
    onObjectsChanged();
}
void CanvasObject::removeComponent(Component *comp)
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
void CanvasObject::removeComponents(const std::vector<Components::Component*>& components)
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
void CanvasObject::removeComponent_internal()
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
    if(m_canvasParent)
        m_canvasParent->removeComponent(removedCount);

}
void CanvasObject::deleteComponent(Component *comp)
{
    if(!comp)return;


    for(size_t i=0; i<m_toAddComponents.size(); ++i)
        if(m_toAddComponents[i] == comp)
        {
            m_toAddComponents.erase(m_toAddComponents.begin() + i);
            break;
        }
    comp->setParent(nullptr);
    comp->setCanvasParent(nullptr);

    size_t index = getComponentIndex(comp);
    if(index == npos) return;
    m_components.erase(m_components.begin() + index);
    if (m_canvasParent)
        m_canvasParent->removeComponent();

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
    delete comp;
}
void CanvasObject::deleteComponents(const std::vector<Components::Component*>& components)
{
    for (size_t j = 0; j < components.size(); ++j)
    {
        Components::Component* comp = components[j];
        deleteComponent(comp);
    }
}
void CanvasObject::addChild_internal()
{
    for(size_t i=0; i<m_toAddChilds.size(); ++i)
    {
        if(childExists(m_toAddChilds[i]))
            continue;
        addChild_internal(m_toAddChilds[i]);
    }
    m_toAddChilds.clear();
}
void CanvasObject::addChild_internal(CanvasObject *obj)
{
    m_childs.push_back(obj);
    obj->setParent_internal(this, m_rootParent, m_canvasParent);
}
void CanvasObject::setParent_internal(CanvasObject *parent,
                                      CanvasObject *rootParent,
                                      Canvas *canvasParent)
{
    if(m_parent != nullptr && m_parent == this)
        return;
    if(m_parent)
    {
        size_t index = m_parent->getChildIndex(this);
        if(index == npos) return;
        m_parent->m_childs.erase(m_parent->m_childs.begin() + index);
    }
    CanvasObject *oldParent = m_parent;
    m_parent = parent;
    m_rootParent = rootParent;
    setCanvasParent(canvasParent);
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setParent(this);
    }

    internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
void CanvasObject::addComponent_internal()
{
    size_t colliderCount = m_colliders.size();
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Components::Component *toAdd = m_toAddComponents[i];
        if(componentExists(toAdd))
            continue;

        toAdd->setParent(this);
        toAdd->setCanvasParent(m_canvasParent);
        if (m_canvasParent)
            m_canvasParent->addComponent();
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
void CanvasObject::onObjectsChanged()
{
    m_objectsChanged = true;
    if(m_parent)
    {
        m_parent->onObjectsChanged();
    }
}
void CanvasObject::deleteComponents()
{
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Component* comp = m_toAddComponents[i];
        comp->setParent(nullptr);
        comp->setCanvasParent(nullptr);
        delete comp;
    }
    if (m_canvasParent)
        m_canvasParent->removeComponent(m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setParent(nullptr);
        comp->setCanvasParent(nullptr);
        delete comp;
    }
    m_updatableComponents.clear();
    m_eventComponents.clear();    
    m_drawableComponents.clear();
    m_toAddComponents.clear();
    m_components.clear();
}
bool CanvasObject::componentExists(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return true;
    return false;
}
size_t CanvasObject::getComponentIndex(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return i;
    return npos;
}
const std::vector<Component*> &CanvasObject::getComponents() const
{
    return m_components;
}

Components::Component* CanvasObject::findFirstComponent(const std::string& name)
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
std::vector<Components::Component*> CanvasObject::findAllComponents(const std::string& name)
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

Components::Component* CanvasObject::findFirstComponentRecursive(const std::string& name)
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
std::vector<Components::Component*> CanvasObject::findAllComponentsRecursive(const std::string& name)
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    std::vector<Components::Component*> results;
    results.reserve(m_components.size());
    findAllComponentsRecursive_internal(name, results);
    return results;
}
bool CanvasObject::findAllComponentsRecursive_internal(const std::string& name, std::vector<Components::Component*>& foundList)
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


const std::vector<Components::Collider*> &CanvasObject::getCollider() const
{
    return m_colliders;
}
bool CanvasObject::checkCollision(const CanvasObject* other) const
{
    std::vector<Utilities::Collisioninfo> collisions;
    return checkCollision(other, collisions);
}
bool CanvasObject::checkCollision(const CanvasObject* other, 
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
void CanvasObject::checkCollision(const Utilities::ObjectQuadTree& tree, 
                                  std::vector<Utilities::Collisioninfo>& collisions,
                                  bool onlyFirstCollision)
{
    std::list<Utilities::ObjectQuadTree::TreeItem> objs = tree.getAllItems();
    for (auto &objStruct : objs)
    {
        CanvasObject* obj = objStruct.obj;
        std::list< QSFML::Objects::CanvasObject*> possibleColliders;
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
/*void CanvasObject::solveCollision(CanvasObject* other)
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        m_colliders[i]->resolveCollision(other->getCollider());
    }
}*/


size_t CanvasObject::getComponentCount() const
{
    return m_components.size();
}

sf::Vector2i CanvasObject::getMousePosition() const
{
    if (!m_canvasParent) return sf::Vector2i(0, 0);
    return m_canvasParent->getMousePosition();
}
sf::Vector2f CanvasObject::getMouseWorldPosition() const
{
    if (!m_canvasParent) return sf::Vector2f(0, 0);
    return m_canvasParent->getMouseWorldPosition();
}
sf::Vector2f CanvasObject::getMouseObjectPosition() const
{
    return getMouseWorldPosition() - getGlobalPosition();
}

sf::Vector2f CanvasObject::getInWorldSpace(const sf::Vector2i& pixelSpace) const
{
    if (!m_canvasParent) return sf::Vector2f(0, 0);
    return m_canvasParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i CanvasObject::getInScreenSpace(const sf::Vector2f& worldSpace) const
{
    if (!m_canvasParent) return sf::Vector2i(0, 0);
    return m_canvasParent->getInScreenSpace(worldSpace);
}
const sf::View CanvasObject::getCameraView() const
{
    static const sf::View dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getCameraView();
}
const sf::View &CanvasObject::getDefaultCameraView() const
{
    static const sf::View dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getDefaultCameraView();
}
Utilities::AABB CanvasObject::getCameraViewRect() const
{
    if (!m_canvasParent) return Utilities::AABB();
    return m_canvasParent->getCameraViewRect();
}
sf::Vector2u CanvasObject::getCanvasSize() const
{
    if(!m_canvasParent) return sf::Vector2u(0,0);
    return m_canvasParent->getCanvasSize();
}
sf::Vector2u CanvasObject::getOldCanvasSize() const
{
    if(!m_canvasParent) return sf::Vector2u(0,0);
    return m_canvasParent->getOldCanvasSize();
}

const sf::Font &CanvasObject::getTextFont() const
{
    const static sf::Font dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getTextFont();
}
size_t CanvasObject::getTick() const
{
    if(!m_canvasParent) return 0;
    return m_canvasParent->getTick();
}
double CanvasObject::getDeltaT() const
{
    if(!m_canvasParent) return 0;
    return m_canvasParent->getDeltaT();
}
double CanvasObject::getFixedDeltaT() const
{
    if (!m_canvasParent) return 0;
    return m_canvasParent->getFixedDeltaT();
}
double CanvasObject::getElapsedTime() const
{
    if(!m_canvasParent) return 0;
    return m_canvasParent->getElapsedTime();
}
double CanvasObject::getFixedElapsedTime() const
{
    if (!m_canvasParent) return 0;
	return m_canvasParent->getFixedElapsedTime();
}


const CanvasSettings::UpdateControlls &CanvasObject::getUpdateControlls() const
{
    return m_updateControlls;
}
void CanvasObject::setUpdateControlls(const CanvasSettings::UpdateControlls &controlls)
{
    m_updateControlls = controlls;
}

const Utilities::AABB &CanvasObject::getBoundingBox() const
{
    return m_boundingBox;
}
void CanvasObject::updateBoundingBox()
{
    std::vector<Utilities::AABB> boxes;
    boxes.reserve(m_colliders.size());
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        boxes.push_back(m_colliders[i]->getBoundingBox());
    }
    m_boundingBox = Utilities::AABB::getFrame(boxes);
}


std::string CanvasObject::toString() const
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
Canvas* CanvasObject::getCanvasParent() const
{
    return m_canvasParent;
}

Objects::CanvasObject* CanvasObject::findFirstObjectGlobal(const std::string& name)
{
    if (!m_canvasParent) return nullptr;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_canvasParent->findFirstObject(name);
}
std::vector<Objects::CanvasObject*> CanvasObject::findAllObjectsGlobal(const std::string& name)
{
    if (!m_canvasParent) return std::vector<CanvasObject*>();
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_canvasParent->findAllObjects(name);
}
Objects::CanvasObject* CanvasObject::findFirstObjectGlobalRecursive(const std::string& name)
{
    if (!m_canvasParent) return nullptr;
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    return m_canvasParent->findFirstObjectRecursive(name);
}
std::vector<Objects::CanvasObject*> CanvasObject::findAllObjectsGlobalRecusive(const std::string& name)
{
    if (!m_canvasParent) return std::vector<CanvasObject*>();
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
	return m_canvasParent->findAllObjectsRecursive(name);
}

void CanvasObject::update()
{

}
std::vector<std::string> CanvasObject::toStringInternal(const std::string &preStr) const
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    using std::string;
    std::vector<string> lines;
    const CanvasObject *t = this;

    auto en = [](bool enabled) {
        return enabled?"[Enabled ]":"[Disabled]";
      };

    lines.push_back(preStr+" | "+string(typeid(t).name())+": \""+m_name + "\" "+en(t->isEnabled()));
    lines.push_back(preStr+" |  Components:");
    for(size_t i=0; i<m_components.size(); ++i)
    {
        lines.push_back(preStr+  " |  - " +string(typeid(m_components[i]).name())+": \""+m_components[i]->getName()+ "\" "+en(m_components[i]->isEnabled()));
    }
    lines.push_back(preStr+" |  Childs:");
    for(size_t i=0; i<m_childs.size(); ++i)
    {

        std::vector<std::string> subList = m_childs[i]->toStringInternal(preStr+" | ");
        for(size_t j=0; j<subList.size(); ++j)
        {
            lines.push_back(subList[j]);
        }

    }
    lines.push_back(preStr+" ----");
    return lines;
}
bool CanvasObject::findAllChilds_internal(const std::string& name, std::vector<CanvasObject*>& foundList)
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
bool CanvasObject::findAllChildsRecursive_internal(const std::string& name, std::vector<CanvasObject*>& foundList)
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

void CanvasObject::onCanvasParentChange(Canvas *oldParent, Canvas *newParent) 
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void CanvasObject::onParentChange(CanvasObject *oldParent, CanvasObject *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void CanvasObject::internalOnCanvasParentChange(Canvas *oldParent, Canvas *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}
void CanvasObject::internalOnParentChange(CanvasObject *oldParent, CanvasObject *newParent)
{
    QSFML_UNUSED(oldParent);
    QSFML_UNUSED(newParent);
}


void CanvasObject::deleteThis()
{
    if(m_parent)
    {
       // m_parent->deleteChild(this);
        m_parent->removeChild(this);
    }
    else if(m_canvasParent)
        m_canvasParent->CanvasObjectContainer::deleteLater(this);

}

void CanvasObject::positionChanged(const sf::Vector2f& oldPosition, const sf::Vector2f& newPosition)
{
    QSFML_UNUSED(oldPosition);
    QSFML_UNUSED(newPosition);
    sf::Vector2f globalPos = getGlobalPosition();
    for (auto &collider : m_colliders)
        collider->setPos(globalPos);
}
void CanvasObject::rotationChanged(float oldRotation, float newRotation)
{
    QSFML_UNUSED(oldRotation);
    QSFML_UNUSED(newRotation);
}



void CanvasObject::needsEventUpdateChanged(bool needsEventUpdate)
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
void CanvasObject::needsEventUpdate(bool needsEventUpdate)
{
    if(m_thisNeedsEventUpdate == needsEventUpdate)
        return;
    m_thisNeedsEventUpdate = needsEventUpdate;
    if(m_parent)
        m_parent->needsEventUpdateChanged(m_thisNeedsEventUpdate);
}

void CanvasObject::needsDrawUpdateChanged(bool needsDrawUpdate)
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
void CanvasObject::needsDrawUpdate(bool needsDrawUpdate)
{
    if(m_thisNeedsDrawUpdate == needsDrawUpdate)
        return;
    m_thisNeedsDrawUpdate = needsDrawUpdate;
    if(m_parent)
        m_parent->needsDrawUpdateChanged(m_thisNeedsDrawUpdate);
}

void CanvasObject::setCanvasParent(Canvas *parent)
{
    if(m_canvasParent == parent)
        return;
    Canvas *oldParent = m_canvasParent;
    m_canvasParent = parent;
    if (oldParent != nullptr)
    {
        oldParent->removeCanvasObject();
        oldParent->removeComponent(m_components.size());
    }
    if (m_canvasParent != nullptr)
    {
        m_canvasParent->addCanvesObject();
        m_canvasParent->addComponent(m_components.size());

        // Set the birth time and tick
        m_birthTick = m_canvasParent->getTick();
        m_birthTime = m_canvasParent->getElapsedTime();
    }

    for (size_t i = 0; i < m_components.size(); ++i)
    {
        Component* comp = m_components[i];
        comp->setCanvasParent(m_canvasParent);
    }

    //for(size_t i=0; i<m_components.size(); ++i)
    //    m_components[i]->setParent(this);

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->setCanvasParent(parent);

    internalOnCanvasParentChange(oldParent, m_canvasParent);
    onCanvasParentChange(oldParent, m_canvasParent);
}

void CanvasObject::updateNewElements()
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
void CanvasObject::sfEvent(const std::vector<sf::Event>& events)
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
void CanvasObject::update_internal()
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
        CanvasObject* obj = m_childs[i];
        if (obj->m_enabled)
            obj->update_internal();
    }
    QSFMLP_OBJECT_END_BLOCK;
}
void CanvasObject::inCanvasAdded_internal()
{
    QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
    QSFMLP_OBJECT_BLOCK("Object inCanvasAdded", QSFML_COLOR_STAGE_2);
    inCanvasAdded();
    QSFMLP_OBJECT_END_BLOCK;

    QSFMLP_OBJECT_BLOCK("Childs inCanvasAdded", QSFML_COLOR_STAGE_4);
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        m_childs[i]->inCanvasAdded_internal();
    }
    QSFMLP_OBJECT_END_BLOCK;
}
void CanvasObject::draw(sf::RenderWindow& window, sf::RenderStates states) const
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



