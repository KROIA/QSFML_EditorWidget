#pragma once

#include "QSFML_EditorWidget_base.h"
#include "QSFML_EditorWidget_ForwardDeclaration.h"

#include "utilities/Singelton.h"
#include <unordered_map>
#include <atomic>
#include <string>
#include <array>

namespace QSFML
{
	namespace Utilities
	{
		class StatsManager;
		class QSFML_EDITOR_WIDGET_EXPORT Stats
		{
			friend StatsManager;
		public:
			Stats();
			Stats(const Stats& other);
			Stats& operator=(const Stats& other);

			// Applys a filter to smooth the fast changing timing values.: f(x, smothness) = smothness*oldX + (1-smothness)*x 
			Stats getSmothed(const Stats& oldStats, double smothness = 0.9);

			unsigned int getRootObjectCount() const
			{
				return m_rootObjectsCount;
			}
			unsigned int getObjectCount() const
			{
				return m_objectsCount;
			}
			unsigned int getComponentCount() const
			{
				return m_componentsCount;
			}
			unsigned int getCollisionCheckCount() const
			{
				return m_collisionChecks;
			}
			unsigned int getBoundingBoxCollisionCheckCount() const
			{
				return m_boundingBoxCollisionChecks;
			}
			unsigned int getCollisionCount() const
			{
				return m_collisions;
			}
			double getFPS() const
			{
				return m_fps;
			}
			double getTPS() const
			{
				return m_tps;
			}
			double getFrameTime() const
			{
				return m_frameTime;
			}
			double getEventTime() const
			{
				return m_eventTime;
			}
			double getUpdateTime() const
			{
				return m_updateTime;
			}
			double getDrawTime() const
			{
				return m_drawTime;
			}

			double getDeltaT() const
			{
				return m_deltaT;
			}
			double getFixedDeltaT() const
			{
				return m_fixedDeltaT;
			}
			double getElapsedTime() const
			{
				return m_elapsedTime;
			}
			double getFiexedElapsedTime() const
			{
				return m_fixedDeltaT * m_tick;
			}

			size_t getTick() const
			{
				return m_tick;
			}

			std::string toString() const;
			void print() const;

#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
			void drawImGui() const;
#endif

		private:
			void resetFrame_synced(Stats &copyTo);
			void resetFrame_eventloop(Stats &copyTo);
			void resetFrame_updateLoop(Stats &copyTo);
			void resetFrame_paintLoop(Stats &copyTo);
			void copyObjectCounts(const Stats& from);
			void resetTiming();

			std::atomic<unsigned int> m_rootObjectsCount;
			std::atomic<unsigned int> m_objectsCount;
			std::atomic<unsigned int> m_componentsCount;

			std::atomic<unsigned int> m_collisionChecks;
			std::atomic<unsigned int> m_boundingBoxCollisionChecks;
			std::atomic<unsigned int> m_collisions;

			double m_fps;
			double m_tps;
			double m_frameTime;
			double m_eventTime;
			double m_updateTime;
			double m_drawTime;

			double m_deltaT;		// Time in s
			double m_fixedDeltaT;	// Time in s
			double m_elapsedTime;	// Time in s

#if IMPLOT_LIBRARY_AVAILABLE == 1
			static constexpr size_t c_dataSize = 100;
			mutable std::array<double, c_dataSize> m_fpsData;
			mutable std::array<double, c_dataSize> m_tpsData;
			//mutable std::array<double, c_dataSize> m_frameTimeData;
			mutable std::array<double, c_dataSize> m_eventTimeData;
			mutable std::array<double, c_dataSize> m_updateTimeData;
			mutable std::array<double, c_dataSize> m_drawTimeData;
			//mutable std::array<double, c_dataSize> m_deltaTData;
			//mutable std::array<double, c_dataSize> m_elapsedTimeData;
			mutable size_t m_currentDataIndex = 0;

			void clearAllPlotBuffer() const;
#endif

			size_t m_tick;
		};
		class QSFML_EDITOR_WIDGET_EXPORT StatsManager//: public Singleton<StatsManager>
		{
			friend Objects::GameObject;
			friend Components::Component;
			friend Components::Collider;
			friend Utilities::ObjectQuadTree;
		public:
			const Stats& getCurrentStats() const
			{
				return m_currentStats;
			}
			const Stats& getLastStats() const
			{
				return m_lastStats;
			}

		protected:	

			void setRootGameObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount = count;
			}
			void addRootGameObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount += count;
			}
			void addGameObject(unsigned int count = 1)
			{
				m_currentStats.m_objectsCount += count;
			}

			void removeRootGameObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount -= count;
			}
			void removeGameObject(unsigned int count= 1)
			{
				m_currentStats.m_objectsCount -= count;
			}

			void addComponent(unsigned int count= 1)
			{
				m_currentStats.m_componentsCount += count;
			}
			void removeComponent(unsigned int count = 1)
			{
				m_currentStats.m_componentsCount -= count;
			}
			void addCollisionCheck(unsigned int count = 1)
			{
				m_currentStats.m_collisionChecks += count;
			}
			void addBoundingBoxCollisionCheck(unsigned int count = 1)
			{
				m_currentStats.m_boundingBoxCollisionChecks += count;
			}
			void addCollision(unsigned int count = 1)
			{
				m_currentStats.m_collisions += count;
			}

			void setFPS(double fps)
			{
				m_currentStats.m_fps = fps;
			}
			void setTPS(double tps)
			{
				m_currentStats.m_tps = tps;
			}
			void setFrameTime(double t)
			{
				m_currentStats.m_frameTime = t;
			}
			void setEventTime(double t)
			{
				m_currentStats.m_eventTime = t;
			}
			void setUpdateTime(double t)
			{
				m_currentStats.m_updateTime = t;
			}
			void setDrawTime(double t)
			{
				m_currentStats.m_drawTime = t;
			}

			void setDeltaT(double t)
			{
				m_currentStats.m_deltaT = t;
			}
			void setFixedDeltaT(double t)
			{
				m_currentStats.m_fixedDeltaT = t;
			}

			void addTick()
			{
				m_currentStats.m_tick++;
			}

			void resetFrame_synced()
			{
				m_currentStats.resetFrame_synced(m_lastStats);
			}
			void resetFrame_eventloop()
			{
				m_currentStats.resetFrame_eventloop(m_lastStats);
			}
			void resetFrame_updateLoop()
			{
				m_currentStats.m_elapsedTime += m_currentStats.m_deltaT;
				m_currentStats.resetFrame_updateLoop(m_lastStats);
			}
			void resetFrame_paintLoop()
			{
				m_currentStats.resetFrame_paintLoop(m_lastStats);
			}
			void copyObjectCounts()
			{
				m_lastStats.copyObjectCounts(m_currentStats);
			}
			void resetTiming()
			{
				m_currentStats.resetTiming();
			}

		protected:
			Stats& getCurrentStats_internal()
			{
				return m_currentStats;
			}
			Stats& getLastStats_internal()
			{
				return m_lastStats;
			}



			Stats m_currentStats;
			Stats m_lastStats;

		};
	}
}
