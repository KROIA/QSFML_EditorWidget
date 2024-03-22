#include "utilities/Stats.h"

#include "objects/base/CanvasObject.h"

#include "components/physics/Collider.h"

#include "utilities/VectorOperations.h"
#include "utilities/Ray.h"
#include "Canvas/Canvas.h"

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(Collider)
Collider::Collider(const std::string &name)
    : Component(name)
    , m_relativeVertices({sf::Vector2f(0,0), sf::Vector2f(1,0), sf::Vector2f(1,1), sf::Vector2f(0,1)})
    , m_pos(sf::Vector2f(0, 0))
{
    calculateBoundingBox();
}
Collider::Collider(const Collider& other)
    : Component(other)
    , m_absoluteVertices(other.m_absoluteVertices)
    , m_relativeVertices(other.m_relativeVertices)
    , m_pos(other.m_pos)
    , m_boundingBox(other.m_boundingBox)
{
    
}
Collider::~Collider()
{
    for (size_t i = 0; i < m_painters.size(); ++i)
        m_painters[i]->onColliderDelete();
}

Collider& Collider::operator=(const Collider& other)
{
    m_absoluteVertices = other.m_absoluteVertices;
    m_relativeVertices = other.m_relativeVertices;
    m_pos = other.m_pos;
    m_boundingBox = other.m_boundingBox;
    return *this;
}



void Collider::setVertecies(const std::vector<sf::Vector2f>& vertecies)
{
    m_relativeVertices = vertecies;
    calculateAbsPos();
    calculateBoundingBox();
}
void Collider::addVertex(const sf::Vector2f& vertex)
{
    m_relativeVertices.push_back(vertex);
    m_absoluteVertices.push_back(vertex + m_pos);
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
    return m_relativeVertices.size();
}
const std::vector<sf::Vector2f>& Collider::getVertecies() const
{
    return m_relativeVertices;
}
const Utilities::AABB& Collider::getBoundingBox() const
{
    return m_boundingBox;
}

void Collider::clear()
{
    m_relativeVertices.clear();
    m_absoluteVertices.clear();
}
void Collider::setPos(const sf::Vector2f& pos)
{
    m_boundingBox.move(pos - m_pos);
    m_pos = pos;
    calculateAbsPos();
    if (getParent())
        getParent()->updateBoundingBox();
}
void Collider::move(const sf::Vector2f& offset)
{
    m_boundingBox.move(offset);
    m_pos += offset;
    calculateAbsPos();
    if (getParent())
        getParent()->updateBoundingBox();
}
const sf::Vector2f& Collider::getPos() const
{
    return m_pos;
}

bool Collider::checkCollision(const std::vector<Objects::CanvasObject*>& objs,
    std::vector<Utilities::Collisioninfo>& collisions,
    bool onlyFirstCollisionPerObject) const
{
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_1);
    Objects::CanvasObject* thisRootParent = getParent()->getRootParent();
    bool hasCollision = false;
    for (auto obj : objs)
    {
        if(obj->isEnabled())
            hasCollision |= checkCollision(obj->getCollider(), collisions, onlyFirstCollisionPerObject);
    }
    return hasCollision;
}
bool Collider::checkCollision(const std::vector<Components::Collider*>& other,
    std::vector<Utilities::Collisioninfo>& collisions,
    bool onlyFirstCollisionPerObject) const
{
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_2);
    bool hasCollision = false;
    for (auto otherCollider : other)
    {
        hasCollision |= checkCollision(otherCollider, collisions, onlyFirstCollisionPerObject);
    }
    return hasCollision;
}


bool Collider::checkCollision(Collider* other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision) const
{ 
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_3);
    if(m_canvasParent)
        m_canvasParent->addBoundingBoxCollisionCheck();
    QSFMLP_PHYSICS_BLOCK("AABB check", QSFML_COLOR_STAGE_4);
    if (!m_boundingBox.intersects(other->m_boundingBox))
    {
        QSFML_PROFILING_END_BLOCK;
        return false;
    }
    QSFML_PROFILING_END_BLOCK;
    
    return checkCollision_noAABB(other, collisions, onlyFirstCollision);
}
void Collider::checkCollision_noAABB(const std::vector<Components::Collider*>& other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision) const
{
    for (auto otherCollider : other)
        checkCollision_noAABB(otherCollider, collisions, onlyFirstCollision);
}
bool Collider::checkCollision_noAABB(Collider* other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision) const
{
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_3);
    
