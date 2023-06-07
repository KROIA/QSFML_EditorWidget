#include "CollisionChecker.h"
#include <iostream>

#define USE_QUADTREE

CLONE_FUNC_IMPL(CollisionChecker);
CollisionChecker::CollisionChecker(const std::string& name, CanvasObject* parent)
	: QObject()
	, CanvasObject(name)
	, m_mode(Mode::intersecting)
	, m_tree(QSFML::Utilities::AABB({ 0,0 }, { 800,600 }))
{
	m_mouseCollider = new MouseCollider();
	QSFML::Utilities::RandomEngine::setSeed(0);
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
	//QSFML::Utilities::AABB range(0, 0, 80, 60);
	m_performanceContainer = new QSFML::Objects::CanvasObject();
	for (size_t i = 0; i <1000; ++i)
	{
		PerformanceObject* obj = new PerformanceObject(/*{
				sf::Vector2f(0,-10),
				sf::Vector2f(10,0),
				sf::Vector2f(0,10),
				sf::Vector2f(-10,0),

			}*/);
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector(range.getPos(), range.getSize() + range.getPos());
		obj->setPositionAbsolute(pos);
		obj->setRange(range);
		
		m_performanceObjs.push_back(obj);
		//m_tree.insert(obj);
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
	addComponent(m_tree.createPainter());
	setMode(Mode::performanceTest);

}
CollisionChecker::CollisionChecker(const CollisionChecker& other)
	: QObject()
	, CanvasObject(other)
	, m_tree(other.m_tree)
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
void CollisionChecker::inCanvasAdded()
{
	m_tree.insert(m_performanceObjs);
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
	std::vector<QSFML::Utilities::Collisioninfo> collisions;
	for (size_t i = 0; i < m_objs.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objs.size(); ++j)
		{
			
			/*if (*/m_objs[i]->checkCollision(m_objs[j], collisions, false);//)
			//{
				//qDebug() << getTick() << " Collision";
				//for (size_t k = 0; k < collisions.size(); ++k)
				//	collisionPoints.push_back(collisions[k].collisionPos);

				//m_objs[i]->solveCollision(m_objs[j]);
			//}
		}
	}
	collisionPoints.reserve(collisions.size());
	for (auto &collision : collisions)
		collisionPoints.push_back(collision.collisionPos);
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
	TimePoint t1 = std::chrono::high_resolution_clock::now();
#ifdef USE_QUADTREE
	m_tree.clear();
	m_tree.shrink();
	for (auto el : m_performanceObjs)
		el->setEnabled(true);
	m_tree.insert(m_performanceObjs);
	/*std::list<QSFML::Utilities::ObjectQuadTree::TreeItem> objs = m_tree.getAllItems();
	for (auto &it : objs)
	{
		m_tree.relocate(it);
	}*/
	
	/*for (size_t i = 0; i < m_performanceObjs.size(); ++i)
	{
		std::list< QSFML::Objects::CanvasObject*> possibleColliders;
		m_tree.search(m_performanceObjs[i]->getBoundingBox(), possibleColliders);
		for (auto it : possibleColliders)
		{
			if (m_performanceObjs[i] != it)
			{
				std::vector<QSFML::Utilities::Collisioninfo> collisions;
				m_performanceObjs[i]->checkCollision(it, collisions, false);
				
			}
		}
	}*/
	std::vector<QSFML::Utilities::Collisioninfo> collisions;
	//collisions.reserve(9999);

	TimePoint t11 = std::chrono::high_resolution_clock::now();
	//CanvasObject::checkCollision(m_tree, collisions, false);	
	m_tree.checkCollisions(collisions, false);
	TimePoint t22 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff2 = t22 - t11;

	collisionPoints.reserve(collisions.size());
	for (auto &el : collisions)
		collisionPoints.push_back(el.collisionPos);

#else
	
	std::vector<QSFML::Utilities::Collisioninfo> collisions;
	TimePoint t11 = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < m_performanceObjs.size(); ++i)
	{

		for (size_t j = i + 1; j < m_performanceObjs.size(); ++j)
		{
			
			//m_performanceObjs[i]->checkCollision(m_performanceObjs[j], collisions, false);
			m_performanceObjs[i]->checkCollision(m_performanceObjs[j], collisions, false);
			//if ()
			{
				//qDebug() << getTick() << " Collision";
				

				//m_objs[i]->solveCollision(m_objs[j]);
			}
		}
	}
	TimePoint t22 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff2 = t22 - t11;

	collisionPoints.reserve(collisions.size());
	for (auto& el : collisions)
		collisionPoints.push_back(el.collisionPos);
#endif
	TimePoint t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = t2 - t1;
	
	static float time1 = 0;
	static float time2 = 0;
	static int counter = 0;
	time1 = 0.8 * time1 + 0.2 * diff.count();
	time2 = 0.8 * time2 + 0.2 * diff2.count();
	++counter;
	if (counter % 10 == 0)
	{
		//std::cout << "Time: " << time1 << " " << time2 << "\n";
	}

	m_pointPainter->setPoints(collisionPoints);
}


/*
CollisionChecker::Painter::Painter(CollisionChecker* checker, const std::string& name)
	: Drawable(name)
	, m_ckecker(checker)
{

}
CollisionChecker::Painter::Painter(const Painter& other)
	: Drawable(other)
	, m_ckecker(other.m_ckecker)
{

}
COMPONENT_IMPL(CollisionChecker::Painter);


CollisionChecker::Painter::~Painter()
{

}


void CollisionChecker::Painter::draw(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	std::list< QSFML::Objects::CanvasObject*> possibleColliders;
	m_ckecker->m_tree.search(getCameraViewRect(), possibleColliders);

	sf::RenderWindow& window = getCanvasParent()->
	for (auto el : possibleColliders)
	{
		el->draw();
	}
}*/