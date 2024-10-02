#pragma once

#include "QSFML_EditorWidget.h"
using namespace QSFML;

class Planet : public Objects::GameObject
{
	friend class Painter;
public:
	OBJECT_DECL(Planet);
	Planet(const std::string& name = "Planet", Objects::GameObjectPtr parent = nullptr);
	~Planet();

	void update() override;
	void applyVelocity();

	void setVelocity(const sf::Vector2f &velocity) { m_velocity = velocity; }
	void setMass(float mass);
	const sf::Vector2f &getVelocity() const { return m_velocity; }

	static const std::vector<Planet*> &getPlanets() { return m_planets; }

	static sf::Vector2f calculateGravityPotential(const sf::Vector2f &position);
	static void setEnableCollision(bool enable) { m_enableCollision = enable; }
	static void setWorldBounds(const sf::FloatRect &bounds) { m_worldBounds = bounds; }
private:
	sf::Vector2f calculateForce();
	sf::Vector2f calculateForce(Planet *other);


	class Painter : public Components::Drawable
	{
	public:
		//COMPONENT_DECL(Painter);

		Painter(const std::string &name, Planet* planet)
			: Drawable(name)
		{
			m_planet = planet;
		}

		void drawComponent(sf::RenderTarget &target, sf::RenderStates states) const override
		{
			sf::CircleShape shape(m_planet->m_radius);
			shape.setFillColor(sf::Color::Green);
			shape.setOutlineColor(sf::Color(0,200,0));
			shape.setOutlineThickness(3);
			shape.setOrigin(m_planet->m_radius, m_planet->m_radius);
			//shape.setPosition(m_planet->getPosition());
			target.draw(shape, states);
		}
		Planet* m_planet = nullptr;
	};


	Painter* m_painter = nullptr;
	Components::VectorPainter* m_accelerationPainter = nullptr;

	sf::Vector2f m_nextPosition;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_force;
	float m_mass = 1;
	float m_radius = 50;

	static std::vector<Planet*> m_planets;
	static bool m_enableCollision;
	static sf::FloatRect m_worldBounds;
};