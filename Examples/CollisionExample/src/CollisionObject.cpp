#include "CollisionObject.h"

CLONE_FUNC_IMPL(CollisionObject);
CollisionObject::CollisionObject(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
{
	setup({
			sf::Vector2f(0,0),
			sf::Vector2f(10,10),
			sf::Vector2f(15,8),
			sf::Vector2f(5,20)
		  });
}
CollisionObject::CollisionObject(const QSFML::vector<sf::Vector2f>& vertecies,
								 const std::string& name, 
	QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
{
	setup(vertecies);
}
CollisionObject::~CollisionObject()
{

}

void CollisionObject::setup(const QSFML::vector<sf::Vector2f>& vertecies)
{
	m_collider = new QSFML::Components::Collider();
	m_collider->setVertecies(vertecies);

	addComponent(m_collider);
	addComponent(m_collider->createPainter());
}
void CollisionObject::onAwake()
{
	//logInfo("CollisionObject added to scene");
}

/*QSFML::Components::Collider* CollisionObject::getCollider() const
{
	return m_collider;
}*/
