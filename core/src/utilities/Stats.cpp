#include "utilities/Stats.h"
#include "Scene/Scene.h"

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
			, m_tps(0)
			, m_frameTime(0)
			, m_eventTime(0)
			, m_updateTime(0)
			, m_drawTime(0)
			, m_tick(0)
			, m_deltaT(0)
			, m_fixedDeltaT(0)
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
			, m_tps(other.m_tps)
			, m_frameTime(other.m_frameTime)
			, m_eventTime(other.m_eventTime)
			, m_updateTime(other.m_updateTime)
			, m_drawTime(other.m_drawTime)
			, m_tick(other.m_tick)
			, m_deltaT(other.m_deltaT)
			, m_fixedDeltaT(other.m_fixedDeltaT)
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
			m_tps = other.m_tps;
			m_frameTime = other.m_frameTime;
			m_eventTime = other.m_eventTime;
			m_updateTime = other.m_updateTime;
			m_drawTime = other.m_drawTime;

			m_deltaT = other.m_deltaT;
			m_fixedDeltaT = other.m_fixedDeltaT;
			m_elapsedTime = other.m_elapsedTime;

			m_tick = other.m_tick;
			return *this;
		}

		Stats Stats::getSmothed(const Stats& oldStats, double smothness)
		{
			Stats s(*this);
			double diff = 1 - smothness;
			s.m_fps = diff * m_fps + smothness * oldStats.m_fps;
			s.m_tps = diff * m_tps + smothness * oldStats.m_tps;
			s.m_frameTime = diff * m_frameTime + smothness * oldStats.m_frameTime;
			s.m_eventTime = diff * m_eventTime + smothness * oldStats.m_eventTime;
			s.m_updateTime = diff * m_updateTime + smothness * oldStats.m_updateTime;
			s.m_drawTime = diff * m_drawTime + smothness * oldStats.m_drawTime;

			s.m_deltaT = diff * m_deltaT + smothness * oldStats.m_deltaT;
			return s;
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
				"  TPS:                 " + std::to_string(m_tps) + "\n" +
				"  DeltaT:              " + std::to_string(m_deltaT * 1000) + " ms\n" +
				"  Elapsed time:        " + std::to_string(m_elapsedTime) + " s\n" +
				"  Elapsed fixed time:  " + std::to_string(m_fixedDeltaT * m_tick) + " s\n" +
				"  Frame time:          " + std::to_string(m_frameTime * 1000) + " ms\n" +
				"   Event  time:        " + std::to_string(m_eventTime * 1000) + " ms\n" +
				"   Update time:        " + std::to_string(m_updateTime * 1000) + " ms\n" +
				"   Draw   time:        " + std::to_string(m_drawTime * 1000) + " ms\n";

		}
		void Stats::print() const
		{
			qDebug() << toString().c_str();
		}
		void Stats::resetFrame_synced(Stats& copyTo)
		{
			copyTo.m_frameTime = m_frameTime;
			m_frameTime = 0;
		}
		void Stats::resetFrame_eventloop(Stats& copyTo)
		{
			copyTo.m_eventTime = m_eventTime;
			m_eventTime = 0;
		}
		void Stats::resetFrame_updateLoop(Stats& copyTo)
		{
			copyTo.m_updateTime = m_updateTime;
			copyTo.m_collisionChecks.store(m_collisionChecks);
			copyTo.m_boundingBoxCollisionChecks.store(m_boundingBoxCollisionChecks);
			copyTo.m_collisions.store(m_collisions);
			copyTo.m_tick = m_tick;
			copyTo.m_deltaT = m_deltaT;
			copyTo.m_fixedDeltaT = m_fixedDeltaT;
			copyTo.m_elapsedTime = m_elapsedTime;
			copyTo.m_tps = m_tps;

			m_updateTime = 0;
			m_collisionChecks = 0;
			m_boundingBoxCollisionChecks = 0;
			m_collisions = 0;
		}
		void Stats::resetFrame_paintLoop(Stats& copyTo)
		{
			copyTo.m_fps = m_fps;
			copyTo.m_drawTime = m_drawTime;

			//m_fps = 0;
			m_drawTime = 0;
		}
		void Stats::copyObjectCounts(const Stats& from)
		{
			m_rootObjectsCount = from.m_rootObjectsCount.load();
			m_objectsCount = from.m_objectsCount.load();
			m_componentsCount = from.m_componentsCount.load();
		}
		void Stats::resetTiming()
		{
			//m_fps = 0;
			m_frameTime = 0;
			m_eventTime = 0;
			m_updateTime = 0;
			m_drawTime = 0;

			m_deltaT = 0;
			m_elapsedTime = 0;
		}
	}
}
