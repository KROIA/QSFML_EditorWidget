#include "utilities/ObjectQuadTree.h"
#include "utilities/Stats.h"
#include "objects/base/GameObject.h"
#include "components/physics/Collider.h"
#include "utilities/Stats.h"


namespace QSFML
{
	namespace Utilities
	{
		ObjectQuadTree::ObjectQuadTree(Utilities::StatsManager *statsManager, const Utilities::AABB& area, size_t maxDepth)
			: m_tree(statsManager, area, 0, maxDepth)
			, m_threadWorker(nullptr)
			, m_statsManager(statsManager)
		{

		}
		ObjectQuadTree::~ObjectQuadTree()
		{
			enableCollisionThreads(false);
			for (size_t i = 0; i < m_painters.size(); ++i)
			{
				m_painters[i]->m_tree = nullptr;
				m_painters[i]->deleteLater();
			}
		}
		void ObjectQuadTree::setStatsManager(Utilities::StatsManager* manager)
		{
			m_statsManager = manager;
			m_tree.setStatsManager(manager);
		}
		bool ObjectQuadTree::insert(Objects::GameObjectPtr obj)
		{
			if (m_allObjMap.find(obj) != m_allObjMap.end())
				return false; // Object already in container
			TreeItem item(obj);
			return insert_internal(item);
		}

		bool ObjectQuadTree::insert(const std::vector<Objects::GameObjectPtr> &objs)
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
		void ObjectQuadTree::search(const Utilities::AABB& area, std::list< Objects::GameObjectPtr>& container) const
		{
			m_tree.search(area, container);
		}
		const std::list<ObjectQuadTree::TreeItem>& ObjectQuadTree::getAllItems() const
		{
			return m_allObjs;
		}
		void ObjectQuadTree::remove(Objects::GameObjectPtr obj)
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
			if (m_threadWorker)
			{
				m_tree.getChild(0).shrink();
				m_tree.getChild(1).shrink();
				m_tree.getChild(2).shrink();
				m_tree.getChild(3).shrink();
			}
			else
				m_tree.shrink();
		}
		const Utilities::AABB& ObjectQuadTree::getArea() const
		{
			return m_tree.getArea();
		}
		size_t ObjectQuadTree::getMaxDepth() const
		{
			return m_tree.getMaxDepth();
		}

