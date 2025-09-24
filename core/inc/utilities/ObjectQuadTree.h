#pragma once

#include "QSFML_EditorWidget_base.h"

#include "objects/base/GameObject.h"
#include "components/base/Drawable.h"

#include "AABB.h"
#include "ThreadWorker.h"

#include <vector>
#include <list>
#include <unordered_map>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_API ObjectQuadTree
		{
			class QSFML_EDITOR_WIDGET_API Tree;
		public:
			struct QSFML_EDITOR_WIDGET_API TreeItem
			{
				friend ObjectQuadTree;
				friend Tree;
				TreeItem(Objects::GameObjectPtr obj)
					: obj(obj)
					, containter(nullptr)
				{}
				TreeItem()
					: obj(nullptr)
					, containter(nullptr)
				{}
				Objects::GameObjectPtr obj;
			private:
				QSFML::vector<Objects::GameObjectPtr>* containter;
				QSFML::vector<Objects::GameObjectPtr>::iterator iterator;
			};

			static void setGizmoColor(const sf::Color& color) { s_gizmoColor = color; }
			static const sf::Color& getGizmoColor() { return s_gizmoColor; }
			static void enableGizmoText(bool enable) { s_gizmoEnableText = enable; }
			static bool isGizmoTextEnabled() { return s_gizmoEnableText; }

			ObjectQuadTree(Utilities::StatsManager* statsManager, const Utilities::AABB& area, size_t maxDepth = 10);
			ObjectQuadTree(const ObjectQuadTree& other) = delete;
			~ObjectQuadTree();

			void setStatsManager(Utilities::StatsManager* manager);

			bool insert(Objects::GameObjectPtr obj);
			bool insert(const QSFML::vector<Objects::GameObjectPtr>& objs);

			void search(const Utilities::AABB& area, QSFML::list< Objects::GameObjectPtr>& container) const;
			const QSFML::vector<TreeItem>& getAllItems() const;
			void remove(Objects::GameObjectPtr obj);
			void remove(const TreeItem& item);
			bool relocate(TreeItem& item);
			void clear();
			void shrink(); // removes not used childs

			const Utilities::AABB& getArea() const;
			size_t getMaxDepth() const;

			void enableCollisionThreads(bool enable);
			void checkCollisions(QSFML::vector<Utilities::Collisioninfo>& collisions,
								 bool onlyFirstCollision = true);



			void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const;


		private:
			bool insert_internal(TreeItem& item);
			class QSFML_EDITOR_WIDGET_API Tree
			{
				friend class ObjectQuadTree;
			public:
				Tree(Utilities::StatsManager* statsManager, const Utilities::AABB& area, size_t depth, size_t maxDepth);
				~Tree();

				void setStatsManager(Utilities::StatsManager* manager);

				Tree &getChild(size_t index);
				void instantiateChilds();
				bool hasChilds();

				void insert(TreeItem& item);
				void search(const Utilities::AABB& area, QSFML::list< Objects::GameObjectPtr>& container) const;
				void clear();
				bool shrink(); // returns false if this can be deleted

				const Utilities::AABB& getArea() const;
				size_t getMaxDepth() const;


				void setupThreadWorker(ThreadWorker* worker);
				void setThreadedParams(QSFML::vector<Utilities::Collisioninfo>* container, bool onlyFisrtCollision);



				void checkCollisions(QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayer(QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelf(QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelfDeep(QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelfDeep(size_t index, QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);

				
			private:
				void checkCollision(Objects::GameObjectPtr other,
					QSFML::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision);
				
				void draw(sf::Text& text, sf::RectangleShape &rect, const sf::Color& color, sf::RenderTarget& target,
					sf::RenderStates states) const;

				void draw(sf::RectangleShape &rect, const sf::Color& color, sf::RenderTarget& target,
					sf::RenderStates states) const;


				Utilities::AABB m_area;
				size_t m_depth;
				size_t m_maxDepth;
				Tree* m_childTrees;
				Utilities::AABB m_childAreas[4];
				bool m_enableChilds;

				QSFML::vector<Objects::GameObjectPtr> m_objects;

				// special collision detection container for multithreading;
				QSFML::vector<Utilities::Collisioninfo>* m_collisions;
				bool m_onlyFirstCollision;
				Utilities::StatsManager* m_statsManager;
			};

			Tree m_tree;
			QSFML::vector<TreeItem> m_allObjs;
			QSFML::unordered_map<Objects::GameObjectPtr, size_t> m_allObjMap;
			ThreadWorker* m_threadWorker;

			// special collision detection container for multithreading;
			QSFML::vector<Utilities::Collisioninfo>* m_collisions1;
			QSFML::vector<Utilities::Collisioninfo>* m_collisions2;
			bool m_onlyFirstCollision;

			Utilities::StatsManager* m_statsManager;

			static sf::Color s_gizmoColor;
			static bool s_gizmoEnableText;
		};
	}
}
