#pragma once

#include "QSFML_base.h"

#include "objects/base/CanvasObject.h"

#include "AABB.h"

#include <vector>
#include <list>
#include <unordered_map>

namespace QSFML
{
	namespace Utilities
	{
		class ObjectQuadTree
		{
			class Tree;
		public:
			struct TreeItem
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
			ObjectQuadTree(const Utilities::AABB& area, size_t maxDepth = 10);
			bool insert(Objects::CanvasObject* obj);
			bool insert(const std::vector<Objects::CanvasObject*> &objs);
			
			void search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const;
			const std::list<TreeItem>& getAllItems() const;
			void remove(Objects::CanvasObject* obj);
			void remove(const TreeItem& item);
			bool relocate(TreeItem& item);
			void clear();

			void checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
								 bool onlyFirstCollision = true);

		private:
			bool insert_internal(TreeItem& item);
			class Tree
			{
				
			public:
				Tree(const Utilities::AABB& area, size_t depth, size_t maxDepth);
				~Tree();

				void insert(TreeItem& item);
				void search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const;
				void clear();

				void checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
									 bool onlyFirstCollision = true);

			private:
				void instantiateChilds();

				Utilities::AABB m_area;
				size_t m_depth;
				size_t m_maxDepth;
				Tree* m_childTrees;
				Utilities::AABB m_childAreas[4];

				std::list<Objects::CanvasObject*> m_objects;
			};

			Tree m_tree;
			std::list<TreeItem> m_allObjs;
			std::unordered_map<Objects::CanvasObject*, size_t> m_allObjMap;
		};
		


