#include "Planet.h"



QSFML::vector<Planet*> Planet::m_planets;
bool Planet::m_enableCollision = true;
sf::FloatRect Planet::m_worldBounds;
bool Planet::m_enableWorldBounds = false;

OBJECT_IMPL(Planet);
Planet::Planet(const std::string& name, Objects::GameObjectPtr parent)
	: GameObject(name, parent)
{
	m_planets.push_back(this);
	m_painter = new Painter("Painter", this);
	m_accelerationPainter = new Components::VectorPainter("AccelerationPainter");
	m_accelerationPainter->setColor(sf::Color::Red);

	addComponent(m_painter);
	addComponent(m_accelerationPainter);

	setMass(1);
}
Planet::~Planet()
{
	auto it = std::find(m_planets.begin(), m_planets.end(), this);
	if (it != m_planets.end())
		m_planets.erase(it);
}

void Planet::update()
{
	float dt = getFixedDeltaT();
	m_force = calculateForce();
	m_acceleration = m_force / m_mass;

	float magnitude = sqrt(m_acceleration.x * m_acceleration.x + m_acceleration.y * m_acceleration.y);
	if(magnitude > 1)
		m_acceleration = VectorMath::getNormalized(m_acceleration);

	m_velocity += m_acceleration * dt;
	m_nextPosition = getPosition() + m_velocity * dt;

	if (!m_enableWorldBounds)
		return;

	// Wrap around
	if (m_worldBounds.width > 0 && m_worldBounds.height > 0)
	{
		if (m_nextPosition.x < m_worldBounds.left)
			m_nextPosition.x = m_worldBounds.left + m_worldBounds.width;
		else if (m_nextPosition.x > m_worldBounds.left + m_worldBounds.width)
			m_nextPosition.x = m_worldBounds.left;
		if (m_nextPosition.y < m_worldBounds.top)
			m_nextPosition.y = m_worldBounds.top + m_worldBounds.height;
		else if (m_nextPosition.y > m_worldBounds.top + m_worldBounds.height)
			m_nextPosition.y = m_worldBounds.top;
	}
}
void Planet::applyVelocity()
{
	sf::Vector2f direction = m_acceleration;
	float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude <= 10)
	{
		direction = VectorMath::getNormalized(direction) * 10.f;
	}
	m_accelerationPainter->setDirection(direction);
	setPosition(m_nextPosition);
}
void Planet::setMass(float mass)
{
	if(mass <= 0)
		mass = 0.1;
	m_mass = mass;
	m_radius = pow(mass, 1.0 / 3.0)*10;
}
sf::Vector2f Planet::calculateForce(const sf::Vector2f& position)
{
	sf::Vector2f force(0, 0);
	for (auto planet : m_planets)
	{
		sf::Vector2f direction = planet->getPosition() - position;
		float distanceSqr = direction.x * direction.x + direction.y * direction.y;
		float distance = sqrt(distanceSqr);
		if (distance <= std::numeric_limits<float>::epsilon())
			continue;
		
		float forceMagnitude = (G * planet->m_mass) / (distanceSqr);
		force += direction / distance * forceMagnitude;
	}
	return force;
}
sf::Vector2f Planet::calculateForce()
{
	sf::Vector2f force(0, 0);
	for (auto planet : m_planets)
	{
		if (planet == this)
			continue;
		force += calculateForce(planet);
	}
	return force;
}
sf::Vector2f Planet::calculateForce(Planet* other)
{
	sf::Vector2f direction = other->getPosition() - getPosition();
	float distanceSqr = direction.x * direction.x + direction.y * direction.y;
	float distance = sqrt(distanceSqr);

	// check collision
	if (m_enableCollision)
	{
		if (distance < m_radius + other->m_radius)
		{
			sf::Vector2f normal = VectorMath::getNormalized(direction);
			sf::Vector2f tangent = VectorMath::getRotatedRAD(normal, M_PI_2);
			sf::Vector2f relativeVelocity = m_velocity - other->m_velocity;
			float normalVelocity = VectorMath::dotProduct(relativeVelocity, normal);
			float tangentVelocity = VectorMath::dotProduct(relativeVelocity, tangent);
			float normalForce = -normalVelocity;
			sf::Vector2f normalForceVector = normal * normalForce;
			sf::Vector2f tangentForceVector = tangent * tangentVelocity;
			m_velocity += normalForceVector;
			other->m_velocity -= normalForceVector;
			return { 0,0 };
		}
	}
	

	if(distance <= std::numeric_limits<float>::epsilon())
		return sf::Vector2f(0, 0);
	float forceMagnitude = (G * m_mass * other->m_mass) / (distanceSqr);
	sf::Vector2f force = direction / distance * forceMagnitude;
	return force;
}