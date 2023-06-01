#include "utilities/ObjectQuadTree.h"
#include "objects/base/CanvasObject.h"
#include "components/physics/Collider.h"

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
		bool ObjectQuadTree::insert(const std::vector<Objects::CanvasObject*> &objs)
		{
			bool success = true;
			for (auto obj : objs)
			{
				TreeItem item(obj);
				success &= insert_internal(item);
			}
			return success;
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
		void ObjectQuadTree::clear()
		{
			m_allObjs.clear();
			m_allObjMap.clear();
			m_tree.clear();
		}

		void ObjectQuadTree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
											 bool onlyFirstCollision)
		{
			//m_tree.checkCollisions(collisions, onlyFirstCollision);
			for (auto& objStruct : m_allObjs)
			{
				Objects::CanvasObject* obj = objStruct.obj;
				std::list< QSFML::Objects::CanvasObject*> possibleColliders;
				m_tree.search(obj->getBoundingBox(), possibleColliders);
				for (auto it : possibleColliders)
				{
					if (obj == it)
						continue;

					const std::vector<Components::Collider*>& otherColliders = it->getCollider();
					const std::vector<Components::Collider*>& collider = obj->getCollider();
					for (auto objCollider : collider)
					{
						objCollider->checkCollision(otherColliders, collisions, onlyFirstCollision);
					}
				}
			}
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
				Utilities::AABB(m_area.TL(), (m_area.BR() - m_area.TL()) * 0.5f),
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
					if(m_childAreas[i].contains(area))
						m_childTrees[i].search(area, container);
				}
			}
		}
		void ObjectQuadTree::Tree::clear()
		{
			m_objects.clear();
			if (m_childTrees)
			{
				#pragma unroll
				for (size_t i = 0; i < 4; ++i)
				{
					m_childTrees[i].clear();
				}
			}
		}
		void ObjectQuadTree::Tree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
												   bool onlyFirstCollision)
		{

			/*for (size_t i = 0; i<m_objects.size(); ++i)
			{
				std::list<Objects::CanvasObject*>::iterator objA = m_objects.begin();
				std::advance(objA, i);
				for (size_t j = i+1; j < m_objects.size(); ++j)
				{
					std::list<Objects::CanvasObject*>::iterator objB = m_objects.begin();
					std::advance(objB, j);
					
					(*objA)->checkCollision(*objB, collisions, onlyFirstCollision);
				}
			}
			if (!m_childTrees)
				return;
			#pragma unroll
			for (size_t k = 0; k < 4; ++k)
			{
				Tree& tree = m_childTrees[k];
				for (auto objA : m_objects)
				{
					for (auto objB : tree.m_objects)
					{
						objA->checkCollision(objB, collisions, onlyFirstCollision);
					}
				}
			}*/
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