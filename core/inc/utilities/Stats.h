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
			double getFrameTime() const;
			double getEventTime() const;
			double getUpdateTime() const;
			double getDrawTime() const;

			double getDeltaT() const;
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

			double m_deltaT;
			double m_elapsedTime;

			size_t m_tick;
		};
		class QSFML_EDITOR_WIDGET_EXPORT StatsManager//: public Singleton<StatsManager>
		{
			friend Objects::CanvasObject;
			friend Components::Component;
			friend Components::Collider;
			friend Utilities::ObjectQuadTree;
		public:
			const Stats& getCurrentStats() const;
			const Stats& getLastStats() const;

		protected:

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

			void addTick();

			void resetFrame();
			void resetTiming();

		protected:
			Stats& getCurrentStats_internal();
			Stats& getLastStats_internal();



			Stats m_currentStats;
			Stats m_lastStats;

		};
	}
}
