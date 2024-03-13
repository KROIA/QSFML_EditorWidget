#pragma once

#include "QSFML_EditorWidget_base.h"

#include "objects/base/CanvasObject.h"
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
		class QSFML_EDITOR_WIDGET_EXPORT ObjectQuadTree
		{
			class QSFML_EDITOR_WIDGET_EXPORT Tree;
		public:
			struct QSFML_EDITOR_WIDGET_EXPORT TreeItem
			{
				friend ObjectQuadTree;
				friend Tree;
				TreeItem(Objects::CanvasObject* obj)
					: obj(obj)
					, containter(nullptr)
				{}
				TreeItem()
					: obj(nullptr)
					, containter(nullptr)
				{}
				Objects::CanvasObject* obj;
			private:
				std::list<Objects::CanvasObject*>* containter;
				std::list<Objects::CanvasObject*>::iterator iterator;
			};
			ObjectQuadTree(StatsManager* statsManager, const Utilities::AABB& area, size_t maxDepth = 10);
			ObjectQuadTree(const ObjectQuadTree& other) = delete;
			~ObjectQuadTree();

			void setStatsManager(StatsManager* manager);

			bool insert(Objects::CanvasObject* obj);
			bool insert(const std::vector<Objects::CanvasObject*>& objs);

			void search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const;
			const std::list<TreeItem>& getAllItems() const;
			void remove(Objects::CanvasObject* obj);
			void remove(const TreeItem& item);
			bool relocate(TreeItem& item);
			void clear();
			void shrink(); // removes not used childs

			const Utilities::AABB& getArea() const;
			size_t getMaxDepth() const;

			void enableCollisionThreads(bool enable);
			void checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
								 bool onlyFirstCollision = true);


			class QSFML_EDITOR_WIDGET_EXPORT ObjectQuadTreePainter : public Components::Drawable
			{
				friend ObjectQuadTree;
				ObjectQuadTreePainter(ObjectQuadTree* tree, const std::string& name = "ObjectQuadTreePainter");
				ObjectQuadTreePainter(const ObjectQuadTreePainter& other);
			public:
				COMPONENT_DECL(ObjectQuadTreePainter);
				~ObjectQuadTreePainter();

				void assignTree(ObjectQuadTree* tree);
				ObjectQuadTree* getTree() const;

				void setColor(const sf::Color& color);
				const sf::Color& getColor() const;

				void draw(sf::RenderTarget& target,
					sf::RenderStates states) const override;

				void destroy();
			private:

				ObjectQuadTree* m_tree;
				sf::Color m_color;
			};

			ObjectQuadTreePainter* createPainter();
			void removePainter(ObjectQuadTreePainter* painter);
			void assignPainter(ObjectQuadTreePainter* painter);

		private:
			bool insert_internal(TreeItem& item);
			class QSFML_EDITOR_WIDGET_EXPORT Tree
			{
				friend ObjectQuadTreePainter;
			public:
				Tree(StatsManager* statsManager, const Utilities::AABB& area, size_t depth, size_t maxDepth);
				~Tree();

				void setStatsManager(StatsManager* manager);

				Tree &getChild(size_t index);
				void instantiateChilds();
				bool hasChilds();

				void insert(TreeItem& item);
				void search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const;
				void clear();
				bool shrink(); // returns false if this can be deleted

				const Utilities::AABB& getArea() const;
				size_t getMaxDepth() const;


				void setupThreadWorker(ThreadWorker* worker);
				void setThreadedParams(std::vector<Utilities::Collisioninfo>* container, bool onlyFisrtCollision);



				void checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayer(std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelf(std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelfDeep(std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);
				void checkCollisionsSingleLayerSelfDeep(size_t index, std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision = true);

				
			private:
				void checkCollision(Objects::CanvasObject* other,
					std::vector<Utilities::Collisioninfo>& collisions,
					bool onlyFirstCollision);
				
				void draw(const sf::Font& font, const sf::Color& color, sf::RenderTarget& target,
					sf::RenderStates states) const;


				Utilities::AABB m_area;
				size_t m_depth;
				size_t m_maxDepth;
				Tree* m_childTrees;
				Utilities::AABB m_childAreas[4];
				bool m_enableChilds;

				std::list<Objects::CanvasObject*> m_objects;

				// special collision detection container for multithreading;
				std::vector<Utilities::Collisioninfo>* m_collisions;
				bool m_onlyFirstCollision;
				StatsManager* m_statsManager;
			};

			Tree m_tree;
			std::list<TreeItem> m_allObjs;
			std::unordered_map<Objects::CanvasObject*, size_t> m_allObjMap;
			std::vector<ObjectQuadTreePainter*> m_painters;
			ThreadWorker* m_threadWorker;

			// special collision detection container for multithreading;
			std::vector<Utilities::Collisioninfo>* m_collisions1;
			std::vector<Utilities::Collisioninfo>* m_collisions2;
			bool m_onlyFirstCollision;

			StatsManager* m_statsManager;
		};
	}
}
