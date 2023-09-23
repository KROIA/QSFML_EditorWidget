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
	Stats& Stats::operator=(const Stats& other)
	{
		m_rootObjectsCount = other.m_rootObjectsCount.load();
		m_objectsCount = other.m_objectsCount.load();
		m_componentsCount = other.m_componentsCount.load();
		m_collisionChecks = other.m_collisionChecks.load();
		m_boundingBoxCollisionChecks = other.m_boundingBoxCollisionChecks.load();
		m_collisions = other.m_collisions.load();
		return *this;
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
	std::string Stats::toString() const
	{
		return	"Root Objects:          " + std::to_string(m_rootObjectsCount) + "\n" +
				"Objects:               " + std::to_string(m_objectsCount) + "\n" +
				"Components:            " + std::to_string(m_componentsCount) + "\n" +
				"Physics:               \n" +
				"  Collision checks:    " + std::to_string(m_collisionChecks) + "\n" +
				"  AABB checks:         " + std::to_string(m_boundingBoxCollisionChecks) + "\n" +
				"  Collisions:          " + std::to_string(m_collisions) + "\n";
	}
	void Stats::print() const
	{
		qDebug() << "Stats: \n" << toString().c_str();
	}


	Stats StatsManager::m_currentStats;
	Stats StatsManager::m_lastStats;
	const Stats& StatsManager::getCurrentStats()
	{
		return m_currentStats;
	}
	const Stats& StatsManager::getLastStats()
	{
		return m_lastStats;
	}
	Stats& StatsManager::getCurrentStats_internal()
	{
		return m_currentStats;
	}
	Stats& StatsManager::getLastStats_internal()
	{
		return m_lastStats;
	}

	void StatsManager::setRootCanvesObject(unsigned int count)
	{
		m_currentStats.m_rootObjectsCount = count;
	}
	void StatsManager::addRootCanvesObject(unsigned int count)
	{
		m_currentStats.m_rootObjectsCount += count;
	}
	void StatsManager::addCanvesObject(unsigned int count)
	{
		m_currentStats.m_objectsCount += count;
	}
	
	void StatsManager::removeRootCanvasObject(unsigned int count)
	{
		m_currentStats.m_rootObjectsCount -= count;
	}
	void StatsManager::removeCanvasObject(unsigned int count)
	{
		m_currentStats.m_objectsCount -= count;
	}
	
	void StatsManager::addComponent(unsigned int count)
	{
		m_currentStats.m_componentsCount += count;
	}
	void StatsManager::removeComponent(unsigned int count)
	{
		m_currentStats.m_componentsCount -= count;
	}
	void StatsManager::addCollisionCheck(unsigned int count)
	{
		m_currentStats.m_collisionChecks += count;
	}
	void StatsManager::addBoundingBoxCollisionCheck(unsigned int count)
	{
		m_currentStats.m_boundingBoxCollisionChecks += count;
	}
	void StatsManager::addCollision(unsigned int count)
	{
		m_currentStats.m_collisions += count;
	}
	void StatsManager::resetCollisionStats()
	{
		m_lastStats = m_currentStats;
		m_currentStats.m_collisionChecks = 0;
		m_currentStats.m_boundingBoxCollisionChecks = 0;
		m_currentStats.m_collisions = 0;
	}
}