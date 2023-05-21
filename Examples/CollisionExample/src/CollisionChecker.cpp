#include "CollisionChecker.h"


CLONE_FUNC_IMPL(CollisionChecker);
CollisionChecker::CollisionChecker(const std::string& name, CanvasObject* parent)
	: QObject()
	, CanvasObject(name)
	, m_mode(Mode::intersecting)
{
	m_mouseCollider = new MouseCollider();
	//m_collisionObject = new CollisionObject();

	
	
	for (size_t i = 0; i < 10; ++i)
	{
		CollisionObject* obj = new CollisionObject();
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector() * 100.f;
		obj->setPositionAbsolute(pos);
		m_collisionObjs.push_back(obj);
	}
	
	m_objs = m_collisionObjs;
	m_objs.push_back(m_mouseCollider);
	addChilds(m_objs);


	m_pointPainter = new QSFML::Components::PointPainter();
	addComponent(m_pointPainter);

	m_mouseFollower = new QSFML::Components::MouseFollower();
	m_mouseFollower->setEnabled(false);
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
		this, &CollisionChecker::onMousePosChanged);
	addComponent(m_mouseFollower);

}
CollisionChecker::CollisionChecker(const CollisionChecker& other)
	: QObject()
	, CanvasObject(other)
{

}
CollisionChecker::~CollisionChecker()
{

}
void CollisionChecker::setMode(Mode mode)
{
	if (m_mode == mode)
		return;
	switch (m_mode)
	{
		case Mode::intersecting:
		{
			m_mouseCollider->setEnabled(false);
			break;
		}
		case Mode::contains:
		{
			m_mouseFollower->setEnabled(false);
			break;
		}
	}

	m_mode = mode;
	switch (m_mode)
	{
	case Mode::intersecting:
	{
		m_mouseCollider->setEnabled(true);
		break;
	}
	case Mode::contains:
	{
		m_mouseFollower->setEnabled(true);
		break;
	}
	}
}
void CollisionChecker::onMousePosChanged(const sf::Vector2f& worldPos,
										 const sf::Vector2i& pixelPos)
{
	m_mousePos = worldPos;
}
void CollisionChecker::update()
{
	switch (m_mode)
	{
		case Mode::intersecting:
			update_intersecting();
		break;
		case Mode::contains:
			update_contains();
		break;
	}
}
void CollisionChecker::update_intersecting()
{
	std::vector<sf::Vector2f> collisionPoints;
	for (size_t i = 0; i < m_objs.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objs.size(); ++j)
		{
			std::vector<QSFML::Components::Collisioninfo> collisions;
			if (m_objs[i]->checkCollision(m_objs[j], collisions, false))
			{
				//qDebug() << getTick() << " Collision";
				for (size_t k = 0; k < collisions.size(); ++k)
					collisionPoints.push_back(collisions[k].collisionPos);

				//m_objs[i]->solveCollision(m_objs[j]);
			}
		}
	}
	m_pointPainter->setPoints(collisionPoints);
}
void CollisionChecker::update_contains()
{
	m_pointPainter->clear();
	for (size_t i = 0; i < m_collisionObjs.size(); ++i)
	{
		std::vector<QSFML::Components::Collider*> colliders = m_collisionObjs[i]->getCollider();
		for (size_t j = 0; j < colliders.size(); ++j)
		{
			if (colliders[j]->contains(m_mousePos))
			{
				m_pointPainter->setPoint(m_mousePos);
			}
		}
	}
}