#define FAST_COLLISION_CHECK

    size_t thisNextVertexIndex;
    size_t otherNextVertexIndex;
    bool collision = false;

    size_t currentCollisionCount = collisions.size();
    for (size_t i = 0; i < m_absoluteVertices.size(); ++i)
    {
        thisNextVertexIndex = (i + 1) % m_absoluteVertices.size();
        sf::Vector2f start1 = m_absoluteVertices[i];
        sf::Vector2f end1 = m_absoluteVertices[thisNextVertexIndex];
#ifndef FAST_COLLISION_CHECK
        Utilities::Ray thisRay(start1, end1 - start1);
#endif
        for (size_t o = 0; o < other->m_absoluteVertices.size(); ++o)
        {
            otherNextVertexIndex = (o + 1) % other->m_absoluteVertices.size();
            sf::Vector2f start2 = other->m_absoluteVertices[o];
            sf::Vector2f end2 = other->m_absoluteVertices[otherNextVertexIndex];

            float thisDistance;
            float otherDistance;
#ifndef FAST_COLLISION_CHECK
            Utilities::Ray otherRay(start2, end2 - start2);

            if (thisRay.raycast(otherRay, thisDistance, otherDistance))
            {
                if (thisDistance >= 0 && thisDistance <= 1 && otherDistance >= 0 && otherDistance <= 1)
                {
                    collision = true;
                    sf::Vector2f collisionPoint = otherRay.getPoint(otherDistance);
                    Utilities::Collisioninfo info;
                    info.collider1 = this;
                    info.collider2 = other;
                    info.collisionPos = collisionPoint;
                    info.vertexIndex.index1 = i;
                    info.vertexIndex.index1 = o;
                    collisions.push_back(info);

                    if (onlyFirstCollision)
                    {

                        StatsManager::addCollisionCheck(canvasParent, i * o);
                        StatsManager::addCollision(canvasParent);
                        return true;
                    }
                }
            }
#else
            
            if (doLineSegmentsIntersect(start1, end1, start2, end2, thisDistance, otherDistance))
            {
                collision = true;
                sf::Vector2f collisionPoint = start1 + (end1 - start1) * thisDistance;
                Utilities::Collisioninfo info;
                info.collider1 = this;
                info.collider2 = other;
                info.collisionPos = collisionPoint;
                info.vertexIndex.index1 = i;
                info.vertexIndex.index2 = o;
                collisions.push_back(info);

                if (onlyFirstCollision)
                {

                    if (m_canvasParent)
                    {
                        m_canvasParent->addCollisionCheck(i * o);
                        m_canvasParent->addCollision();
                    }
                    return true;
                }
            }
#endif
            
            
        }
    }
    if (m_canvasParent)
    {
        m_canvasParent->addCollisionCheck(m_absoluteVertices.size() * other->m_absoluteVertices.size());
        m_canvasParent->addCollision(collisions.size() - currentCollisionCount);
    }

    return collision;
}

