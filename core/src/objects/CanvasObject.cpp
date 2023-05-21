#include "objects/CanvasObject.h"
#include "canvas/Canvas.h"
#include "canvas/Stats.h"



using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

OBJECT_IMPL(CanvasObject)

size_t CanvasObject::s_objNameCounter = 0;
CanvasObject::CanvasObject(const std::string &name, CanvasObject *parent)
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
    m_position = sf::Vector2f(0, 0);
    m_renderLayer = RenderLayer::layer_0;
    
}
CanvasObject::CanvasObject(const CanvasObject &other)
{
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    m_position = other.m_position;
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

    StatsManager::removeCanvasObject(m_canvasParent);
    StatsManager::removeComponent(m_canvasParent, m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
        delete m_components[i];
    m_components.clear();
    m_toAddComponents.clear();
    m_updatableComponents.clear();
    m_drawableComponents.clear();
    m_toRemoveComponents.clear();
    m_toDeleteChilds.clear();
    m_toRemoveChilds.clear();
    if(m_canvasParent)
        m_canvasParent->removeObject(this);
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
const std::string CanvasObject::getName() const
{
    return m_name;
}
void CanvasObject::setPositionRelative(const sf::Vector2f& pos)
{
    m_position = pos;
    for (size_t i = 0; i < m_colliders.size(); ++i)
        m_colliders[i]->setPos(m_position);
}
void CanvasObject::setPositionAbsolute(const sf::Vector2f& pos)
{
    if (m_parent)
    {
        sf::Vector2f parentPos = m_parent->getPositionAbsolute();
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
sf::Vector2f CanvasObject::getPositionAbsolute() const
{
    if (m_parent)
    {
        sf::Vector2f parentPos = m_parent->getPositionAbsolute();
        return m_position + parentPos;
    }
    return m_position;
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
    StatsManager::removeComponent(m_canvasParent, removedCount);

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

    size_t index = getComponentIndex(comp);
    if(index == npos) return;
    m_components.erase(m_components.begin() + index);
    StatsManager::removeComponent(m_canvasParent);

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
    for(size_t i=0; i<m_components.size(); ++i)
        m_components[i]->setParent(this);

    internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
void CanvasObject::addComponent_internal()
{
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        Components::Component *toAdd = m_toAddComponents[i];
        if(componentExists(toAdd))
            continue;

        toAdd->setParent(this);
        StatsManager::addComponent(m_canvasParent);
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
            collider->setPos(m_position);
        }
    }
    m_toAddComponents.clear();
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
        m_toAddComponents[i]->setParent(nullptr);
        delete m_toAddComponents[i];
    }
    StatsManager::removeComponent(m_canvasParent, m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
    {
        m_components[i]->setParent(nullptr);
        delete m_components[i];
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
const std::vector<Components::Collider*> &CanvasObject::getCollider() const
{
    return m_colliders;
}
bool CanvasObject::checkCollision(const CanvasObject* other) const
{
    std::vector<Collisioninfo> collisions;
    return checkCollision(other, collisions);
}
bool CanvasObject::checkCollision(const CanvasObject* other, 
    std::vector<Collisioninfo>& collisions, 
    bool onlyFirstCollision) const
{
    std::vector<Components::Collider*> otherColliders = other->getCollider();
    for (auto thisCollider : m_colliders)
    {
        thisCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
    }
    if (collisions.size() > 0)
        return true;
    return false;
}
void CanvasObject::solveCollision(CanvasObject* other)
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        m_colliders[i]->resolveCollision(other->getCollider());
    }
}


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
float CanvasObject::getDeltaT() const
{
    if(!m_canvasParent) return 0;
    return m_canvasParent->getDeltaT();
}

const CanvasSettings::UpdateControlls &CanvasObject::getUpdateControlls() const
{
    return m_updateControlls;
}
void CanvasObject::setUpdateControlls(const CanvasSettings::UpdateControlls &controlls)
{
    m_updateControlls = controlls;
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
void CanvasObject::update()
{

}
std::vector<std::string> CanvasObject::toStringInternal(const std::string &preStr) const
{
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

void CanvasObject::onCanvasParentChange(Canvas *oldParent, Canvas *newParent) {}
void CanvasObject::onParentChange(CanvasObject *oldParent, CanvasObject *newParent) {}
void CanvasObject::internalOnCanvasParentChange(Canvas *oldParent, Canvas *newParent) {}
void CanvasObject::internalOnParentChange(CanvasObject *oldParent, CanvasObject *newParent) {}


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
    if (m_canvasParent == nullptr)
    {
        StatsManager::removeCanvasObject(oldParent);
        StatsManager::removeComponent(oldParent, m_components.size());
    }
    else if (oldParent == nullptr)
    {
        StatsManager::addCanvesObject(m_canvasParent);
        StatsManager::addComponent(m_canvasParent, m_components.size());
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
    QSFMLP_FUNCTION(QSFMLP_OBJECT_COLOR_1);
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
    QSFMLP_FUNCTION(QSFMLP_OBJECT_COLOR_1);
    QSFMLP_BLOCK("Components event", QSFMLP_OBJECT_COLOR_2);
    for (auto component : m_eventComponents)
    {
        if (!component->isEnabled())
            continue;

        for (const auto &event : events)
            component->sfEvent(event);

    }
    QSFMLP_END_BLOCK;

    QSFMLP_BLOCK("Childs event", QSFMLP_OBJECT_COLOR_3);
    for (auto obj : m_childs)
    {
        if (obj->m_enabled)
            obj->sfEvent(events);
    }
    QSFMLP_END_BLOCK;
}
void CanvasObject::update_internal()
{
    if (!m_enabled || !m_updateControlls.enableUpdateLoop) return;
    QSFMLP_FUNCTION(QSFMLP_OBJECT_COLOR_1);
    QSFMLP_BLOCK("Object update", QSFMLP_OBJECT_COLOR_2);
    update();
    QSFMLP_END_BLOCK;

    QSFMLP_BLOCK("Components update", QSFMLP_OBJECT_COLOR_3);
    for (size_t i = 0; i < m_updatableComponents.size(); ++i)
    {
        Utilities::Updatable* comp = m_updatableComponents[i];
        Components::Component* comp1 = dynamic_cast<Components::Component*>(comp);
        if (!comp1->isEnabled())
            continue;
        comp->update();
    }
    QSFMLP_END_BLOCK;

    QSFMLP_BLOCK("Childs update", QSFMLP_OBJECT_COLOR_4);
    for (size_t i = 0; i < m_childs.size(); ++i)
    {
        CanvasObject* obj = m_childs[i];
        if (obj->m_enabled)
            obj->update_internal();
    }
    QSFMLP_END_BLOCK;
}
void CanvasObject::draw(sf::RenderWindow &window) const
{
    if(!m_enabled || !m_updateControlls.enablePaintLoop || !m_thisNeedsDrawUpdate) return;
    QSFMLP_FUNCTION(QSFMLP_OBJECT_COLOR_1);
    QSFMLP_BLOCK("Components draw", QSFMLP_OBJECT_COLOR_2);
    for(size_t i=0; i<m_drawableComponents.size(); ++i)
    {
        if(!m_drawableComponents[i]->isEnabled())
            continue;
        window.draw(*m_drawableComponents[i]);
    }
    QSFMLP_END_BLOCK;

    QSFMLP_BLOCK("Childs draw", QSFMLP_OBJECT_COLOR_3);
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->draw(window);
    }
    QSFMLP_END_BLOCK;
}

