#include "MouseCollider.h"

CLONE_FUNC_IMPL(MouseCollider)

MouseCollider::MouseCollider(const std::string& name, CanvasObject* parent)
	: CanvasObject(name)
{
	m_collider = new QSFML::Components::Collider();
	m_collider->setVertecies({
			sf::Vector2f(0,0),
			sf::Vector2f(10,10),
			sf::Vector2f(5,20)
		});

	addComponent(m_collider);
	addComponent(m_collider->createPainter());

	m_mouseFollower = new QSFML::Components::MouseFollower();
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
			this, &MouseCollider::onMousePosChanged);
	addComponent(m_mouseFollower);
}
MouseCollider::MouseCollider(const MouseCollider& other)
	: CanvasObject(other)
{

}
MouseCollider::~MouseCollider()
{

}

void MouseCollider::onMousePosChanged(const sf::Vector2f& worldPos,
									  const sf::Vector2i& pixelPos)
{
	m_collider->setPos(worldPos);
}