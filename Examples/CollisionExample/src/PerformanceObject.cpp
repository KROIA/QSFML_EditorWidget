#include "PerformanceObject.h"

CLONE_FUNC_IMPL(PerformanceObject);
PerformanceObject::PerformanceObject(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
	, m_dynamic(true)
{
	m_collider = new QSFML::Components::Collider();
	

	size_t verteciesCount = 3 + (rand() % 50);
	float radius = QSFML::Utilities::RandomEngine::getFloat(2, 10);
	QSFML::vector<sf::Vector2f> vertecies;
	vertecies.reserve(verteciesCount);
	float rad = 0;
	float dRad = M_PI * 2 / (float)verteciesCount;
	for (size_t i = 0; i < verteciesCount; ++i)
	{
		sf::Vector2f rotated(sin(rad) * radius, cos(rad) * radius);
		sf::Vector2f randomVec = QSFML::Utilities::RandomEngine::getVector() * radius;

		vertecies.push_back(rotated + randomVec);
		rad += dRad;
	}
	m_currentDirection = QSFML::Utilities::RandomEngine::getFloat(-M_PI, M_PI);

	m_collider->setVertecies(vertecies);

	addComponent(m_collider);
	addComponent(m_collider->createPainter());
}
PerformanceObject::PerformanceObject(const QSFML::vector<sf::Vector2f>& vertecies, const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
	, m_dynamic(true)
{
	m_collider = new QSFML::Components::Collider();
	m_currentDirection = QSFML::Utilities::RandomEngine::getFloat(-M_PI, M_PI);

	m_collider->setVertecies(vertecies);
	
	addComponent(m_collider);
	addComponent(m_collider->createPainter());
	
}
PerformanceObject::~PerformanceObject()
{

}
void PerformanceObject::setRange(const QSFML::Utilities::AABB& range)
{
	m_range = range;
}
void PerformanceObject::setDynamic(bool isDynamic)
{
	m_dynamic = isDynamic;
}
void PerformanceObject::update()
{
	if (!m_dynamic)
		return;
	const float maxVelocity = 100;
	float deltaT = getDeltaT();
	float deltaDir = QSFML::Utilities::RandomEngine::getFloat(-0.1, 0.1);
	m_currentDirection += deltaDir;
	//sf::Vector2f deltaV = m_acceletation * deltaT;
	sf::Vector2f deltaV = QSFML::VectorMath::getRotatedRAD(sf::Vector2f(1,0), sf::Vector2f(0,0), m_currentDirection);

	m_velocity += deltaV;
	if (QSFML::VectorMath::getLength(m_velocity) > maxVelocity)
	{
		m_velocity = QSFML::VectorMath::getNormalized(m_velocity) * maxVelocity;
	}
	sf::Vector2f deltaPos = m_velocity * deltaT;
	sf::Vector2f pos = getPosition() + deltaPos;
	if (!m_range.contains(pos))
	{
		m_velocity = -m_velocity;
		pos = getPosition() - deltaPos;
	}
	setPosition(pos);
}
