#include "utilities/Stats.h"
#include "canvas/Canvas.h"

#include <QDebug>

namespace QSFML
{
	namespace Utilities
	{
		Stats::Stats()
			: m_rootObjectsCount(0)
			, m_objectsCount(0)
			, m_componentsCount(0)
			, m_collisionChecks(0)
			, m_boundingBoxCollisionChecks(0)
			, m_collisions(0)
			, m_fps(0)
			, m_frameTime(0)
			, m_eventTime(0)
			, m_updateTime(0)
			, m_drawTime(0)
			, m_tick(0)
			, m_deltaT(0)
			, m_elapsedTime(0)
		{

		}
		Stats::Stats(const Stats& other)
			: m_rootObjectsCount(other.m_rootObjectsCount.load())
			, m_objectsCount(other.m_objectsCount.load())
			, m_componentsCount(other.m_componentsCount.load())
			, m_collisionChecks(other.m_collisionChecks.load())
			, m_boundingBoxCollisionChecks(other.m_boundingBoxCollisionChecks.load())
			, m_collisions(other.m_collisions.load())
			, m_fps(other.m_fps)
			, m_frameTime(other.m_frameTime)
			, m_eventTime(other.m_eventTime)
			, m_updateTime(other.m_updateTime)
			, m_drawTime(other.m_drawTime)
			, m_tick(other.m_tick)
			, m_deltaT(other.m_deltaT)
			, m_elapsedTime(other.m_elapsedTime)
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

			m_fps = other.m_fps;
			m_frameTime = other.m_frameTime;
			m_eventTime = other.m_eventTime;
			m_updateTime = other.m_updateTime;
			m_drawTime = other.m_drawTime;

			m_deltaT = other.m_deltaT;
			m_elapsedTime = other.m_elapsedTime;

			m_tick = other.m_tick;
			return *this;
		}

		Stats Stats::getSmothed(const Stats& oldStats, double smothness)
		{
			Stats s(*this);
			double diff = 1 - smothness;
			s.m_fps = diff * m_fps + smothness * oldStats.m_fps;
			s.m_frameTime = diff * m_frameTime + smothness * oldStats.m_frameTime;
			s.m_eventTime = diff * m_eventTime + smothness * oldStats.m_eventTime;
			s.m_updateTime = diff * m_updateTime + smothness * oldStats.m_updateTime;
			s.m_drawTime = diff * m_drawTime + smothness * oldStats.m_drawTime;

			s.m_deltaT = diff * m_deltaT + smothness * oldStats.m_deltaT;
			//s.m_elapsedTime = diff * m_elapsedTime + smothness * oldStats.m_elapsedTime;
			return s;
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
		double Stats::getFPS() const
		{
			return m_fps;
		}
		double Stats::getFrameTime() const
		{
			return m_frameTime;
		}
		double Stats::getEventTime() const
		{
			return m_eventTime;
		}
		double Stats::getUpdateTime() const
		{
			return m_updateTime;
		}
		double Stats::getDrawTime() const
		{
			return m_drawTime;
		}

		double Stats::getDeltaT() const
		{
			return m_deltaT;
		}
		double Stats::getElapsedTime() const
		{
			return m_elapsedTime;
		}

		size_t Stats::getTick() const
		{
			return m_tick;
		}

		std::string Stats::toString() const
		{
			return	"Stats:\n"
				" Tick:                 " + std::to_string(m_tick) + "\n" +
				" Root Objects:         " + std::to_string(m_rootObjectsCount) + "\n" +
				" Objects:              " + std::to_string(m_objectsCount) + "\n" +
				" Components:           " + std::to_string(m_componentsCount) + "\n" +
				" Physics:\n" +
				"  Collision checks:    " + std::to_string(m_collisionChecks) + "\n" +
				"  AABB checks:         " + std::to_string(m_boundingBoxCollisionChecks) + "\n" +
				"  Collisions:          " + std::to_string(m_collisions) + "\n" +
				" Timing:\n"
				"  FPS:                 " + std::to_string(m_fps) + "\n" +
				"  DeltaT:              " + std::to_string(m_deltaT * 1000) + " ms\n" +
				"  Elapsed time:        " + std::to_string(m_elapsedTime) + " s\n" +
				"  Frame time:          " + std::to_string(m_frameTime * 1000) + " ms\n" +
				"   Event  time:        " + std::to_string(m_eventTime * 1000) + " ms\n" +
				"   Update time:        " + std::to_string(m_updateTime * 1000) + " ms\n" +
				"   Draw   time:        " + std::to_string(m_drawTime * 1000) + " ms\n";

		}
		void Stats::print() const
		{
			qDebug() << toString().c_str();
		}
		void Stats::resetFrame()
		{
			m_collisionChecks = 0;
			m_boundingBoxCollisionChecks = 0;
			m_collisions = 0;

			m_fps = 0;
			m_frameTime = 0;
			m_eventTime = 0;
			m_updateTime = 0;
			m_drawTime = 0;
		}

		void Stats::resetTiming()
		{
			m_fps = 0;
			m_frameTime = 0;
			m_eventTime = 0;
			m_updateTime = 0;
			m_drawTime = 0;

			m_deltaT = 0;
			m_elapsedTime = 0;
		}


		//Stats StatsManager::m_currentStats;
		//Stats StatsManager::m_lastStats;
		const Stats& StatsManager::getCurrentStats() const
		{
			return m_currentStats;
		}
		const Stats& StatsManager::getLastStats() const
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

		void StatsManager::setFPS(double fps)
		{
			m_currentStats.m_fps = fps;
		}
		void StatsManager::setFrameTime(double t)
		{
			m_currentStats.m_frameTime = t;
		}
		void StatsManager::setEventTime(double t)
		{
			m_currentStats.m_eventTime = t;
		}
		void StatsManager::setUpdateTime(double t)
		{
			m_currentStats.m_updateTime = t;
		}
		void StatsManager::setDrawTime(double t)
		{
			m_currentStats.m_drawTime = t;
		}

		void StatsManager::setDeltaT(double t)
		{
			m_currentStats.m_deltaT = t;
		}

		void StatsManager::addTick()
		{
			m_currentStats.m_tick++;
		}

		void StatsManager::resetFrame()
		{
			m_currentStats.m_elapsedTime += m_currentStats.m_deltaT;
			m_lastStats = m_currentStats;
			m_currentStats.resetFrame();
		}
		void StatsManager::resetTiming()
		{
			m_currentStats.resetTiming();
		}
	}
}