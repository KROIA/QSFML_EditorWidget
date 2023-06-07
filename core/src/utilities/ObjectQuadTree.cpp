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
		ObjectQuadTree::~ObjectQuadTree()
		{
			for (size_t i = 0; i < m_painters.size(); ++i)
			{
				m_painters[i]->m_tree = nullptr;
				m_painters[i]->deleteThis();
			}
				
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
		void ObjectQuadTree::shrink()
		{
			m_tree.shrink();
		}

		void ObjectQuadTree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
											 bool onlyFirstCollision)
		{
			m_tree.checkCollisions(collisions, onlyFirstCollision);
			
			/*for (auto& objStruct : m_allObjs)
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
						objCollider->checkCollision_noAABB(otherColliders, collisions, onlyFirstCollision);
					}
				}
			}*/
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
					if(m_childAreas[i].intersects(area))
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
		bool ObjectQuadTree::Tree::shrink()
		{
			bool anyContent = false;
			if (m_childTrees)
			{
				anyContent |= m_childTrees[0].shrink();
				anyContent |= m_childTrees[1].shrink();
				anyContent |= m_childTrees[2].shrink();
				anyContent |= m_childTrees[3].shrink();
			}
			if (!anyContent)
			{
				delete[] m_childTrees;
				m_childTrees = nullptr;
			}

			return m_objects.size() || anyContent;
		}
		void ObjectQuadTree::Tree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
												   bool onlyFirstCollision)
		{
			// Iterate over the objects and call checkCollision
			for (auto it1 = m_objects.begin(); it1 != m_objects.end(); ++it1)
			{
				//(*m_objects.begin())->setEnabled(false);
				for (auto it2 = std::next(it1); it2 != m_objects.end(); ++it2)
				{
					//(*it2)->setEnabled(false);
					(*it1)->checkCollision(*it2, collisions, onlyFirstCollision);
				}
			}

			if (!m_childTrees)
				return;

			Tree& tree0 = m_childTrees[0];
			Tree& tree1 = m_childTrees[1];
			Tree& tree2 = m_childTrees[2]; 
			Tree& tree3 = m_childTrees[3];
			for (auto objA : m_objects)
			{
				if (objA->getBoundingBox().intersects(m_childAreas[0]))
				{
					tree0.checkCollision(objA, collisions, onlyFirstCollision);
				}
				if (objA->getBoundingBox().intersects(m_childAreas[1]))
				{
					tree1.checkCollision(objA, collisions, onlyFirstCollision);
				}
				if (objA->getBoundingBox().intersects(m_childAreas[2]))
				{
					tree2.checkCollision(objA, collisions, onlyFirstCollision);
				}
				if (objA->getBoundingBox().intersects(m_childAreas[3]))
				{
					tree3.checkCollision(objA, collisions, onlyFirstCollision);
				}
			}
			tree0.checkCollisions(collisions, onlyFirstCollision);
			tree1.checkCollisions(collisions, onlyFirstCollision);
			tree2.checkCollisions(collisions, onlyFirstCollision);
			tree3.checkCollisions(collisions, onlyFirstCollision);
		
		}
		void ObjectQuadTree::Tree::checkCollision(Objects::CanvasObject* other,
			std::vector<Utilities::Collisioninfo>& collisions,
			bool onlyFirstCollision)
		{
			for (auto obj : m_objects)
			{
				other->checkCollision(obj, collisions, onlyFirstCollision);
			}
			if (!m_childTrees)
				return;
			m_childTrees[0].checkCollision(other, collisions, onlyFirstCollision);
			m_childTrees[1].checkCollision(other, collisions, onlyFirstCollision);
			m_childTrees[2].checkCollision(other, collisions, onlyFirstCollision);
			m_childTrees[3].checkCollision(other, collisions, onlyFirstCollision);
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
		void ObjectQuadTree::Tree::draw(const sf::Font& font, const sf::Color& color, sf::RenderTarget& target,
			sf::RenderStates states) const
		{
			sf::RectangleShape rect;
			rect.setPosition(m_area.TL());
			rect.setSize(m_area.getSize());
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			
			rect.setOutlineColor(color);
			rect.setOutlineThickness(0.5);

			sf::Text text;
			text.setFont(font); // font is a sf::Font
			text.setString(std::to_string(m_objects.size()));
			text.setPosition((m_area.TL() + m_area.BR())*0.5f);
			text.setScale(sf::Vector2f(0.08, 0.08));
			text.setCharacterSize(40);

			if (m_childTrees)
			{
				float fade = 0.7;
				//sf::Color col(color.r * fade, color.g * fade, color.b * fade, color.a * fade);
				sf::Color col = color;
				col.a *= fade;
				m_childTrees[0].draw(font, col, target, states);
				m_childTrees[1].draw(font, col, target, states);
				m_childTrees[2].draw(font, col, target, states);
				m_childTrees[3].draw(font, col, target, states);
			}
			target.draw(rect);
			target.draw(text);
		}

		ObjectQuadTree::ObjectQuadTreePainter* ObjectQuadTree::createPainter()
		{
			ObjectQuadTreePainter* painter = new ObjectQuadTreePainter(this);
			m_painters.push_back(painter);
			painter->setColor(sf::Color::Blue);
			return painter;
		}
		void ObjectQuadTree::removePainter(ObjectQuadTreePainter* painter)
		{
			for (size_t i = 0; i < m_painters.size(); ++i)
			{
				if (m_painters[i] == painter)
				{
					m_painters.erase(m_painters.begin() + i);
					return;
				}
			}
		}

		ObjectQuadTree::ObjectQuadTreePainter::ObjectQuadTreePainter(ObjectQuadTree* tree, const std::string& name)
			: Drawable(name)
			, m_tree(tree)
		{

		}
		ObjectQuadTree::ObjectQuadTreePainter::ObjectQuadTreePainter(const ObjectQuadTreePainter& other)
			: Drawable(other)
			, m_tree(other.m_tree)
		{

		}
		COMPONENT_IMPL(ObjectQuadTree::ObjectQuadTreePainter);
		ObjectQuadTree::ObjectQuadTreePainter::~ObjectQuadTreePainter()
		{
			if (m_tree)
				m_tree->removePainter(this);
		}
		void ObjectQuadTree::ObjectQuadTreePainter::setColor(const sf::Color& color)
		{
			m_color = color;
		}
		const sf::Color& ObjectQuadTree::ObjectQuadTreePainter::getColor() const
		{
			return m_color;
		}
		void ObjectQuadTree::ObjectQuadTreePainter::draw(sf::RenderTarget& target,
			sf::RenderStates states) const
		{
			if (m_tree)
				m_tree->m_tree.draw(getTextFont(), m_color, target, states);
		}
		void ObjectQuadTree::ObjectQuadTreePainter::destroy()
		{
			deleteThis();
		}
	}
}