		void ObjectQuadTree::enableCollisionThreads(bool enable)
		{
			if (m_threadWorker && enable || !m_threadWorker && !enable)
				return;
			if (!enable)
			{
				delete m_threadWorker;
				m_threadWorker = nullptr;
				if (m_tree.hasChilds())
				{
					m_tree.getChild(0).setThreadedParams(nullptr, false);
					m_tree.getChild(1).setThreadedParams(nullptr, false);
					m_tree.getChild(2).setThreadedParams(nullptr, false);
					m_tree.getChild(3).setThreadedParams(nullptr, false);
				}
				return;
			}

			m_threadWorker = new ThreadWorker("ObjectQuadTree Worker");
			m_tree.instantiateChilds();
			m_tree.getChild(0).setupThreadWorker(m_threadWorker);
			m_tree.getChild(1).setupThreadWorker(m_threadWorker);
			m_tree.getChild(2).setupThreadWorker(m_threadWorker);
			m_tree.getChild(3).setupThreadWorker(m_threadWorker);


		m_threadWorker->addFunction([this] {
			this->m_tree.checkCollisionsSingleLayerSelfDeep(0, *this->m_collisions1, this->m_onlyFirstCollision);
			this->m_tree.checkCollisionsSingleLayerSelfDeep(1, *this->m_collisions1, this->m_onlyFirstCollision);
			});
		m_threadWorker->addFunction([this] {
			this->m_tree.checkCollisionsSingleLayerSelfDeep(2, *this->m_collisions2, this->m_onlyFirstCollision);
			this->m_tree.checkCollisionsSingleLayerSelfDeep(3, *this->m_collisions2, this->m_onlyFirstCollision);
			});
			
		}
		void ObjectQuadTree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
											 bool onlyFirstCollision)
		{
			QSFMLP_PHYSICS_FUNCTION(QSFML_COLOR_STAGE_1);
			if (m_threadWorker)
			{
				std::vector<Utilities::Collisioninfo> sector1;
				std::vector<Utilities::Collisioninfo> sector2;
				std::vector<Utilities::Collisioninfo> sector3;
				std::vector<Utilities::Collisioninfo> sector4;
				std::vector<Utilities::Collisioninfo> sector5;
				std::vector<Utilities::Collisioninfo> sector6;
				m_collisions1 = &sector5;
				m_collisions2 = &sector6;
				m_onlyFirstCollision = onlyFirstCollision;
				m_tree.getChild(0).setThreadedParams(&sector1 , onlyFirstCollision);
				m_tree.getChild(1).setThreadedParams(&sector2 , onlyFirstCollision);
				m_tree.getChild(2).setThreadedParams(&sector3 , onlyFirstCollision);
				m_tree.getChild(3).setThreadedParams(&sector4 , onlyFirstCollision);

				m_threadWorker->signalExecution();
				m_tree.checkCollisionsSingleLayerSelf(collisions, onlyFirstCollision);

				//this->m_tree.checkCollisionsSingleLayerSelfDeep(0, *this->m_collisions1, this->m_onlyFirstCollision);
				//this->m_tree.checkCollisionsSingleLayerSelfDeep(1, *this->m_collisions1, this->m_onlyFirstCollision);
				//
				//
				//this->m_tree.checkCollisionsSingleLayerSelfDeep(2, *this->m_collisions2, this->m_onlyFirstCollision);
				//this->m_tree.checkCollisionsSingleLayerSelfDeep(3, *this->m_collisions2, this->m_onlyFirstCollision);
				
				m_threadWorker->waitForThreadsNoDelayNoExec();
				QSFMLP_PHYSICS_BLOCK("Collisions copy", QSFML_COLOR_STAGE_2);
				size_t cap = collisions.size() + sector1.size() + sector2.size() + sector3.size() + sector4.size() + sector5.size() + sector6.size();
				collisions.reserve(cap);
				collisions.insert(collisions.end(), sector1.begin(), sector1.end());
				collisions.insert(collisions.end(), sector2.begin(), sector2.end());
				collisions.insert(collisions.end(), sector3.begin(), sector3.end());
				collisions.insert(collisions.end(), sector4.begin(), sector4.end());
				collisions.insert(collisions.end(), sector5.begin(), sector5.end());
				collisions.insert(collisions.end(), sector6.begin(), sector6.end());
				QSFMLP_PHYSICS_END_BLOCK;
			}
			else
			{
				m_tree.checkCollisions(collisions, onlyFirstCollision);
			}
		}


		//
		// TREE
		//

		ObjectQuadTree::Tree::Tree(Utilities::StatsManager* statsManager, const Utilities::AABB& area, size_t depth, size_t maxDepth)
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
			, m_collisions(nullptr)
			, m_statsManager(statsManager)
		{
			if (m_maxDepth < 1)
				m_maxDepth = 1;
			m_enableChilds = m_depth < m_maxDepth;
		}
		ObjectQuadTree::Tree::~Tree()
		{
			if (m_childTrees)
				delete[] m_childTrees;
		}
		void ObjectQuadTree::Tree::setStatsManager(Utilities::StatsManager* manager)
		{
			m_statsManager = manager;
			if (m_childTrees)
			{
				m_childTrees[0].setStatsManager(manager);
				m_childTrees[1].setStatsManager(manager);
				m_childTrees[2].setStatsManager(manager);
				m_childTrees[3].setStatsManager(manager);
			}
		}
		ObjectQuadTree::Tree& ObjectQuadTree::Tree::getChild(size_t index)
		{
			return m_childTrees[index];
		}
		void ObjectQuadTree::Tree::instantiateChilds()
		{
			if (m_childTrees)
				return;
			size_t newDepth = m_depth + 1;

			m_childTrees = new Tree[4]{
				Tree(m_statsManager, m_childAreas[0], newDepth, m_maxDepth),
				Tree(m_statsManager, m_childAreas[1], newDepth, m_maxDepth),
				Tree(m_statsManager, m_childAreas[2], newDepth, m_maxDepth),
				Tree(m_statsManager, m_childAreas[3], newDepth, m_maxDepth),
			};

		}
		bool ObjectQuadTree::Tree::hasChilds()
		{
			return m_childTrees;
		}

		void ObjectQuadTree::Tree::insert(TreeItem& item)
		{
			AABB objBB = item.obj->getBoundingBox();
			if (m_enableChilds)
			{
				if (m_childAreas[0].contains(objBB))
				{
					if (!m_childTrees)
						instantiateChilds();
					m_childTrees[0].insert(item);
					return;
				}
				if (m_childAreas[1].contains(objBB))
				{
					if (!m_childTrees)
						instantiateChilds();
					m_childTrees[1].insert(item);
					return;
				}
				if (m_childAreas[2].contains(objBB))
				{
					if (!m_childTrees)
						instantiateChilds();
					m_childTrees[2].insert(item);
					return;
				}
				if (m_childAreas[3].contains(objBB))
				{
					if (!m_childTrees)
						instantiateChilds();
					m_childTrees[3].insert(item);
					return;
				}
			}

			m_objects.push_back(item.obj);
			item.containter = &m_objects;
			item.iterator = m_objects.end();
		}
		void ObjectQuadTree::Tree::search(const Utilities::AABB& area, std::list< Objects::GameObjectPtr>& container) const
		{
			for (auto obj : m_objects)
			{
				if (obj->getBoundingBox().intersects(area))
					container.push_back(obj);
			}
			if (m_childTrees)
			{
				if (m_childAreas[0].intersects(area))
					m_childTrees[0].search(area, container);
				if (m_childAreas[1].intersects(area))
					m_childTrees[1].search(area, container);
				if (m_childAreas[2].intersects(area))
					m_childTrees[2].search(area, container);
				if (m_childAreas[3].intersects(area))
					m_childTrees[3].search(area, container);
			}
		}
		void ObjectQuadTree::Tree::clear()
		{
			m_objects.clear();
			if (m_childTrees)
			{
				m_childTrees[0].clear();
				m_childTrees[1].clear();
				m_childTrees[2].clear();
				m_childTrees[3].clear();
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
		
		const Utilities::AABB& ObjectQuadTree::Tree::getArea() const
		{
			return m_area;
		}
		size_t ObjectQuadTree::Tree::getMaxDepth() const
		{
			return m_maxDepth;
		}

		void ObjectQuadTree::Tree::setupThreadWorker(ThreadWorker* worker)
		{
			worker->addFunction([this] {
				this->checkCollisions(*this->m_collisions, this->m_onlyFirstCollision);
				});
		}
		void ObjectQuadTree::Tree::setThreadedParams(std::vector<Utilities::Collisioninfo>* container, bool onlyFisrtCollision)
		{
			m_collisions = container;
			m_onlyFirstCollision = onlyFisrtCollision;
		}

		void ObjectQuadTree::Tree::checkCollisions(std::vector<Utilities::Collisioninfo>& collisions,
												   bool onlyFirstCollision)
		{
			checkCollisionsSingleLayerSelf(collisions, onlyFirstCollision);
			if (!m_childTrees)
				return;

			checkCollisionsSingleLayerSelfDeep(collisions, onlyFirstCollision);
			m_childTrees[0].checkCollisions(collisions, onlyFirstCollision);
			m_childTrees[1].checkCollisions(collisions, onlyFirstCollision);
			m_childTrees[2].checkCollisions(collisions, onlyFirstCollision);
			m_childTrees[3].checkCollisions(collisions, onlyFirstCollision);
		
		}
		void ObjectQuadTree::Tree::checkCollisionsSingleLayer(std::vector<Utilities::Collisioninfo>& collisions,
														      bool onlyFirstCollision)
		{
			checkCollisionsSingleLayerSelf(collisions, onlyFirstCollision);
			if (!m_childTrees)
				return;
			checkCollisionsSingleLayerSelfDeep(collisions, onlyFirstCollision);
		}
		void ObjectQuadTree::Tree::checkCollisionsSingleLayerSelf(std::vector<Utilities::Collisioninfo>& collisions,
			bool onlyFirstCollision)
		{
			// Iterate over the objects and call checkCollision
			for (auto it1 = m_objects.begin(); it1 != m_objects.end(); ++it1)
			{
				for (auto it2 = std::next(it1); it2 != m_objects.end(); ++it2)
				{
					(*it1)->checkCollision(*it2, collisions, onlyFirstCollision);
				}
			}
		}
		void ObjectQuadTree::Tree::checkCollisionsSingleLayerSelfDeep(std::vector<Utilities::Collisioninfo>& collisions,
																	  bool onlyFirstCollision)
		{
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
			if(m_statsManager)
				m_statsManager->addBoundingBoxCollisionCheck(4 * m_objects.size());
		}
		void ObjectQuadTree::Tree::checkCollisionsSingleLayerSelfDeep(size_t index, std::vector<Utilities::Collisioninfo>& collisions,
															          bool onlyFirstCollision)
		{
			Tree& tree = m_childTrees[index];
			AABB& box = m_childAreas[index];
			for (auto objA : m_objects)
			{
				if (objA->getBoundingBox().intersects(box))
				{
					tree.checkCollision(objA, collisions, onlyFirstCollision);
				}
			}
		}
		void ObjectQuadTree::Tree::checkCollision(Objects::GameObjectPtr other,
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
		
		std::string toStr(size_t i)
		{
			char buffer[20];
			buffer[19] = 0;
			char * p = &buffer[18];
			while (i > 0)
			{
				*p = '0' + (i % 10);
				i /= 10;
				--p;
			}
			return p + 1;
		}

		void ObjectQuadTree::Tree::draw(sf::Text& text, sf::RectangleShape &rect, const sf::Color& color, sf::RenderTarget& target,
			sf::RenderStates states) const
		{
			
			rect.setPosition(m_area.TL());
			rect.setSize(m_area.getSize());

			//text.setString(std::to_string(m_objects.size()));
			//text.setString(toStr(m_objects.size()));
			text.setPosition((m_area.TL() + m_area.BR())*0.5f);

			
			target.draw(rect);
			target.draw(text);
			if (m_childTrees)
			{
				float fade = 0.7;
				sf::Color col = color;
				col.a *= fade;
				rect.setOutlineColor(col);
				{
					QSFMLP_COMPONENT_BLOCK("leaf 0", QSFML_COLOR_STAGE_1);
					m_childTrees[0].draw(text, rect, col, target, states);
				}
				{
					QSFMLP_COMPONENT_BLOCK("leaf 1", QSFML_COLOR_STAGE_2);
					m_childTrees[1].draw(text, rect, col, target, states);
				}
				{
					QSFMLP_COMPONENT_BLOCK("leaf 2", QSFML_COLOR_STAGE_3);
					m_childTrees[2].draw(text, rect, col, target, states);
				}
				{
					QSFMLP_COMPONENT_BLOCK("leaf 3", QSFML_COLOR_STAGE_4);
					m_childTrees[3].draw(text, rect, col, target, states);
				}
			}
		}
		void ObjectQuadTree::Tree::draw(sf::RectangleShape &rect, const sf::Color& color, sf::RenderTarget& target,
			sf::RenderStates states) const
		{
			rect.setPosition(m_area.TL());
			rect.setSize(m_area.getSize());

			if (m_childTrees)
			{
				float fade = 0.7;
				sf::Color col = color;
				col.a *= fade;
				rect.setOutlineColor(col);
				m_childTrees[0].draw(rect, col, target, states);
				m_childTrees[1].draw(rect, col, target, states);
				m_childTrees[2].draw(rect, col, target, states);
				m_childTrees[3].draw(rect, col, target, states);
			}
			target.draw(rect);
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
			if (!painter) return;
			for (size_t i = 0; i < m_painters.size(); ++i)
			{
				if (m_painters[i] == painter)
				{
					m_painters.erase(m_painters.begin() + i);
					painter->m_tree = nullptr;
					return;
				}
			}			
		}
		void ObjectQuadTree::assignPainter(ObjectQuadTreePainter* painter)
		{
			if (!painter) return;
			for (size_t i = 0; i < m_painters.size(); ++i)
			{
				if (m_painters[i] == painter)
				{
					return;
				}
			}
			painter->m_tree = this;
			m_painters.push_back(painter);
		}

		ObjectQuadTree::ObjectQuadTreePainter::ObjectQuadTreePainter(ObjectQuadTree* tree, const std::string& name)
			: Drawable(name)
			, m_tree(tree)
		{

		}
		ObjectQuadTree::ObjectQuadTreePainter::ObjectQuadTreePainter(const ObjectQuadTreePainter& other)
			: Drawable(other)
			, m_tree(nullptr)
		{
			assignTree(other.m_tree);
		}
		COMPONENT_IMPL(ObjectQuadTree::ObjectQuadTreePainter);
		ObjectQuadTree::ObjectQuadTreePainter::~ObjectQuadTreePainter()
		{
			if (m_tree)
				m_tree->removePainter(this);
		}
		void ObjectQuadTree::ObjectQuadTreePainter::assignTree(ObjectQuadTree* tree)
		{
			if (m_tree)
			{
				m_tree->removePainter(this);
			}
			m_tree = tree;
			if (m_tree)
				m_tree->assignPainter(this);

		}
		ObjectQuadTree* ObjectQuadTree::ObjectQuadTreePainter::getTree() const
		{
			return m_tree;
		}
		void ObjectQuadTree::ObjectQuadTreePainter::setColor(const sf::Color& color)
		{
			m_color = color;
		}
		const sf::Color& ObjectQuadTree::ObjectQuadTreePainter::getColor() const
		{
			return m_color;
		}
		/*void ObjectQuadTree::ObjectQuadTreePainter::drawComponent(sf::RenderTarget& target,
			sf::RenderStates states) const
		{
			if (m_tree)
				m_tree->m_tree.draw(getTextFont(), m_color, target, states);
		}*/
	}
}