/*
		template <typename T>
		struct QuadTreeItemLocation
		{
			typename std::list<std::pair<AABB, T>>* container = nullptr;
			typename std::list<std::pair<AABB, T>>::iterator iterator;
		};

		template <typename pT>
		class DynamicQuadTree
		{
		public:
			DynamicQuadTree(const AABB& size, const size_t nDepth = 0, const size_t nMaxDepth = 8)
			{
				m_depth = nDepth;
				m_rect = size;
				m_maxdepth = nMaxDepth;
				resize(m_rect);
			}

			// Insert a region into this area
			QuadTreeItemLocation<pT> insert(const pT item, const AABB& itemsize)
			{
				for (int i = 0; i < 4; i++)
				{
					if (geom2d::contains(m_rChild[i], itemsize))
					{
						// Have we reached depth limit?
						if (m_depth + 1 < m_maxdepth)
						{
							// No, so does child exist?
							if (!m_pChild[i])
							{
								// No, so create it
								m_pChild[i] = std::make_shared<DynamicQuadTree<pT>>(m_rChild[i], m_depth + 1, m_maxdepth);
							}

							// Yes, so add item to it
							return m_pChild[i]->insert(item, itemsize);
						}
					}
				}

				// It didnt fit, so item must belong to this AABB
				m_pItems.push_back({ itemsize, item });
				return { &m_pItems, std::prev(m_pItems.end()) };
			}

			void relocate(pT item, const AABB& rArea)
			{
				// Remove it
				remove(item);

				// Reinsert it with new location
				insert(item, rArea);
			}

			size_t size() const
			{
				size_t nCount = m_pItems.size();
				for (int i = 0; i < 4; i++)
					if (m_pChild[i]) nCount += m_pChild[i]->size();
				return nCount;
			}

			void search(const AABB& rArea, std::list<pT>& listItems) const
			{
				// First, check for items belonging to this area, add them to the list
				// if there is overlap
				for (const auto& p : m_pItems)
				{
					if (rArea.intersects(p.first))
						listItems.push_back(p.second);
				}

				// Second, recurse through children and see if they can
				// add to the list
				for (int i = 0; i < 4; i++)
				{
					if (m_pChild[i])
					{
						// If child is entirely contained within area, recursively
						// add all of its children, no need to check boundaries
						if (rArea.contains(m_rChild[i]))
							m_pChild[i]->items(listItems);

						// If child overlaps with search area then checks need
						// to be made
						else if (m_rChild[i].intersects(rArea))
							m_pChild[i]->search(rArea, listItems);
					}
				}


			}

			void items(std::list<pT>& listItems) const
			{
				// No questions asked, just return child items
				for (const auto& p : m_pItems)
					listItems.push_back(p.second);

				for (int i = 0; i < 4; i++)
					if (m_pChild[i]) m_pChild[i]->items(listItems);
			}

			void clear()
			{
				m_pItems.clear();

				for (int i = 0; i < 4; i++)
				{
					if (m_pChild[i])
						m_pChild[i]->clear();
					else
						m_pChild[i].reset();
				}
			}

			void resize(const AABB& rArea)
			{
				clear();
				m_rect = rArea;
				sf::Vector2f vChildSize = m_rect.getSize() / 2.f;
				m_rChild =
				{
					AABB(m_rect.TL(), vChildSize),
					AABB({m_rect.TL().x + vChildSize.x, m_rect.TL().y}, vChildSize),
					AABB({m_rect.TL().x, m_rect.TL().y + vChildSize.y}, vChildSize),
					AABB(m_rect.TL() + vChildSize, vChildSize)
				};
			}


			const AABB& area()
			{
				return m_rect;
			}

		protected:
			size_t m_depth = 0;
			size_t m_maxdepth = 8;

			// Area of this quadnode
			AABB m_rect;

			// 4 child areas of this quadnode
			std::array<AABB, 4> m_rChild{};

			// 4 potential children of this quadnode
			std::array<std::shared_ptr<DynamicQuadTree<pT>>, 4> m_pChild{};

			// Items which belong to this quadnode
			std::list<std::pair<AABB, pT>> m_pItems;

		};

		template<typename T>
		struct QuadTreeItem
		{
			// The item Itself
			T item;

			// A "locator" to the container/iterator that points to this item's iterator in the
			// top level list - phew
			QuadTreeItemLocation<typename std::list<QuadTreeItem<T>>::iterator> pItem;
		};

		template <typename T>
		class QuadTreeContainer
		{
			using IQuadtreeContainer = std::list<QuadTreeItem<T>>;

		public:
			QuadTreeContainer(const AABB& size = { {0.0f, 0.0f}, { 100.0f, 100.0f } }, 
							  const size_t nDepth = 0, 
							  const size_t nMaxDepth = 8) 
				: root(size, nDepth, nMaxDepth)
			{

			}

			// Sets the spatial coverage area of teh quadtree
			void resize(const AABB& rArea)
			{
				root.resize(rArea);
			}

			// Inserts an item into the quadtree
			void insert(const T& item, const AABB& itemsize)
			{
				QuadTreeItem<T> newItem;
				newItem.item = item;

				// Item i stored in container
				m_allItems.emplace_back(newItem);

				// Pointer/Area of item is stored in AABB tree
				m_allItems.back().pItem = root.insert(std::prev(m_allItems.end()), itemsize);
			}

			// Returns a std::list of pointers to items within the search area
			std::list<typename IQuadtreeContainer::iterator> search(const AABB& rArea) const
			{
				std::list<typename IQuadtreeContainer::iterator> listItemPointers;
				root.search(rArea, listItemPointers);
				return listItemPointers;
			}

			void remove(typename IQuadtreeContainer::iterator& item)
			{
				// Iterator points to a QuadTreeItem
				item->pItem.container->erase(item->pItem.iterator);

				// Remove actual item from container
				m_allItems.erase(item);
			}

			void relocate(typename IQuadtreeContainer::iterator& item, const AABB& itemsize)
			{
				// Remove pointer to item from whichever container its stored in
				item->pItem.container->erase(item->pItem.iterator);

				// Update the items pointer by reinsertion into AABB tree
				item->pItem = root.insert(item, itemsize);

			}

			typename IQuadtreeContainer::iterator begin()
			{
				return m_allItems.begin();
			}

			typename IQuadtreeContainer::iterator end()
			{
				return m_allItems.end();
			}

			typename IQuadtreeContainer::const_iterator cbegin()
			{
				return m_allItems.cbegin();
			}

			typename IQuadtreeContainer::const_iterator cend()
			{
				return m_allItems.cend();
			}

			size_t size() const
			{
				return root.size();
			}

			void clear()
			{
				m_allItems.clear();
				root.clear();
			}

			const AABB& area()
			{
				return root.area();
			}

		protected:
			DynamicQuadTree<typename IQuadtreeContainer::iterator> root;
			IQuadtreeContainer m_allItems;
		};*/

	}
}