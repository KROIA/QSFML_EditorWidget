#include "CollisionChecker.h"


CLONE_FUNC_IMPL(CollisionChecker);
CollisionChecker::CollisionChecker(const std::string& name, CanvasObject* parent)
	: QObject()
	, CanvasObject(name)
	, m_mode(Mode::intersecting)
{
	m_mouseCollider = new MouseCollider();
	//m_collisionObject = new CollisionObject();

	
	m_colliderContainer = new QSFML::Objects::CanvasObject();
	for (size_t i = 0; i < 10; ++i)
	{
		CollisionObject* obj = new CollisionObject();
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector() * 100.f;
		obj->setPositionAbsolute(pos);
		m_collisionObjs.push_back(obj);
	}
	m_colliderContainer->addChilds(m_collisionObjs);
	
	addChild(m_mouseCollider);
	addChild(m_colliderContainer);
	m_objs = m_collisionObjs;
	m_objs.push_back(m_mouseCollider);
	//addChilds(m_objs);

	QSFML::Utilities::AABB range(0, 0, 800, 600);
	m_performanceContainer = new QSFML::Objects::CanvasObject();
	for (size_t i = 0; i < 1000; ++i)
	{
		PerformanceObject* obj = new PerformanceObject();
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector(range.getPos(), range.getSize() + range.getPos());
		obj->setPositionAbsolute(pos);
		obj->setRange(range);
		
		m_performanceObjs.push_back(obj);
	}
	m_performanceContainer->addChilds(m_performanceObjs);
	m_performanceObjs.push_back(m_mouseCollider);
	addChild(m_performanceContainer);
	m_performanceContainer->setEnabled(false);
	
	

	m_pointPainter = new QSFML::Components::PointPainter();
	m_pointPainter->setColor(sf::Color::Yellow);
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
		case Mode::performanceTest:
		{
			m_colliderContainer->setEnabled(true);
			m_mouseCollider->setEnabled(false);
			//m_mouseFollower->setEnabled(false);
			m_performanceContainer->setEnabled(false);
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
		case Mode::performanceTest:
		{
			m_colliderContainer->setEnabled(false);
			m_mouseCollider->setEnabled(true);
			m_performanceContainer->setEnabled(true);
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
		case Mode::performanceTest:
			update_performanceTest();
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
void CollisionChecker::update_performanceTest()
{
	std::vector<sf::Vector2f> collisionPoints;
	collisionPoints.reserve(1000);
	for (size_t i = 0; i < m_performanceObjs.size(); ++i)
	{
		for (size_t j = i + 1; j < m_performanceObjs.size(); ++j)
		{
			std::vector<QSFML::Components::Collisioninfo> collisions;
			//m_performanceObjs[i]->checkCollision(m_performanceObjs[j], collisions, false);
			if (m_performanceObjs[i]->checkCollision(m_performanceObjs[j], collisions, false))
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