bool Collider::doLineSegmentsIntersect(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, float& scalar1, float& scalar2)
{
    // Calculate the directional vectors of the line segments
    /*sf::Vector2f dirP0P1 = p1 - p0;
    sf::Vector2f dirP2P3 = p3 - p2;

    // Calculate the determinant of the matrix formed by the directional vectors
    float determinant = dirP0P1.x * dirP2P3.y - dirP0P1.y * dirP2P3.x;

    // If the determinant is close to zero, the lines are parallel or collinear
    if (std::abs(determinant) < 1e-6)
        return false;

    // Calculate the vector between the starting points of the line segments
    sf::Vector2f p0P2 = p2 - p0;

    // Calculate the parameters t and u for the intersection point
    scalar1 = (p0P2.x * dirP2P3.y - p0P2.y * dirP2P3.x) / determinant;
    scalar2 = (p0P2.x * dirP0P1.y - p0P2.y * dirP0P1.x) / determinant;
    */

    sf::Vector2f s1 = p1 - p0;
    sf::Vector2f s2 = p3 - p2;

    // Calculate the determinant of the matrix formed by the directional vectors
    float determinant = (s2.x * s1.y - s1.x * s2.y);
    // If the determinant is close to zero, the lines are parallel or collinear
    if (std::abs(determinant) < 1e-6)
        return false;

    determinant = 1 / determinant;
    scalar1 = (p0.x * s2.y - p0.y * s2.x - p2.x * s2.y + p2.y * s2.x) * determinant;
    scalar2 = (p0.x * s1.y - p0.y * s1.x + s1.x * p2.y - s1.y * p2.x) * determinant;

    //scalar1 = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / determinant;
    //scalar2 = (s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x))  / determinant;


    // If t and u are within [0, 1], the line segments intersect
    return scalar1 >= 0.0 && scalar1 <= 1.0 && scalar2 >= 0.0 && scalar2 <= 1.0;
}
bool Collider::contains(const sf::Vector2f& point)
{
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_1);
    
    if(m_canvasParent)
        m_canvasParent->addBoundingBoxCollisionCheck();
    if (!m_boundingBox.contains(point))
        return false;

    bool result = contains(m_absoluteVertices, point);
    if (m_canvasParent)
    {
        if (result)
            m_canvasParent->addCollision();
        m_canvasParent->addCollisionCheck(m_relativeVertices.size());
    }
    return result;
}
bool Collider::contains(const std::vector<sf::Vector2f>& polygon, 
                        const sf::Vector2f& point)
{
    QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_2);
    // Get the number of vertices in the polygon
    std::size_t numVertices = polygon.size();

    // Initialize the crossing count
    int crossingCount = 0;

    // Iterate over each edge of the polygon
    for (std::size_t i = 0; i < numVertices; ++i) {
        const sf::Vector2f& vertex1 = polygon[i];
        const sf::Vector2f& vertex2 = polygon[(i + 1) % numVertices];

        // Check if the ray from the test const sf::Vector2f & crosses the edge
        if (((vertex1.y > point.y) != (vertex2.y > point.y)) &&
            (point.x < (vertex2.x - vertex1.x) * (point.y - vertex1.y) / (vertex2.y - vertex1.y) + vertex1.x)) {
            crossingCount++;
        }
    }
    

    // If the number of edge crossings is odd, the const sf::Vector2f & is inside the polygon
    return (crossingCount % 2) == 1;
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
    m_boundingBox = Utilities::AABB::getFrame(m_relativeVertices);
    m_boundingBox.move(m_pos);
    if (getParent())
        getParent()->updateBoundingBox();
}
void Collider::calculateAbsPos()
{
    m_absoluteVertices.clear();
    m_absoluteVertices.reserve(m_relativeVertices.size());
    for (size_t i = 0; i < m_relativeVertices.size(); ++i)
        m_absoluteVertices.push_back(m_relativeVertices[i] + m_pos);
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
    : Drawable(name)
    , m_collider(collider)
{

}
Collider::Painter::Painter(const Painter& other)
    : Drawable(other)
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
void Collider::Painter::drawComponent(sf::RenderTarget& target,
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
    size_t certexCount = m_collider->m_absoluteVertices.size();
    if (certexCount > 0)
    {
        sf::Vertex* coll = new sf::Vertex[certexCount + 1];
        for (size_t i = 0; i < certexCount; ++i)
        {
            coll[i] = sf::Vertex(m_collider->m_absoluteVertices[i], m_colliderColor);
        }
        coll[m_collider->m_relativeVertices.size()] = sf::Vertex(m_collider->m_absoluteVertices[0], m_colliderColor);
        target.draw(coll, certexCount+1, sf::LineStrip);
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
    m_collider = nullptr;
    setEnabled(false);
}
}
}
