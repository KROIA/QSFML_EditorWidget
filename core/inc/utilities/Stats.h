#pragma once

#include "QSFML_EditorWidget_base.h"
#include "QSFML_EditorWidget_ForwardDeclaration.h"

#include "utilities/Singelton.h"
#include <unordered_map>
#include <atomic>
#include <string>

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

			unsigned int getRootObjectCount() const;
			unsigned int getObjectCount() const;
			unsigned int getComponentCount() const;
			unsigned int getCollisionCheckCount() const;
			unsigned int getBoundingBoxCollisionCheckCount() const;
			unsigned int getCollisionCount() const;


			double getFPS() const;
			double getFrameTime() const;	// Time in ms
			double getEventTime() const;	// Time in ms
			double getUpdateTime() const;	// Time in ms
			double getDrawTime() const;		// Time in ms

			double getDeltaT() const;
			double getFixedDeltaT() const;
			double getElapsedTime() const;

			size_t getTick() const;

			std::string toString() const;
			void print() const;



		private:
			void resetFrame();
			void resetTiming();

			std::atomic<unsigned int> m_rootObjectsCount;
			std::atomic<unsigned int> m_objectsCount;
			std::atomic<unsigned int> m_componentsCount;
			std::atomic<unsigned int> m_collisionChecks;
			std::atomic<unsigned int> m_boundingBoxCollisionChecks;
			std::atomic<unsigned int> m_collisions;

			double m_fps;
			double m_frameTime;
			double m_eventTime;
			double m_updateTime;
			double m_drawTime;

			double m_deltaT;		// Time in s
			double m_fixedDeltaT;	// Time in s
			double m_elapsedTime;	// Time in s

			size_t m_tick;
		};
		class QSFML_EDITOR_WIDGET_EXPORT StatsManager//: public Singleton<StatsManager>
		{
			friend Objects::CanvasObject;
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
			/*
			void setRootCanvesObject(unsigned int count);
			void addRootCanvesObject(unsigned int count = 1);
			void addCanvesObject(unsigned int count = 1);
			void removeRootCanvasObject(unsigned int count = 1);
			void removeCanvasObject(unsigned int count = 1);

			void addComponent(unsigned int count = 1);
			void removeComponent(unsigned int count = 1);

			void addCollisionCheck(unsigned int count = 1);
			void addBoundingBoxCollisionCheck(unsigned int count = 1);
			void addCollision(unsigned int count = 1);

			void setFPS(double fps);
			void setFrameTime(double t);
			void setEventTime(double t);
			void setUpdateTime(double t);
			void setDrawTime(double t);

			void setDeltaT(double t);

			void addTick();*/
			
			

			void setRootCanvesObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount = count;
			}
			void addRootCanvesObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount += count;
			}
			void addCanvesObject(unsigned int count = 1)
			{
				m_currentStats.m_objectsCount += count;
			}

			void removeRootCanvasObject(unsigned int count = 1)
			{
				m_currentStats.m_rootObjectsCount -= count;
			}
			void removeCanvasObject(unsigned int count= 1)
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

			void resetFrame();
			void resetTiming();

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
