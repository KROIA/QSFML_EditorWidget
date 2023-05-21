#include "components/physics/Collider.h"
#include "objects/CanvasObject.h"
#include "utilities/VectorOperations.h"
#include "utilities/Ray.h"
#include "canvas/Stats.h"

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(Collider)
Collider::Collider(const std::string &name)
    : Component(name)
    , m_vertices({sf::Vector2f(0,0), sf::Vector2f(1,0), sf::Vector2f(1,1), sf::Vector2f(0,1)})
    , m_pos(sf::Vector2f(0, 0))
{
    calculateBoundingBox();
}
Collider::Collider(const Collider& other)
    : Component(other)
    , m_vertices(other.m_vertices)
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
    QSFMLP_FUNCTION(QSFMLP_PHYSICS_COLOR_1);
    Objects::CanvasObject* thisRootParent = getParent()->getRootParent();
    for (auto obj : objs)
    {
        if(obj->isEnabled())
            checkCollision(obj->getCollider(), collisions, onlyFirstCollisionPerObject);
    }
}
void Collider::checkCollision(const std::vector<Components::Collider*>& other,
    std::vector<Collisioninfo>& collisions,
    bool onlyFirstCollisionPerObject) const
{
    QSFMLP_FUNCTION(QSFMLP_PHYSICS_COLOR_2);
    for (auto otherCollider : other)
    {
        checkCollision(otherCollider, collisions, onlyFirstCollisionPerObject);
    }
}


