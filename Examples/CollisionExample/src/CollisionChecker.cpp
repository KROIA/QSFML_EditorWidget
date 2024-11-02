#include "CollisionChecker.h"
#include <iostream>

#define USE_QUADTREE

CLONE_FUNC_IMPL(CollisionChecker);
CollisionChecker::CollisionChecker(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: QObject()
	, GameObject(name)
	, m_mode(Mode::intersecting)
	, m_tree(nullptr,QSFML::Utilities::AABB({ 0,0 }, { 800,600 }), 4)
{
	m_mouseCollider = new MouseCollider();
	QSFML::Utilities::RandomEngine::setSeed(0);
	//m_collisionObject = new CollisionObject();

	
	m_colliderContainer = new QSFML::Objects::GameObject();
	for (size_t i = 0; i < 10; ++i)
	{
		CollisionObject* obj = new CollisionObject();
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector() * 100.f;
		obj->setPosition(pos);
		sf::Vector2f pos2 = obj->getGlobalPosition();
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
	m_performanceContainer = new QSFML::Objects::GameObject();
	for (size_t i = 0; i <1000; ++i)
	{
		PerformanceObject* obj = new PerformanceObject(/*{
				sf::Vector2f(0,-10),
				sf::Vector2f(10,0),
				sf::Vector2f(0,10),
				sf::Vector2f(-10,0),

			}*/);
		sf::Vector2f pos = QSFML::Utilities::RandomEngine::getVector(range.getPos(), range.getSize() + range.getPos());
		obj->setPosition(pos);
		//obj->setPosition({0,0});
		obj->setRange(range);
		obj->setDynamic(false);
		
		m_performanceObjs.push_back(obj);
		//m_tree.insert(obj);
	}
	m_performanceContainer->addChilds(m_performanceObjs);
	m_performanceObjs.push_back(m_mouseCollider);
	addChild(m_performanceContainer);
	m_performanceContainer->setEnabled(false);
	
	

	m_pointPainter = new QSFML::Components::PointPainter();
	m_pointPainter->setColor(sf::Color::Yellow);
	

	m_mouseFollower = new QSFML::Components::MouseFollower();
	m_mouseFollower->setEnabled(false);
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
		this, &CollisionChecker::onMousePosChanged);
	addComponent(m_mouseFollower);
	//addComponent(m_tree.createPainter());
	addComponent(m_pointPainter);
	m_tree.enableCollisionThreads(true);
	setMode(Mode::performanceTest);
	setDrawOrder({ DrawSequenceElement::childs, DrawSequenceElement::components });
	
	m_performanceContainer->setDrawOrder({ DrawSequenceElement::childs });

	//createLogger();

}
CollisionChecker::CollisionChecker(const CollisionChecker& other)
	: QObject()
	, GameObject(other)
	, m_tree(other.getSceneParent(), other.m_tree.getArea(), other.m_tree.getMaxDepth())
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
void CollisionChecker::onSceneParentChange(QSFML::Scene* oldParent, QSFML::Scene* newParent)
{
	m_tree.setStatsManager(newParent);
}
void CollisionChecker::onAwake()
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
	QSFML::vector<sf::Vector2f> collisionPoints;
	QSFML::vector<QSFML::Utilities::Collisioninfo> collisions;
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
		QSFML::vector<QSFML::Components::Collider*> colliders = m_collisionObjs[i]->getComponents<QSFML::Components::Collider>();
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
	QSFML::vector<sf::Vector2f> collisionPoints;
	TimePoint t1 = std::chrono::high_resolution_clock::now();
#ifdef USE_QUADTREE
	m_tree.clear();
	m_tree.shrink();
	for (auto el : m_performanceObjs)
		el->setEnabled(true);
	QSFMLP_GENERAL_BLOCK_C("Quadtree Insert", profiler::colors::Red);
	m_tree.insert(m_performanceObjs);
	QSFMLP_GENERAL_END_BLOCK;
	//logInfo("Insertion took: " + std::to_string(std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t1).count()));
	/*QSFML::list<QSFML::Utilities::ObjectQuadTree::TreeItem> objs = m_tree.getAllItems();
	for (auto &it : objs)
	{
		m_tree.relocate(it);
	}*/
	
	/*for (size_t i = 0; i < m_performanceObjs.size(); ++i)
	{
		QSFML::list< QSFML::Objects::GameObjectPtr> possibleColliders;
		m_tree.search(m_performanceObjs[i]->getBoundingBox(), possibleColliders);
		for (auto it : possibleColliders)
		{
			if (m_performanceObjs[i] != it)
			{
				QSFML::vector<QSFML::Utilities::Collisioninfo> collisions;
				m_performanceObjs[i]->checkCollision(it, collisions, false);
				
			}
		}
	}*/
	QSFML::vector<QSFML::Utilities::Collisioninfo> collisions;
	//collisions.reserve(9999);

	TimePoint t11 = std::chrono::high_resolution_clock::now();
	//GameObject::checkCollision(m_tree, collisions, false);	
	m_tree.checkCollisions(collisions, false);
	TimePoint t22 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff2 = t22 - t11;

	collisionPoints.reserve(collisions.size());
	for (auto &el : collisions)
		collisionPoints.push_back(el.collisionPos);

#else
	
	QSFML::vector<QSFML::Utilities::Collisioninfo> collisions;
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


void CollisionChecker::Painter::drawComponent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	QSFML::list< QSFML::Objects::GameObjectPtr> possibleColliders;
	m_ckecker->m_tree.search(getCameraViewRect(), possibleColliders);

	sf::RenderWindow& window = getSceneParent()->
	for (auto el : possibleColliders)
	{
		el->draw();
	}
}*/
