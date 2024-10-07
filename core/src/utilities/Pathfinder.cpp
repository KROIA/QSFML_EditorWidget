#include "utilities/Pathfinder.h"
#include "utilities/LifetimeChecker.h"
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
			m_nodes[id] = node;
            onPathChanged();
			//m_built = false;
			return true;
		}
        bool Pathfinder::addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, Direction dir)
        {
            float distance = 0;
			if (m_nodes.find(sourceNodeID) != m_nodes.end() && m_nodes.find(destinationNodeID) != m_nodes.end())
			{
				distance = std::sqrt(std::pow(m_nodes[sourceNodeID].position.x - m_nodes[destinationNodeID].position.x, 2) + std::pow(m_nodes[sourceNodeID].position.y - m_nodes[destinationNodeID].position.y, 2));
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
            auto& sourceEdges = m_edges[sourceNodeID];
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


		void Pathfinder::setNodes(const std::unordered_map<std::string, Node>& nodes)
		{
			m_nodes = nodes;

			setEdges(m_edges);
		}
        void Pathfinder::setEdges(const std::unordered_map<std::string, std::vector<Edge>>& edges)
        {
            std::unordered_map<std::string, std::vector<Edge>> newEdges;

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
                            newEdges[edge.first].push_back(edge2);
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
			auto& it = m_nodes.find(id);
            if (it != m_nodes.end())
            {
				it->second = node;
				onPathChanged();
            }
        }

        void Pathfinder::setWeigthsFromNodeDistances()
        {
            for (auto& pair : m_edges)
            {
                for (Edge& edge : pair.second)
                {
                    float distance = std::sqrt(std::pow(m_nodes[pair.first].position.x - m_nodes[edge.destinationNodeID].position.x, 2) + std::pow(m_nodes[pair.first].position.y - m_nodes[edge.destinationNodeID].position.y, 2));
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

		std::vector<std::string> Pathfinder::findPath(const std::string& startNodeID, const std::string& endNodeID)
		{
            // If either start or end node is missing, return an empty path
            if (m_nodes.find(startNodeID) == m_nodes.end() || m_nodes.find(endNodeID) == m_nodes.end())
            {
                return {};
            }

            // Priority queue for Dijkstra's algorithm; stores (cost, node ID)
            using QueueElement = std::pair<float, std::string>;
            std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<>> priorityQueue;

            // Map to store the minimum cost to reach each node
            std::unordered_map<std::string, float> minCost;
            // Map to store the previous node in the optimal path
            std::unordered_map<std::string, std::string> previousNode;

            // Initialize all nodes with maximum possible cost
            for (const auto& pair : m_nodes)
            {
                minCost[pair.first] = std::numeric_limits<float>::infinity();
            }

            // Set the start node cost to 0 and add it to the queue
            minCost[startNodeID] = 0.0f;
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
                if (currentCost > minCost[currentNodeID])
                {
                    continue;
                }

                // Process each neighboring edge
                if (m_edges.find(currentNodeID) != m_edges.end())
                {
                    for (const Edge& edge : m_edges[currentNodeID])
                    {
                        float newCost = currentCost + edge.weight;

                        // If a cheaper path to the destination is found
                        if (newCost < minCost[edge.destinationNodeID])
                        {
                            minCost[edge.destinationNodeID] = newCost;
                            previousNode[edge.destinationNodeID] = currentNodeID;
                            priorityQueue.push({ newCost, edge.destinationNodeID });
                        }
                    }
                }
            }

            // Build the path by tracing from endNodeID to startNodeID
            std::vector<std::string> path;
            if (previousNode.find(endNodeID) != previousNode.end() || startNodeID == endNodeID)
            {
                std::string currentNodeID = endNodeID;
                while (currentNodeID != startNodeID)
                {
                    path.push_back(currentNodeID);
                    currentNodeID = previousNode[currentNodeID];
                }
                path.push_back(startNodeID);

                // Reverse the path to get it from start to end
                std::reverse(path.begin(), path.end());
            }

            return path;
		}








        // Painter
        Pathfinder::Painter::Painter(Pathfinder* pathfinder, const std::string& name)
			: Components::Drawable(name)
			, m_pathfinder(pathfinder)
		{

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
				m_pathNodes[nodeID] = true;
			}
			m_pathChanged = false;
        }
        void Pathfinder::Painter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
            if (!m_pathfinder)
                return;

			// Draw edges
			for (const auto& pair : m_pathfinder->m_edges)
			{
				const Node& sourceNode = m_pathfinder->m_nodes.at(pair.first);
				for (const Edge& edge : pair.second)
				{
					const Node& destinationNode = m_pathfinder->m_nodes.at(edge.destinationNodeID);

					sf::Vertex line[] =
					{
						sf::Vertex(sourceNode.position, sf::Color::White),
						sf::Vertex(destinationNode.position, sf::Color::White)
					};

					target.draw(line, 2, sf::Lines, states);
				}
			}

			// Draw path if it is defined
			if (m_pathChanged)
			{
				updatePath();
			}
            if (m_path.size() > 1)
            {
                for (size_t i = 0; i < m_path.size() - 1; i++)
                {
                    const Node& sourceNode = m_pathfinder->m_nodes.at(m_path[i]);
                    const Node& destinationNode = m_pathfinder->m_nodes.at(m_path[i + 1]);

                    sf::Vertex line[] =
                    {
                        sf::Vertex(sourceNode.position, sf::Color::Green),
                        sf::Vertex(destinationNode.position, sf::Color::Green)
                    };

                    target.draw(line, 2, sf::Lines, states);
                }
            }

			// Draw nodes
			for (const auto& pair : m_pathfinder->m_nodes)
			{
				const Node& node = pair.second;

				sf::CircleShape circle(5.0f);
				circle.setOrigin(5.0f, 5.0f);
				circle.setPosition(node.position);
				sf::Color color = sf::Color::White;
				if (m_pathNodes.find(pair.first) != m_pathNodes.end())
					color = sf::Color::Green;
				circle.setFillColor(color);

				target.draw(circle, states);
			}
		}



    }
}