#pragma once

#include "QSFML_base.h"
#include "QSFML_ForwardDeclaration.h"

#include "utilities/Singelton.h"
#include <unordered_map>
#include <atomic>

namespace QSFML
{
	class StatsManager;
	class QSFML_EDITOR_WIDGET_EXPORT Stats
	{
		friend StatsManager;
		Stats();
		public:
		
		Stats(const Stats& other);
		Stats& operator=(const Stats& other);

		unsigned int getRootObjectCount() const;
		unsigned int getObjectCount() const;
		unsigned int getComponentCount() const;
		unsigned int getCollisionCheckCount() const;
		unsigned int getBoundingBoxCollisionCheckCount() const;
		unsigned int getCollisionCount() const;

		std::string toString() const;
		void print() const;

	private:
		std::atomic<unsigned int> m_rootObjectsCount;
		std::atomic<unsigned int> m_objectsCount;
		std::atomic<unsigned int> m_componentsCount;
		std::atomic<unsigned int> m_collisionChecks;
		std::atomic<unsigned int> m_boundingBoxCollisionChecks;
		std::atomic<unsigned int> m_collisions;

	};
	class QSFML_EDITOR_WIDGET_EXPORT StatsManager: public Singleton<StatsManager>
	{
	public:
		static const Stats& getCurrentStats();
		static const Stats& getLastStats();

		static void setRootCanvesObject(unsigned int count);
		static void addRootCanvesObject(unsigned int count = 1);
		static void addCanvesObject(unsigned int count = 1);
		static void removeRootCanvasObject(unsigned int count = 1);
		static void removeCanvasObject(unsigned int count = 1);

		static void addComponent(unsigned int count = 1);
		static void removeComponent(unsigned int count = 1);

		static void addCollisionCheck(unsigned int count = 1);
		static void addBoundingBoxCollisionCheck(unsigned int count = 1);
		static void addCollision(unsigned int count = 1);
		static void resetCollisionStats();
	private:
		static Stats& getCurrentStats_internal();
		static Stats& getLastStats_internal();

		

		static Stats m_currentStats;
		static Stats m_lastStats;

	};
}