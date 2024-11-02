#include "MouseCollider.h"

CLONE_FUNC_IMPL(MouseCollider)

MouseCollider::MouseCollider(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
{
	m_collider = new QSFML::Components::Collider();
	m_collider->setVertecies({
			sf::Vector2f(0,0),
			sf::Vector2f(10,10),
			sf::Vector2f(5,20),
			sf::Vector2f(-15,35),
			sf::Vector2f(-20,15)
		});

	addComponent(m_collider);

	m_mouseFollower = new QSFML::Components::MouseFollower();
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
			this, &MouseCollider::onMousePosChanged);
	addComponent(m_mouseFollower);

	m_staticCollider = new QSFML::Components::Collider();
	m_staticCollider->setVertecies(
		{
			sf::Vector2f(0,0),
			sf::Vector2f(100,100),
			sf::Vector2f(5,200),
			sf::Vector2f(-15,305),
			sf::Vector2f(-20,150)
		}
	);
	m_staticCollider->setPos(sf::Vector2f(100.1f, 100.1f));
	enableDrawGizmosRecursive(true);

}
MouseCollider::MouseCollider(const MouseCollider& other)
	: GameObject(other)
{

}
MouseCollider::~MouseCollider()
{

}

void MouseCollider::onMousePosChanged(const sf::Vector2f& worldPos,
									  const sf::Vector2i& pixelPos)
{
	setPosition(worldPos);
	//m_collider->setPos(worldPos);
}

void MouseCollider::update()
{
	
}
