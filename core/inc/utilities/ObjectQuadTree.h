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
		class QSFML_EDITOR_WIDGET_EXPORT ObjectQuadTree
		{
			class QSFML_EDITOR_WIDGET_EXPORT Tree;
		public:
			struct QSFML_EDITOR_WIDGET_EXPORT TreeItem
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
				std::list<Objects::GameObjectPtr>* containter;
				std::list<Objects::GameObjectPtr>::iterator iterator;
			};
			ObjectQuadTree(Utilities::StatsManager* statsManager, const Utilities::AABB& area, size_t maxDepth = 10);
			ObjectQuadTree(const ObjectQuadTree& other) = delete;
			~ObjectQuadTree();

			void setStatsManager(Utilities::StatsManager* manager);

			bool insert(Objects::GameObjectPtr obj);
			bool insert(const std::vector<Objects::GameObjectPtr>& objs);

			void search(const Utilities::AABB& area, std::list< Objects::GameObjectPtr>& container) const;
			const std::list<TreeItem>& getAllItems() const;
			void remove(Objects::GameObjectPtr obj);
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

				void enableText(bool enable)
				{
					m_enableText = enable;
				}

				void assignTree(ObjectQuadTree* tree);
				ObjectQuadTree* getTree() const;

				void setColor(const sf::Color& color);
				const sf::Color& getColor() const;

				//void drawComponent(sf::RenderTarget& target,
				//	sf::RenderStates states) const override;
				void ObjectQuadTree::ObjectQuadTreePainter::drawComponent(sf::RenderTarget& target,
					sf::RenderStates states) const override
				{
					if (m_tree)
					{
						sf::RectangleShape rect;
						//rect.setPosition(m_area.TL());
						//rect.setSize(m_area.getSize());
						rect.setFillColor(sf::Color(0, 0, 0, 0));

						rect.setOutlineColor(m_color);
						rect.setOutlineThickness(0.5);
						if (m_enableText)
						{
							sf::Text text;
							text.setFont(getTextFont()); // font is a sf::Font
							text.setScale(sf::Vector2f(0.08, 0.08));
							text.setCharacterSize(40);
							m_tree->m_tree.draw(text, rect, m_color, target, states);
						}
						else
							m_tree->m_tree.draw(rect, m_color, target, states);
					}
				}

			private:
				bool m_enableText = true;
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
				Tree(Utilities::StatsManager* statsManager, const Utilities::AABB& area, size_t depth, size_t maxDepth);
				~Tree();

				void setStatsManager(Utilities::StatsManager* manager);

				Tree &getChild(size_t index);
				void instantiateChilds();
				bool hasChilds();

				void insert(TreeItem& item);
				void search(const Utilities::AABB& area, std::list< Objects::GameObjectPtr>& container) const;
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
				void checkCollision(Objects::GameObjectPtr other,
					std::vector<Utilities::Collisioninfo>& collisions,
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

				std::list<Objects::GameObjectPtr> m_objects;

				// special collision detection container for multithreading;
				std::vector<Utilities::Collisioninfo>* m_collisions;
				bool m_onlyFirstCollision;
				Utilities::StatsManager* m_statsManager;
			};

			Tree m_tree;
			std::list<TreeItem> m_allObjs;
			std::unordered_map<Objects::GameObjectPtr, size_t> m_allObjMap;
			std::vector<ObjectQuadTreePainter*> m_painters;
			ThreadWorker* m_threadWorker;

			// special collision detection container for multithreading;
			std::vector<Utilities::Collisioninfo>* m_collisions1;
			std::vector<Utilities::Collisioninfo>* m_collisions2;
			bool m_onlyFirstCollision;

			Utilities::StatsManager* m_statsManager;
		};
	}
}
