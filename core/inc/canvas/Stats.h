#pragma once

#include "QSFML_base.h"
#include "utilities/Singelton.h"
#include "CanvasForwardDeclaration.h"
#include <unordered_map>

namespace QSFML
{
	class StatsManager;
	class QSFML_EDITOR_WIDGET_EXPORT Stats
	{
		friend StatsManager;
		Stats(Canvas *canvas);
	public:
		
		Stats(const Stats& other);

		Canvas* getCanvas() const;
		unsigned int getRootObjectCount() const;
		unsigned int getObjectCount() const;
		unsigned int getComponentCount() const;
		unsigned int getTickCount() const;
		unsigned int getCollisionCheckCount() const;
		unsigned int getBoundingBoxCollisionCheckCount() const;
		unsigned int getCollisionCount() const;
		float getFrametime() const;
		float getFPS() const;

		void print() const;

	private:
		Canvas* m_canvas;
		unsigned int m_rootObjectsCount;
		unsigned int m_objectsCount;
		unsigned int m_componentsCount;
		unsigned int m_collisionChecks;
		unsigned int m_boundingBoxCollisionChecks;
		unsigned int m_collisions;

	};
	class QSFML_EDITOR_WIDGET_EXPORT StatsManager: public Singleton<StatsManager>
	{
		friend Objects::CanvasObject;
		friend Canvas;
		friend CanvasObjectContainer;
		friend Components::Collider;
	public:
		static const Stats& getStats(Canvas* canvas);
	private:
		static void createStats(Canvas* canvas);

		static Stats& getStats_internal(Canvas* canvas);

		static void setRootCanvesObject(Canvas* canvas, unsigned int count);
		static void addRootCanvesObject(Canvas* canvas, unsigned int count = 1);
		static void addCanvesObject(Canvas* canvas, unsigned int count = 1);
		static void removeRootCanvasObject(Canvas* canvas, unsigned int count = 1);
		static void removeCanvasObject(Canvas* canvas, unsigned int count = 1);

		static void addComponent(Canvas* canvas, unsigned int count = 1);
		static void removeComponent(Canvas* canvas, unsigned int count = 1);

		static void addCollisionCheck(Canvas* canvas, unsigned int count = 1);
		static void addBoundingBoxCollisionCheck(Canvas* canvas, unsigned int count = 1);
		static void addCollision(Canvas* canvas, unsigned int count = 1);
		static void resetCollisionStats(Canvas* canvas);

		static std::unordered_map<Canvas*, Stats> m_instances;
	};
}