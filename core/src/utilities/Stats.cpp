#include "utilities/Stats.h"
#include "canvas/Canvas.h"
namespace QSFML
{
	Stats::Stats(Canvas* canvas)
		: m_canvas(canvas)
		, m_rootObjectsCount(0)
		, m_objectsCount(0)
		, m_componentsCount(0)
		, m_collisionChecks(0)
		, m_boundingBoxCollisionChecks(0)
		, m_collisions(0)
	{

	}
	Stats::Stats(const Stats& other)
		: m_canvas(other.m_canvas)
		, m_rootObjectsCount(other.m_rootObjectsCount)
		, m_objectsCount(other.m_objectsCount)
		, m_componentsCount(other.m_componentsCount)
		, m_collisionChecks(other.m_collisionChecks)
		, m_boundingBoxCollisionChecks(other.m_boundingBoxCollisionChecks)
		, m_collisions(other.m_collisions)
	{

	}

	Canvas* Stats::getCanvas() const
	{
		return m_canvas;
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
	unsigned int Stats::getTickCount() const
	{
		if (m_canvas)
			return m_canvas->getTick();
		return 0;
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
	float Stats::getFrametime() const
	{
		if (m_canvas)
			return m_canvas->getDeltaT();
		return 0;
	}
	float Stats::getFPS() const
	{
		if (m_canvas)
			return 1.0f / m_canvas->getDeltaT();
		return 0;
	}
	void Stats::print() const
	{
		qDebug() << "Stats: ";
		qDebug() << "  Tick:                  " << getTickCount();
		qDebug() << "  FPS:                   " << getFPS();
		qDebug() << "  Frametime:             " << getFrametime()*1000.f << " ms";
		qDebug() << "  Root Objects:          " << m_rootObjectsCount;
		qDebug() << "  Objects:               " << m_objectsCount;
		qDebug() << "  Components:            " << m_componentsCount;
		qDebug() << "  Physics:               ";
		qDebug() << "    Collision checks:    " << m_collisionChecks;
		qDebug() << "    AABB checks:         " << m_boundingBoxCollisionChecks;
		qDebug() << "    Collisions:          " << m_collisions;
	}


	std::unordered_map<Canvas*, Stats> StatsManager::m_instances;
	const Stats& StatsManager::getStats(Canvas* canvas)
	{
		auto it = m_instances.find(canvas);
		if (it != m_instances.end()) {
			return it->second;
		}
		static Stats dummy(nullptr);
		return dummy;
	}
	void StatsManager::createStats(Canvas* canvas)
	{
		m_instances.emplace(std::pair<Canvas*, Stats>( canvas, Stats(canvas)));
	}
	Stats& StatsManager::getStats_internal(Canvas* canvas)
	{
		auto it = m_instances.find(canvas);
		if (it != m_instances.end()) {
			return it->second;
		}
		static Stats dummy(nullptr);
		return dummy;
	}

	void StatsManager::setRootCanvesObject(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_rootObjectsCount = count;
	}
	void StatsManager::addRootCanvesObject(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_rootObjectsCount += count;
	}
	void StatsManager::addCanvesObject(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_objectsCount += count;
	}
	
	void StatsManager::removeRootCanvasObject(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_rootObjectsCount -= count;
	}
	void StatsManager::removeCanvasObject(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_objectsCount -= count;
	}
	
	void StatsManager::addComponent(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_componentsCount += count;
	}
	void StatsManager::removeComponent(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_componentsCount -= count;
	}
	void StatsManager::addCollisionCheck(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_collisionChecks += count;
	}
	void StatsManager::addBoundingBoxCollisionCheck(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_boundingBoxCollisionChecks += count;
	}
	void StatsManager::addCollision(Canvas* canvas, unsigned int count)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_collisions += count;
	}
	void StatsManager::resetCollisionStats(Canvas* canvas)
	{
		Stats& stats = getStats_internal(canvas);
		stats.m_collisionChecks = 0;
		stats.m_boundingBoxCollisionChecks = 0;
		stats.m_collisions = 0;
	}
}