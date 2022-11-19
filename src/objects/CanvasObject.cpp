#include "objects/CanvasObject.h"
#include "canvas/Canvas.h"




using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

size_t CanvasObject::m_objNameCounter = 0;
CanvasObject::CanvasObject(const std::string &name, CanvasObject *parent)
{
    m_name = name;
    if(m_name.size() == 0)
    {
        m_objNameCounter++;
        m_name = "CanvasObject_"+std::to_string(m_objNameCounter);
    }
    m_updateControlls.enableUpdateLoop = true;
    m_updateControlls.enableEventLoop = true;
    m_updateControlls.enablePaintLoop = true;

    m_canvasParent = nullptr;
    m_parent = parent;
    m_objectsChanged = false;
    m_thisNeedsDrawUpdate = false;
    m_enabled = true;
    m_renderLayer = RenderLayer::layer_0;
}
CanvasObject::CanvasObject(const CanvasObject &other)
{
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    m_canvasParent = nullptr;
    m_parent = nullptr;
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

    for(size_t i=0; i<m_components.size(); ++i)
        delete m_components[i];
    m_components.clear();
    if(m_canvasParent)
        m_canvasParent->removeObject(this);
}

CanvasObject* CanvasObject::clone() const
{
    CanvasObject *obj = new CanvasObject(*this);
    return obj;
}
void CanvasObject::setParent(CanvasObject *parent)
{
    if(parent && parent != this && parent != m_parent)
    {
        parent->addChild_internal(this);
        return;
    }
    if(!parent)
    {
        m_parent->removeChild(this);
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

void CanvasObject::setName(const std::string &name)
{
    m_name = name;
}
const std::string CanvasObject::getName() const
{
    return m_name;
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

void CanvasObject::addChild(CanvasObject *child)
{
    if(!child)return;

    m_toAddChilds.push_back(child);
    onObjectsChanged();

}


sf::Vector2i CanvasObject::getMousePosition() const
{
    if(!m_canvasParent) return sf::Vector2i(0,0);
    return m_canvasParent->getMousePosition();
}
sf::Vector2f CanvasObject::getMouseWorldPosition() const
{
    if(!m_canvasParent) return sf::Vector2f(0,0);
    return m_canvasParent->getMouseWorldPosition();
}
sf::Vector2f CanvasObject::getInWorldSpace(const sf::Vector2i &pixelSpace) const
{
    if(!m_canvasParent) return sf::Vector2f(0,0);
    return m_canvasParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i CanvasObject::getInScreenSpace(const sf::Vector2f &worldSpace) const
{
    if(!m_canvasParent) return sf::Vector2i(0,0);
    return m_canvasParent->getInScreenSpace(worldSpace);
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
    /*size_t index = getChildIndex(child);
    if(index == npos) return;*/

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
void CanvasObject::deleteChilds()
{
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        bool match = false;
        for(size_t j=0; j<m_toDeleteChilds.size(); ++j)
            if(m_toDeleteChilds[j] == m_childs[i])
                match = true;
        if(!match)
            m_toDeleteChilds.push_back(m_childs[i]);
    }
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
void CanvasObject::removeComponent(Component *comp)
{
    if(!comp)return;
    /*size_t index = getComponentIndex(comp);
    if(index == npos) return;*/
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
void CanvasObject::removeComponent_internal()
{
    for(size_t i=0; i<m_toRemoveComponents.size(); ++i)
    {
        size_t index = getComponentIndex(m_toRemoveComponents[i]);
        if(index != npos)
            m_components.erase(m_components.begin() + index);

        SfEventHandle *evComp = dynamic_cast<SfEventHandle*>(m_toRemoveComponents[i]);
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
        Drawable *drawComp = dynamic_cast<Drawable*>(m_toRemoveComponents[i]);
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
    }
    m_toRemoveComponents.clear();
}
void CanvasObject::deleteComponent(Component *comp)
{
    if(!comp)return;
    size_t index = getComponentIndex(comp);
    if(index == npos) return;
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
        if(m_toAddComponents[i] == comp)
        {
            m_toAddComponents.erase(m_toAddComponents.begin() + i);
            break;
        }
    comp->setParent(nullptr);
    m_components.erase(m_components.begin() + index);
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
    delete comp;
}
/*void CanvasObject::deleteComponent_internal()
{
    for(size_t i=0; i<m_toDeleteComponents.size(); ++i)
    {
        size_t index = getComponentIndex(m_toDeleteComponents[i]);
        if(index != npos)
            m_components.erase(m_components.begin() + index);
        delete m_toDeleteComponents[i];
    }
    m_toDeleteComponents.clear();
}*/
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
    obj->setParent_internal(this, m_canvasParent);
}
void CanvasObject::setParent_internal(CanvasObject *parent, Canvas *canvasParent)
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
    setCanvasParent(canvasParent);
    for(size_t i=0; i<m_components.size(); ++i)
        m_components[i]->setParent(this);

   // for(size_t i=0; i<m_childs.size(); ++i)
   //     m_childs[i]->setCanvasParent(m_canvasParent);

    internalOnParentChange(oldParent, m_parent);
    onParentChange(oldParent, m_parent);
}
void CanvasObject::addComponent_internal()
{
    for(size_t i=0; i<m_toAddComponents.size(); ++i)
    {
        if(componentExists(m_toAddComponents[i]))
            continue;
        m_toAddComponents[i]->setParent(this);
        m_components.push_back(m_toAddComponents[i]);
        SfEventHandle* eventComp = dynamic_cast<SfEventHandle*>(m_toAddComponents[i]);
        if(eventComp)
        {
            m_eventComponents.push_back(eventComp);
            if(m_eventComponents.size() && !m_thisNeedsEventUpdate)
            {
                needsEventUpdate(true);
            }
        }

        Drawable* drawComp = dynamic_cast<Drawable*>(m_toAddComponents[i]);
        if(drawComp)
        {
            m_drawableComponents.push_back(drawComp);
            if(m_drawableComponents.size() && !m_thisNeedsDrawUpdate)
            {
                needsDrawUpdate(true);
            }
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
    for(size_t i=0; i<m_components.size(); ++i)
    {
        m_components[i]->setParent(nullptr);
        delete m_components[i];
    }
    m_eventComponents.clear();
    m_drawableComponents.clear();
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

size_t CanvasObject::getComponentCount() const
{
    return m_components.size();
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

void CanvasObject::update()
{

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

Canvas *CanvasObject::getCanvasParent() const
{
    return m_canvasParent;
}
void CanvasObject::deleteThis()
{
    if(m_parent)
    {
        m_parent->deleteChild(this);
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
    //for(size_t i=0; i<m_components.size(); ++i)
    //    m_components[i]->setParent(this);

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->setCanvasParent(parent);

    internalOnCanvasParentChange(oldParent, m_canvasParent);
    onCanvasParentChange(oldParent, m_canvasParent);
}
/*
void CanvasObject::updateNewElements()
{
    QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange));
    removeChild_internal();
    deleteChild_internal();
    removeComponent_internal();
    //deleteComponent_internal();
    addChild_internal();
    addComponent_internal();

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->updateNewElements();
    m_objectsChanged = false;
}
void CanvasObject::sfEvent(const std::vector<sf::Event> &events)
{
    if(!m_enabled || !m_updateControlls.enableEventLoop) return;
    QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange100));
    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components event", profiler::colors::Orange400));
    for(size_t i=0; i<m_eventComponents.size(); ++i)
    {
        if(!m_eventComponents[i]->isEnabled())
            continue;

        for(size_t j=0; j<events.size(); ++j)
            m_eventComponents[i]->sfEvent(events[j]);

    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs event", profiler::colors::Orange500));
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->sfEvent(events);
    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
}
void CanvasObject::update_internal()
{
    if(!m_enabled || !m_updateControlls.enableUpdateLoop) return;
    QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange600));
    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("This update", profiler::colors::Orange700));
    update();
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components update", profiler::colors::Orange800));
    for(size_t i=0; i<m_components.size(); ++i)
    {
        if(!m_components[i]->isEnabled())
            continue;
        m_components[i]->update();
    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs update", profiler::colors::Orange800));
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->update_internal();
    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
}*/
void CanvasObject::draw(sf::RenderWindow &window) const
{
    if(!m_enabled || !m_updateControlls.enablePaintLoop || !m_thisNeedsDrawUpdate) return;
    QSFML_PROFILE_CANVASOBJECT(EASY_FUNCTION(profiler::colors::Orange900));
    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Components draw", profiler::colors::OrangeA100));
    for(size_t i=0; i<m_drawableComponents.size(); ++i)
    {
        if(!m_drawableComponents[i]->isEnabled())
            continue;
        window.draw(*m_drawableComponents[i]);
    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);

    QSFML_PROFILE_CANVASOBJECT(EASY_BLOCK("Childs draw", profiler::colors::OrangeA200));
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->draw(window);
    }
    QSFML_PROFILE_CANVASOBJECT(EASY_END_BLOCK);
}

