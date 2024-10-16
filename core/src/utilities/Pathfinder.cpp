#include "utilities/Pathfinder.h"
#include "utilities/LifetimeChecker.h"
#include "utilities/VectorOperations.h"
#include "objects/base/GameObject.h"
#include <SFML/OpenGL.hpp>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <algorithm>

namespace QSFML
{
	namespace Utilities
	{
		Pathfinder::Pathfinder()
		{
			Internal::LifetimeChecker::add(this);
		}
		Pathfinder::~Pathfinder()
		{
			Internal::LifetimeChecker::setDead(this);
            for (Painter* painter : m_painters)
            {
                if (Internal::LifetimeChecker::isAlive(painter))
                {
					painter->m_pathfinder = nullptr;
                }
            }
		}

		bool Pathfinder::addNode(const std::string& id, const sf::Vector2f& position)
		{
			if (m_nodes.find(id) != m_nodes.end())
				return false;
			Node node;
			node.position = position;
			m_nodes.at(id) = node;
            onPathChanged();
			//m_built = false;
			return true;
		}
        bool Pathfinder::addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, Direction dir)
        {
            float distance = 0;
			if (m_nodes.find(sourceNodeID) != m_nodes.end() && m_nodes.find(destinationNodeID) != m_nodes.end())
			{
				distance = std::sqrt(std::pow(m_nodes.at(sourceNodeID).position.x - m_nodes.at(destinationNodeID).position.x, 2) + std::pow(m_nodes.at(sourceNodeID).position.y - m_nodes.at(destinationNodeID).position.y, 2));
			}
			return addEdge(sourceNodeID, destinationNodeID, distance, dir);
        }
		bool Pathfinder::addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, float weight, Direction dir)
		{
            bool success = addEdge(sourceNodeID, destinationNodeID, weight);
            if (dir == Direction::Bidirectional)
                success &= addEdge(destinationNodeID, sourceNodeID, weight);

            onPathChanged();
            return success;
		}
        bool Pathfinder::addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, float weight)
        {
            // Check if both nodes exist
            if (m_nodes.find(sourceNodeID) == m_nodes.end() || m_nodes.find(destinationNodeID) == m_nodes.end())
            {
                return false; // One or both nodes do not exist
            }

            // Add edge from source to destination if it doesn't already exist
            auto& sourceEdges = m_edges.at(sourceNodeID);
            bool edgeExists = std::any_of(sourceEdges.begin(), sourceEdges.end(), [&](const Edge& edge) {
                return edge.destinationNodeID == destinationNodeID;
                });

            if (!edgeExists)
            {
                sourceEdges.push_back({ destinationNodeID, weight });
                onPathChanged();
                return true;
            }
            return false;            
        }


		void Pathfinder::setNodes(const QSFML::unordered_map<std::string, Node>& nodes)
		{
			m_nodes = nodes;

			setEdges(m_edges);
		}
        void Pathfinder::setEdges(const QSFML::unordered_map<std::string, QSFML::vector<Edge>>& edges)
        {
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
            QSFML::unordered_map<std::string, QSFML::vector<Edge>> newEdges;

            // Remove edges that can't exist
            for (auto& edge : edges)
            {
                auto it = m_nodes.find(edge.first);
                if (it != m_nodes.end())
                {
                    for (auto& edge2 : edge.second)
                    {
                        if (m_nodes.find(edge2.destinationNodeID) != m_nodes.end())
                        {
                            newEdges.at(edge.first).push_back(edge2);
                        }
                    }
                }
            }
            m_edges = newEdges;
            onPathChanged();
        }
        void Pathfinder::clearNodes()
        {
            m_nodes.clear();
            m_edges.clear();
			onPathChanged();
        }
        void Pathfinder::clearEdges()
        {
            m_edges.clear();
			onPathChanged();
        }
        void Pathfinder::clearEdges(const std::string& nodeID)
        {
            QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			m_edges.erase(nodeID);
			for (auto& pair : m_edges)
			{
				pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(), [&](const Edge& edge) { return edge.destinationNodeID == nodeID; }), pair.second.end());
			}
            onPathChanged();
        }
        void Pathfinder::clear()
        {
			m_nodes.clear();
			m_edges.clear();
			onPathChanged();
        }


        Pathfinder::Node Pathfinder::getNode(const std::string& id) const
        {
            if (m_nodes.find(id) != m_nodes.end())
            {
                return m_nodes.at(id);
            }
            return Node();
        }
        void Pathfinder::setNode(const std::string& id, const Node& node)
        {
			const auto& it = m_nodes.find(id);
            if (it != m_nodes.end())
            {
				it->second = node;
				onPathChanged();
            }
        }

        void Pathfinder::setWeigthsFromNodeDistances()
        {
            QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
            for (auto& pair : m_edges)
            {
                for (Edge& edge : pair.second)
                {
                    float distance = std::sqrt(std::pow(m_nodes.at(pair.first).position.x - m_nodes.at(edge.destinationNodeID).position.x, 2) + std::pow(m_nodes.at(pair.first).position.y - m_nodes.at(edge.destinationNodeID).position.y, 2));
                    edge.weight = distance;
                }
            }
            onPathChanged();
        }

        float Pathfinder::getDistance(const std::string& startNodeID, const std::string& endNodeID) const
        {
			if (m_nodes.find(startNodeID) == m_nodes.end() || m_nodes.find(endNodeID) == m_nodes.end())
			{
				return std::numeric_limits<float>::infinity();
			}
			const sf::Vector2f& startPos = m_nodes.at(startNodeID).position;
			const sf::Vector2f& endPos = m_nodes.at(endNodeID).position;

			return std::sqrt(std::pow(startPos.x - endPos.x, 2) + std::pow(startPos.y - endPos.y, 2));
        }

        float Pathfinder::getPathDistance(const std::string& startNodeID, const std::string& endNodeID) const
        {
			return getPathDistance(findPath(startNodeID, endNodeID));
        }
        float Pathfinder::getPathDistance(const QSFML::vector<std::string>& path) const
        {
			QSFML::vector<sf::Vector2f> positions = getPathPositions(path);
            float length = 0;
			for (size_t i = 0; i < positions.size() - 1; i++)
			{
				length += VectorMath::getLength(positions[i + 1] - positions[i]);
			}
			return length;
        }
        sf::Vector2f Pathfinder::lerp(const std::string& startNodeID, const std::string& endNodeID, float t) const
        {
			QSFML::vector<sf::Vector2f> positions = findPathPositions(startNodeID, endNodeID);
			return VectorMath::lerp(positions, t);
        }
        sf::Vector2f Pathfinder::lerp(const QSFML::vector<std::string>& path, float t) const
        {
			QSFML::vector<sf::Vector2f> positions = getPathPositions(path);
			return VectorMath::lerp(positions, t);
        }

        Pathfinder::Painter* Pathfinder::createPainter()
        {
			Painter* painter = new Pathfinder::Painter(this);
			m_painters.push_back(painter);
			return painter;
		}
        void Pathfinder::painterDeleted(Painter* painter)
        {
			m_painters.erase(std::remove(m_painters.begin(), m_painters.end(), painter), m_painters.end());
		}
        void Pathfinder::onPathChanged()
        {
			for (Painter* painter : m_painters)
			{
				painter->m_pathChanged = true;
			}
        }

		QSFML::vector<std::string> Pathfinder::findPath(const std::string& startNodeID, const std::string& endNodeID) const
		{
            QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
            // If either start or end node is missing, return an empty path
            if (m_nodes.find(startNodeID) == m_nodes.end() || m_nodes.find(endNodeID) == m_nodes.end())
            {
                return {};
            }

            // Priority queue for Dijkstra's algorithm; stores (cost, node ID)
            using QueueElement = QSFML::pair<float, std::string>;
            std::priority_queue<QueueElement, QSFML::vector<QueueElement>, std::greater<>> priorityQueue;

            // Map to store the minimum cost to reach each node
            QSFML::unordered_map<std::string, float> minCost;
            // Map to store the previous node in the optimal path
            QSFML::unordered_map<std::string, std::string> previousNode;

            // Initialize all nodes with maximum possible cost
            for (const auto& pair : m_nodes)
            {
                minCost.at(pair.first) = std::numeric_limits<float>::infinity();
            }

            // Set the start node cost to 0 and add it to the queue
            minCost.at(startNodeID) = 0.0f;
            priorityQueue.push({ 0.0f, startNodeID });

            // Dijkstra's algorithm loop
            while (!priorityQueue.empty())
            {
                // Get the node with the lowest cost
                auto [currentCost, currentNodeID] = priorityQueue.top();
                priorityQueue.pop();

                // If we reach the destination, we can stop
                if (currentNodeID == endNodeID)
                {
                    break;
                }

                // Skip processing if this path is not optimal anymore
                if (currentCost > minCost.at(currentNodeID))
                {
                    continue;
                }

                // Process each neighboring edge
                if (m_edges.find(currentNodeID) != m_edges.end())
                {
					const auto& edges = m_edges.at(currentNodeID);
                    for (const Edge& edge : edges)
                    {
                        float newCost = currentCost + edge.weight;

                        // If a cheaper path to the destination is found
                        if (newCost < minCost.at(edge.destinationNodeID))
                        {
                            minCost.at(edge.destinationNodeID) = newCost;
							previousNode.at(edge.destinationNodeID) = currentNodeID;
                            //previousNode[edge.destinationNodeID] = currentNodeID;
                            priorityQueue.push({ newCost, edge.destinationNodeID });
                        }
                    }
                }
            }

            // Build the path by tracing from endNodeID to startNodeID
            QSFML::vector<std::string> path;
			path.reserve(50);
            if (previousNode.find(endNodeID) != previousNode.end() || startNodeID == endNodeID)
            {
                std::string currentNodeID = endNodeID;
                while (currentNodeID != startNodeID)
                {
                    path.push_back(currentNodeID);
                    currentNodeID = previousNode.at(currentNodeID);
                }
                path.push_back(startNodeID);

                // Reverse the path to get it from start to end
                std::reverse(path.begin(), path.end());
            }

            return path;
		}
        QSFML::vector<sf::Vector2f> Pathfinder::findPathPositions(const std::string& startNodeID, const std::string& endNodeID) const
        {
            QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
            QSFML::vector<std::string> path = findPath(startNodeID, endNodeID);
            QSFML::vector<sf::Vector2f> positions;
            for (const std::string& nodeID : path)
            {
                positions.push_back(m_nodes.at(nodeID).position);
            }
            return positions;
        }
        QSFML::vector<sf::Vector2f> Pathfinder::getPathPositions(const QSFML::vector<std::string> &path) const
        {
            QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
            QSFML::vector<sf::Vector2f> positions;
            for (const std::string& nodeID : path)
            {
				const auto& it = m_nodes.find(nodeID);
				if (it != m_nodes.end())
                    positions.push_back(it->second.position);
            }
            return positions;
        }







        // Painter
        Pathfinder::Painter::Painter(Pathfinder* pathfinder, const std::string& name)
			: Components::Drawable(name)
			, m_pathfinder(pathfinder)
		{
            Drawable::ignoreTransform(true);
		}

    
        Pathfinder::Painter::~Painter()
        {
            if (!m_pathfinder)
                return;
			if (Internal::LifetimeChecker::isAlive(m_pathfinder))
			{
				m_pathfinder->painterDeleted(this);
			}
        }

        void Pathfinder::Painter::drawPath(const std::string& startNodeID, const std::string& endNodeID)
        {
			m_pathChanged = true;
			m_startNodeID = startNodeID;
			m_endNodeID = endNodeID;
        }
        void Pathfinder::Painter::updatePath() const 
        {
			if (!m_pathfinder)
				return;

            m_path = m_pathfinder->findPath(m_startNodeID, m_endNodeID);
			m_pathNodes.clear();
			for (const std::string& nodeID : m_path)
			{
				m_pathNodes.at(nodeID) = true;
			}
			m_pathChanged = false;
        }
        void Pathfinder::Painter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
            if (!m_pathfinder)
                return;

            // Draw path if it is defined
            if (m_pathChanged && (m_enablePathPoints || m_enablePathLines))
            {
                updatePath();
            }
            float pathLength = 0;
            QSFML::vector<sf::Vector2f> pathPositions = m_pathfinder->getPathPositions(m_path);

