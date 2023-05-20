#include "components/physics/Collider.h"
#include "objects/CanvasObject.h"
#include "utilities/VectorOperations.h"


namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(Collider)
Collider::Collider(const std::string &name)
    :   Component(name)
{

}
Collider::Collider(const Collider &other)
    :   Component(other)
    ,   m_vertices(other.m_vertices)
    ,   m_pos(other.m_pos)
{
    calculateBoundingBox();
}
Collider::~Collider()
{
    for (size_t i = 0; i < m_painters.size(); ++i)
        m_painters[i]->onColliderDelete();
}

Collider& Collider::operator=(const Collider& other)
{
    m_vertices = other.m_vertices;
    m_pos = other.m_pos;
    m_boundingBox = other.m_boundingBox;
    return *this;
}

void Collider::setVertecies(const std::vector<sf::Vector2f>& vertecies)
{
    m_vertices = vertecies;
    calculateBoundingBox();
}
void Collider::addVertex(const sf::Vector2f& vertex)
{
    m_vertices.push_back(vertex);
    if (m_boundingBox.contains(vertex))
        return;

    if (vertex.x > m_boundingBox.BR().x)
        m_boundingBox.setWidth(vertex.x - m_boundingBox.getPos().x);
    if (vertex.y > m_boundingBox.BR().y)
        m_boundingBox.setHeight(vertex.y - m_boundingBox.getPos().y);

    if (vertex.x < m_boundingBox.getPos().x)
    {
        float dif = m_boundingBox.getPos().x - vertex.x;
        m_boundingBox.setWidth(m_boundingBox.BR().x + dif);
        m_boundingBox.setX(vertex.x);
    }
    if (vertex.y < m_boundingBox.getPos().y)
    {
        float dif = m_boundingBox.getPos().y - vertex.y;
        m_boundingBox.setWidth(m_boundingBox.BR().y + dif);
        m_boundingBox.setY(vertex.y);
    }

}
size_t Collider::getVertexCount() const
{
    return m_vertices.size();
}
const std::vector<sf::Vector2f>& Collider::getVertecies() const
{
    return m_vertices;
}

void Collider::clear()
{
    m_vertices.clear();
}
void Collider::setPos(const sf::Vector2f& pos)
{
    m_boundingBox.move(pos - m_pos);
    m_pos = pos;
}
void Collider::move(const sf::Vector2f& offset)
{
    m_boundingBox.move(offset);
    m_pos += offset;
}
const sf::Vector2f& Collider::getPos() const
{
    return m_pos;
}

void Collider::checkCollision(const std::vector<Objects::CanvasObject*>& objs,
    std::vector<Collisioninfo>& collisions,
    bool onlyFirstCollisionPerObject) const
{

    collisions.clear();
    Objects::CanvasObject* thisRootParent = getParent()->getRootParent();
    for (size_t i = 0; i < objs.size(); ++i)
    {
        checkCollision(objs[i], collisions, onlyFirstCollisionPerObject);
    }
}
void Collider::checkCollision(const Objects::CanvasObject* obj,
    std::vector<Collisioninfo>& collisions,
    bool onlyFirstCollisionPerObject) const
{
    if (!obj->isEnabled())
        return;
    //if(thisRootParent == obj1->getRootParent())
    //    continue; // Don't check collision on objects which are
    //              // contained within the same root object.
    std::vector<Components::Collider*> coll = obj->getCollider();
    for (size_t j = 0; j < coll.size(); ++j)
        checkCollision(coll[j], collisions, onlyFirstCollisionPerObject);
}
bool Collider::checkCollision(Collider* other, std::vector<Collisioninfo>& collisions, bool onlyFirstCollision) const
{ 
    if (!other)
        return false;
    if (!m_boundingBox.intersects(other->m_boundingBox))
        return false;

    Collisioninfo info;
    info.collider1 = this;
    info.collider2 = other;
    collisions.push_back(info);
    return true;
}
Collider::Painter* Collider::createPainter()
{
    Painter *p = new Painter(this, getName() + " Painter");
    p->setColorAABB(sf::Color(200, 0, 0));
    p->setColorCollider(sf::Color(0, 200, 0));
    m_painters.push_back(p);
    return p;
}

void Collider::calculateBoundingBox()
{
    m_boundingBox = Utilities::AABB::getFrame(m_vertices);
    m_boundingBox.move(m_pos);
}
void Collider::onPainterDeleted(Painter* p)
{
    for (size_t i = 0; i < m_painters.size(); ++i)
    {
        if (m_painters[i] == p)
        {
            m_painters.erase(m_painters.begin() + i);
            return;
        }
    }
}


COMPONENT_IMPL(Collider::Painter);
Collider::Painter::Painter(Collider* collider, const std::string& name)
    : /*QObject()
    , */Drawable(name)
    , m_collider(collider)
{

}
Collider::Painter::Painter(const Painter& other)
    : /*QObject()
    , */Drawable(other)
    , m_collider(other.m_collider)
{
    if (m_collider)
        m_collider->m_painters.push_back(this);
}
Collider::Painter::~Painter()
{
    if (m_collider)
        m_collider->onPainterDeleted(this);
}
void Collider::Painter::draw(sf::RenderTarget& target,
                             sf::RenderStates states) const
{
    if (!m_collider)
        return;

    sf::Vertex aabb[] =
    {
        sf::Vertex(m_collider->m_boundingBox.TL(), m_aabbColor),
        sf::Vertex(m_collider->m_boundingBox.TR(), m_aabbColor),
        sf::Vertex(m_collider->m_boundingBox.BR(), m_aabbColor),
        sf::Vertex(m_collider->m_boundingBox.BL(), m_aabbColor),
        sf::Vertex(m_collider->m_boundingBox.TL(), m_aabbColor),
    };
    size_t certexCount = m_collider->m_vertices.size();
    sf::Vector2f colliderPos = m_collider->m_pos;
    if (certexCount > 0)
    {
        sf::Vertex* coll = new sf::Vertex[certexCount + 1];
        for (size_t i = 0; i < certexCount; ++i)
        {
            coll[i] = sf::Vertex(m_collider->m_vertices[i] + colliderPos, m_colliderColor);
        }
        coll[m_collider->m_vertices.size()] = sf::Vertex(m_collider->m_vertices[0], m_colliderColor);
        target.draw(coll, certexCount, sf::LineStrip);
        delete[] coll;
    }
    target.draw(aabb, 5, sf::LineStrip);
}
void Collider::Painter::setColor(const sf::Color& color)
{
    int colorOffset = 50;
    int r = (int)color.r + colorOffset;
    int g = (int)color.g + colorOffset;
    int b = (int)color.b + colorOffset;

    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    m_aabbColor = sf::Color(r, g, b);
    m_colliderColor = color;
}
void Collider::Painter::setColorAABB(const sf::Color& color)
{
    m_aabbColor = color;
}
const sf::Color& Collider::Painter::getColorAABB() const
{
    return m_aabbColor;
}
void Collider::Painter::setColorCollider(const sf::Color& color)
{
    m_colliderColor = color;
}
const sf::Color& Collider::Painter::getColorCollider() const
{
    return m_colliderColor;
}
const sf::Color& Collider::Painter::getColor() const
{
    return m_colliderColor;
}
void Collider::Painter::onColliderDelete()
{
    //emit colliderDeleted(m_collider);
    m_collider = nullptr;
    setEnabled(false);
}
}
}