bool Collider::checkCollision(Collider* other, std::vector<Collisioninfo>& collisions, bool onlyFirstCollision) const
{ 
    if (!other)
        return false;
    Canvas* canvasParent = getCanvasParent();
    StatsManager::addBoundingBoxCollisionCheck(canvasParent);
    if (!m_boundingBox.intersects(other->m_boundingBox))
    {
        return false;
    }
    QSFMLP_FUNCTION(QSFMLP_PHYSICS_COLOR_3);

    

    size_t thisNextVertexIndex;
    size_t otherNextVertexIndex;
    bool collision = false;
    
    size_t currentCollisionCount = collisions.size();
    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        thisNextVertexIndex = (i + 1) % m_vertices.size();
        Utilities::Ray thisRay(m_vertices[i]+m_pos, m_vertices[thisNextVertexIndex] - m_vertices[i]);
        for (size_t o = 0; o < other->m_vertices.size(); ++o)
        {
            otherNextVertexIndex = (o + 1) % other->m_vertices.size();
            Utilities::Ray otherRay(other->m_vertices[o]+other->m_pos, other->m_vertices[otherNextVertexIndex] - other->m_vertices[o]);
        
            float thisDistance;
            float otherDistance;
            if (thisRay.raycast(otherRay, thisDistance, otherDistance))
            {
                if (thisDistance >= 0 && thisDistance <= 1 && otherDistance >= 0 && otherDistance <= 1)
                {
                    collision = true;
                    sf::Vector2f collisionPoint = otherRay.getPoint(otherDistance);
                    Collisioninfo info;
                    info.collider1 = this;
                    info.collider2 = other;
                    info.collisionPos = collisionPoint;
                    info.vertexIndex.index1 = i;
                    info.vertexIndex.index1 = o;
                    collisions.push_back(info);

                    if (onlyFirstCollision)
                    {
                        
                        StatsManager::addCollisionCheck(canvasParent, i*o);
                        StatsManager::addCollision(canvasParent);
                        return true;
                    }
                }
            }
        }
    }
    StatsManager::addCollisionCheck(canvasParent, m_vertices.size() * other->m_vertices.size());
    StatsManager::addCollision(canvasParent, collisions.size() - currentCollisionCount);

    
    return collision;
}
bool Collider::contains(const sf::Vector2f& point)
{
    QSFMLP_FUNCTION(QSFMLP_PHYSICS_COLOR_1);
    
    StatsManager::addBoundingBoxCollisionCheck(getCanvasParent());
    if (!m_boundingBox.contains(point))
        return false;

    bool result = contains(m_vertices, point, m_pos);
    Canvas* canvasParent = getCanvasParent();
    if(result)
        StatsManager::addCollision(canvasParent);
    StatsManager::addCollisionCheck(canvasParent, m_vertices.size());
    return result;
}
bool Collider::contains(const std::vector<sf::Vector2f>& polygon, 
                        const sf::Vector2f& point,
                        const sf::Vector2f& polygonPos)
{
    QSFMLP_FUNCTION(QSFMLP_PHYSICS_COLOR_2);
    // Get the number of vertices in the polygon
    std::size_t numVertices = polygon.size();

    // Initialize the crossing count
    int crossingCount = 0;

    // Iterate over each edge of the polygon
    for (std::size_t i = 0; i < numVertices; ++i) {
        const sf::Vector2f& vertex1 = polygon[i] + polygonPos;
        const sf::Vector2f& vertex2 = polygon[(i + 1) % numVertices] + polygonPos;

        // Check if the ray from the test point crosses the edge
        if (((vertex1.y > point.y) != (vertex2.y > point.y)) &&
            (point.x < (vertex2.x - vertex1.x) * (point.y - vertex1.y) / (vertex2.y - vertex1.y) + vertex1.x)) {
            crossingCount++;
        }
    }
    

    // If the number of edge crossings is odd, the point is inside the polygon
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
void Collider::resolveCollision(Collider* other)
{
    move(resolveCollision(m_vertices, other->m_vertices));
    calculateBoundingBox();
}
void Collider::resolveCollision(const std::vector<Collider*>& other)
{
    for (auto& collider : other)
        move(resolveCollision(m_vertices, collider->m_vertices));
    calculateBoundingBox();
}
// CHAT GPT

bool Collider::isSeparatingAxis(const std::vector<sf::Vector2f>& polygonA, const std::vector<sf::Vector2f>& polygonB, const sf::Vector2f& axis) {
    float minA = std::numeric_limits<float>::max();
    float maxA = std::numeric_limits<float>::min();
    float minB = std::numeric_limits<float>::max();
    float maxB = std::numeric_limits<float>::min();

    // Project polygon A onto the axis
    for (const sf::Vector2f& point : polygonA) {
        float projection = axis.x * point.x + axis.y * point.y;
        minA = std::min(minA, projection);
        maxA = std::max(maxA, projection);
    }

    // Project polygon B onto the axis
    for (const sf::Vector2f& point : polygonB) {
        float projection = axis.x * point.x + axis.y * point.y;
        minB = std::min(minB, projection);
        maxB = std::max(maxB, projection);
    }

    // Check for separation
    if (maxA < minB || maxB < minA) {
        return true; // Separating axis found
    }

    return false; // No separating axis found
}

sf::Vector2f Collider::getMinimumTranslationVector(const std::vector<sf::Vector2f>& polygonA, const std::vector<sf::Vector2f>& polygonB, const sf::Vector2f& axis) {
    float minOverlap = std::numeric_limits<float>::max();
    sf::Vector2f minOverlapAxis;

    // Project polygon A onto the axis
    float minA = std::numeric_limits<float>::max();
    float maxA = std::numeric_limits<float>::min();
    for (const sf::Vector2f& point : polygonA) {
        float projection = axis.x * point.x + axis.y * point.y;
        minA = std::min(minA, projection);
        maxA = std::max(maxA, projection);
    }

    // Project polygon B onto the axis
    float minB = std::numeric_limits<float>::max();
    float maxB = std::numeric_limits<float>::min();
    for (const sf::Vector2f& point : polygonB) {
        float projection = axis.x * point.x + axis.y * point.y;
        minB = std::min(minB, projection);
        maxB = std::max(maxB, projection);
    }

    // Calculate the overlap
    float overlap = std::min(maxA, maxB) - std::max(minA, minB);

    if (overlap < minOverlap) {
        minOverlap = overlap;
        minOverlapAxis = axis;
    }

    return minOverlap * minOverlapAxis;
}

sf::Vector2f Collider::normalize(const sf::Vector2f& vector) {
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    return sf::Vector2f(vector.x / length, vector.y / length);
}

sf::Vector2f Collider::resolveCollision(std::vector<sf::Vector2f>& polygonA,  std::vector<sf::Vector2f>& polygonB) {
    // Check for an early exit: bounding box intersection
    sf::FloatRect rectA = sf::FloatRect(polygonA[0], polygonA[0]);
    sf::FloatRect rectB = sf::FloatRect(polygonB[0], polygonB[0]);
    for (const sf::Vector2f& point : polygonA) {
        rectA.left = std::min(rectA.left, point.x);
        rectA.top = std::min(rectA.top, point.y);
        rectA.width = std::max(rectA.width, point.x);
        rectA.height = std::max(rectA.height, point.y);
    }
    for (const sf::Vector2f& point : polygonB) {
        rectB.left = std::min(rectB.left, point.x);
        rectB.top = std::min(rectB.top, point.y);
        rectB.width = std::max(rectB.width, point.x);
        rectB.height = std::max(rectB.height, point.y);
    }

    if (!rectA.intersects(rectB))
        return sf::Vector2f(0.f, 0.f); // No collision

    // Convert the polygons to a list of edges
    std::vector<sf::Vector2f> edgesA;
    std::vector<sf::Vector2f> edgesB;
    for (size_t i = 0; i < polygonA.size(); ++i) {
        sf::Vector2f edge = polygonA[(i + 1) % polygonA.size()] - polygonA[i];
        edgesA.push_back(normalize(sf::Vector2f(-edge.y, edge.x))); // Calculate the normal vector (perpendicular)
    }
    for (size_t i = 0; i < polygonB.size(); ++i) {
        sf::Vector2f edge = polygonB[(i + 1) % polygonB.size()] - polygonB[i];
        edgesB.push_back(normalize(sf::Vector2f(-edge.y, edge.x))); // Calculate the normal vector (perpendicular)
    }

    // Check for separation along the normal vectors (SAT)
    for (const sf::Vector2f& axis : edgesA) {
        if (isSeparatingAxis(polygonA, polygonB, axis))
            return sf::Vector2f(0.f, 0.f); // No collision
    }
    for (const sf::Vector2f& axis : edgesB) {
        if (isSeparatingAxis(polygonA, polygonB, axis))
            return sf::Vector2f(0.f, 0.f); // No collision
    }

    // Find the minimum translation vector (MTV)
    sf::Vector2f minTranslationVector;
    float minOverlap = std::numeric_limits<float>::max();
    for (const sf::Vector2f& axis : edgesA) {
        sf::Vector2f translationVector = getMinimumTranslationVector(polygonA, polygonB, axis);
        float overlap = std::sqrt(translationVector.x * translationVector.x + translationVector.y * translationVector.y);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minTranslationVector = translationVector;
        }
    }
    for (const sf::Vector2f& axis : edgesB) {
        sf::Vector2f translationVector = getMinimumTranslationVector(polygonA, polygonB, axis);
        float overlap = std::sqrt(translationVector.x * translationVector.x + translationVector.y * translationVector.y);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minTranslationVector = translationVector;
        }
    }

    // Separate the polygons
    //for (sf::Vector2f& point : polygonA) {
    //    point += minTranslationVector;
    //}

    return minTranslationVector;
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
        coll[m_collider->m_vertices.size()] = sf::Vertex(m_collider->m_vertices[0] + colliderPos, m_colliderColor);
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
