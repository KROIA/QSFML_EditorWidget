#include "utilities/ObjectQuadTree.h"

namespace QSFML
{
	namespace Utilities
	{
		ObjectQuadTree::ObjectQuadTree(const Utilities::AABB& area, size_t maxDepth)
			: m_tree(area, 0, maxDepth)
		{

		}
		bool ObjectQuadTree::insert(Objects::CanvasObject* obj)
		{
			if (m_allObjMap.find(obj) != m_allObjMap.end())
				return false; // Object already in container
			TreeItem item(obj);
			return insert_internal(item);
		}
		bool ObjectQuadTree::insert_internal(TreeItem& item)
		{
			m_tree.insert(item);
			if (!item.containter)
				return false; // Object can't be added to the tree

			m_allObjMap[item.obj] = m_allObjs.size();
			m_allObjs.push_back(item);
			return true;
		}
		void ObjectQuadTree::search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const
		{
			m_tree.search(area, container);
		}
		const std::list<ObjectQuadTree::TreeItem>& ObjectQuadTree::getAllItems() const
		{
			return m_allObjs;
		}
		void ObjectQuadTree::remove(Objects::CanvasObject* obj)
		{
			auto mapIt = m_allObjMap.find(obj);
			if (mapIt == m_allObjMap.end())
				return; // Object not in container

			auto it = m_allObjs.begin();
			std::advance(it, m_allObjMap[obj]);
			it->containter->erase(it->iterator);
			m_allObjs.erase(it);
			m_allObjMap.erase(mapIt);
		}
		void ObjectQuadTree::remove(const TreeItem& item)
		{
			if (item.containter)
			{
				auto mapIt = m_allObjMap.find(item.obj);
				if (mapIt == m_allObjMap.end())
					return; // Object not in container

				item.containter->erase(item.iterator);
				auto it = m_allObjs.begin();
				std::advance(it, m_allObjMap[item.obj]);
				m_allObjs.erase(it);
				m_allObjMap.erase(mapIt);
			}
		}
		bool ObjectQuadTree::relocate(TreeItem& item)
		{
			remove(item);
			item.containter = nullptr;
			return insert_internal(item);
		}


		//
		// TREE
		//

		ObjectQuadTree::Tree::Tree(const Utilities::AABB& area, size_t depth, size_t maxDepth)
			: m_area(area)
			, m_depth(depth)
			, m_maxDepth(maxDepth)
			, m_childTrees(nullptr)
			, m_childAreas{
				Utilities::AABB(m_area.TL(), m_area.BR() - m_area.TL()),
				Utilities::AABB(m_childAreas[0].TR(), m_childAreas[0].getSize()),
				Utilities::AABB(m_childAreas[0].BL(), m_childAreas[0].getSize()),
				Utilities::AABB(m_childAreas[0].BR(), m_childAreas[0].getSize())
			}
		{

		}
		ObjectQuadTree::Tree::~Tree()
		{
			if (m_childTrees)
				delete[] m_childTrees;
		}

		void ObjectQuadTree::Tree::insert(TreeItem& item)
		{
			AABB objBB = item.obj->getBoundingBox();
			if (m_area.contains(objBB))
			{
				#pragma unroll
				for (size_t i = 0; i < 4; ++i)
				{
					if (m_childAreas[i].contains(objBB))
					{
						if (!m_childTrees)
							instantiateChilds();
						m_childTrees[i].insert(item);
						return;
					}
				}

				m_objects.push_back(item.obj);
				item.containter = &m_objects;
				item.iterator = m_objects.end();
			}
		}
		void ObjectQuadTree::Tree::search(const Utilities::AABB& area, std::list< Objects::CanvasObject*>& container) const
		{
			for (auto obj : m_objects)
			{
				if (obj->getBoundingBox().intersects(area))
					container.push_back(obj);
			}
			if (m_childTrees)
			{
				#pragma unroll
				for (size_t i = 0; i < 4; ++i)
				{
					m_childTrees[i].search(area, container);
				}
			}
		}
		void ObjectQuadTree::Tree::instantiateChilds()
		{
			size_t newDepth = m_depth + 1;

			m_childTrees = new Tree[4]{
				Tree(m_childAreas[0], newDepth, m_maxDepth),
				Tree(m_childAreas[1], newDepth, m_maxDepth),
				Tree(m_childAreas[2], newDepth, m_maxDepth),
				Tree(m_childAreas[3], newDepth, m_maxDepth),
			};

		}
	}
}