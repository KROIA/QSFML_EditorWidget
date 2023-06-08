#include "utilities/Stats.h"
#include "canvas/Canvas.h"
namespace QSFML
{
	Stats::Stats()
		: m_rootObjectsCount(0)
		, m_objectsCount(0)
		, m_componentsCount(0)
		, m_collisionChecks(0)
		, m_boundingBoxCollisionChecks(0)
		, m_collisions(0)
	{

	}
	Stats::Stats(const Stats& other)
		: m_rootObjectsCount(other.m_rootObjectsCount.load())
		, m_objectsCount(other.m_objectsCount.load())
		, m_componentsCount(other.m_componentsCount.load())
		, m_collisionChecks(other.m_collisionChecks.load())
		, m_boundingBoxCollisionChecks(other.m_boundingBoxCollisionChecks.load())
		, m_collisions(other.m_collisions.load())
	{

	}

	unsigned int Stats::getRootObjectCount() const
	{
		return m_rootObjectsCount;
	}
	unsigned int Stats::getObjectCount() const
	{
		return m_objectsCount;
	}
	unsigned int Stats::getComponentCount() const
	{
		return m_componentsCount;
	}
	unsigned int Stats::getCollisionCheckCount() const
	{
		return m_collisionChecks;
	}
	unsigned int Stats::getBoundingBoxCollisionCheckCount() const
	{
		return m_boundingBoxCollisionChecks;
	}
	unsigned int Stats::getCollisionCount() const
	{
		return m_collisions;
	}
	void Stats::print() const
	{
		qDebug() << "Stats: ";
		//qDebug() << "  Tick:                  " << getTickCount();
		//qDebug() << "  FPS:                   " << getFPS();
		//qDebug() << "  Frametime:             " << getFrametime()*1000.f << " ms";
		qDebug() << "  Root Objects:          " << m_rootObjectsCount;
		qDebug() << "  Objects:               " << m_objectsCount;
		qDebug() << "  Components:            " << m_componentsCount;
		qDebug() << "  Physics:               ";
		qDebug() << "    Collision checks:    " << m_collisionChecks;
		qDebug() << "    AABB checks:         " << m_boundingBoxCollisionChecks;
		qDebug() << "    Collisions:          " << m_collisions;
	}


	Stats StatsManager::m_instance;
	const Stats& StatsManager::getStats()
	{
		return m_instance;
	}
	Stats& StatsManager::getStats_internal()
	{
		return m_instance;
	}

	void StatsManager::setRootCanvesObject(unsigned int count)
	{
		m_instance.m_rootObjectsCount = count;
	}
	void StatsManager::addRootCanvesObject(unsigned int count)
	{
		m_instance.m_rootObjectsCount += count;
	}
	void StatsManager::addCanvesObject(unsigned int count)
	{
		m_instance.m_objectsCount += count;
	}
	
	void StatsManager::removeRootCanvasObject(unsigned int count)
	{
		m_instance.m_rootObjectsCount -= count;
	}
	void StatsManager::removeCanvasObject(unsigned int count)
	{
		m_instance.m_objectsCount -= count;
	}
	
	void StatsManager::addComponent(unsigned int count)
	{
		m_instance.m_componentsCount += count;
	}
	void StatsManager::removeComponent(unsigned int count)
	{
		m_instance.m_componentsCount -= count;
	}
	void StatsManager::addCollisionCheck(unsigned int count)
	{
		m_instance.m_collisionChecks += count;
	}
	void StatsManager::addBoundingBoxCollisionCheck(unsigned int count)
	{
		m_instance.m_boundingBoxCollisionChecks += count;
	}
	void StatsManager::addCollision(unsigned int count)
	{
		m_instance.m_collisions += count;
	}
	void StatsManager::resetCollisionStats()
	{
		m_instance.m_collisionChecks = 0;
		m_instance.m_boundingBoxCollisionChecks = 0;
		m_instance.m_collisions = 0;
	}
}