#ifdef QSFML_USE_GL_DRAW
            QSFML_UNUSED(target);
			QSFML_UNUSED(states);
            glColor4ub(m_edgeColor.r, m_edgeColor.g, m_edgeColor.b, m_edgeColor.a);
#endif

			// Draw edges
			for (const auto& pair : m_pathfinder->m_edges)
			{
				const Node& sourceNode = m_pathfinder->m_nodes.at(pair.first);
				for (const Edge& edge : pair.second)
				{
					const Node& destinationNode = m_pathfinder->m_nodes.at(edge.destinationNodeID);

#ifdef QSFML_USE_GL_DRAW
                    glBegin(GL_LINES);

                    glVertex2f(sourceNode.position.x, sourceNode.position.y);
                    glVertex2f(destinationNode.position.x, destinationNode.position.y);
                    glEnd();
#else

					sf::Vertex line[] =
					{
						sf::Vertex(sourceNode.position, m_edgeColor),
						sf::Vertex(destinationNode.position, m_edgeColor)
					};

					target.draw(line, 2, sf::Lines, states);
#endif
				}
			}

			
            
            if (m_enablePathLines)
            {
                if (pathPositions.size() > 1)
                {
#ifdef QSFML_USE_GL_DRAW
                    glColor4ub(m_pathColor.r, m_pathColor.g, m_pathColor.b, m_pathColor.a);
#endif
                    for (size_t i = 0; i < pathPositions.size() - 1; i++)
                    {
                        pathLength += VectorMath::getLength(pathPositions[i+1] - pathPositions[i]);
#ifdef QSFML_USE_GL_DRAW
						glBegin(GL_LINES);
						glVertex2f(pathPositions[i].x, pathPositions[i].y);
						glVertex2f(pathPositions[i + 1].x, pathPositions[i + 1].y);
						glEnd();
#else
                        sf::Vertex line[] =
                        {
                            sf::Vertex(pathPositions[i+1], m_pathColor),
                            sf::Vertex(pathPositions[i], m_pathColor)
                        };
                        target.draw(line, 2, sf::Lines, states);
#endif
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < pathPositions.size() - 1; i++)
                {
                    pathLength += VectorMath::getLength(pathPositions[i + 1] - pathPositions[i]);
                }
            }
            
            sf::Color color = m_nodeColor;
#ifdef QSFML_USE_GL_DRAW
            glColor4ub(color.r, color.g, color.b, color.a);
#endif
			for (const auto& pair : m_pathfinder->m_nodes)
			{
				const Node& node = pair.second;
                
                if (m_pathNodes.find(pair.first) != m_pathNodes.end())
                {
#ifdef QSFML_USE_GL_DRAW
                    if(color != m_pathColor)
                        glColor4ub(m_pathColor.r, m_pathColor.g, m_pathColor.b, m_pathColor.a);
#endif
                    color = m_pathColor;
                }
                else
                {
#ifdef QSFML_USE_GL_DRAW
                    if (color != m_nodeColor)
                        glColor4ub(m_nodeColor.r, m_nodeColor.g, m_nodeColor.b, m_nodeColor.a);
#endif
                    color = m_nodeColor;
                }
#ifdef QSFML_USE_GL_DRAW
				drawGlCircleShape<16>(node.position, m_nodeRadius);               
#else
				sf::CircleShape circle(m_nodeRadius);
				circle.setOrigin(m_nodeRadius, m_nodeRadius);
				circle.setPosition(node.position);
				circle.setFillColor(color);

				target.draw(circle, states);
#endif
			}
            

            // Draw moving dot
            if (m_enablePathPoints)
            {
                if (pathPositions.size() > 1)
                {
#ifdef QSFML_USE_GL_DRAW
					glColor4ub(m_pathPointColor.r, m_pathPointColor.g, m_pathPointColor.b, m_pathPointColor.a);

#else
                    sf::CircleShape circle(m_pathPointRadius);
                    circle.setOrigin(m_pathPointRadius, m_pathPointRadius);
                    circle.setFillColor(m_pathPointColor);
#endif

                    int points = pathLength / m_pathPointDistance;
                    for (int i = 0; i < points; i++)
                    {
                        float t = m_t + (float)i / points;
                        if (t > 1)
                        {
                            t -= (int)t;
                        }
                        else if (t < 0)
                        {
                            t += (int)t;
                        }
                        sf::Vector2f position = QSFML::VectorMath::lerp(pathPositions, t);
#ifdef QSFML_USE_GL_DRAW
                        drawGlCircleShape<16>(position, m_pathPointRadius);
#else
                        circle.setPosition(position);
                        target.draw(circle, states);
#endif
                    }
                }
                m_t += getDeltaT() * m_pathPointSpeed / pathLength;
                if (m_t > 1)
                {
                    m_t = 0;
                }
            }			
		}



    }
}