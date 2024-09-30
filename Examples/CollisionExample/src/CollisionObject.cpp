#include "CollisionObject.h"

CLONE_FUNC_IMPL(CollisionObject);
CollisionObject::CollisionObject(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
{
	m_collider = new QSFML::Components::Collider();
	m_collider->setVertecies({
			sf::Vector2f(0,0),
			sf::Vector2f(10,10),
			sf::Vector2f(15,8),
			sf::Vector2f(5,20)
		});

	createTransform();
	addComponent(m_collider);
	addComponent(m_collider->createPainter());
	updateObjectChanges();
}
CollisionObject::CollisionObject(const std::vector<sf::Vector2f>& vertecies,
								 const std::string& name, 
	QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
{
	m_collider = new QSFML::Components::Collider();
	m_collider->setVertecies(vertecies);

	createTransform();
	addComponent(m_collider);
	addComponent(m_collider->createPainter());
	updateObjectChanges();
}
CollisionObject::~CollisionObject()
{

}

/*QSFML::Components::Collider* CollisionObject::getCollider() const
{
	return m_collider;
}*/
