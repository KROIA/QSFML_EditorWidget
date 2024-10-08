#pragma once

#include "UnitTest.h"
#include "QSFML_EditorWidget.h"
#include <QObject>
#include <QCoreapplication>
#include <QTimer>


using namespace QSFML;
using namespace QSFML::Utilities;

class TST_Pathfinder : public QObject, public UnitTest::Test
{
	TEST_CLASS(TST_Pathfinder);
	Q_OBJECT;
public:
	TST_Pathfinder()
		: Test("TST_Pathfinder")
	{
		//ADD_TEST(TST_Pathfinder::simplePath);
		//ADD_TEST(TST_Pathfinder::simplePath2);
		ADD_TEST(TST_Pathfinder::randomPaths);


		connect(&m_stopTimer, &QTimer::timeout, this, &TST_Pathfinder::onTimeout);
		m_stopTimer.setInterval(60000);
	}


private slots:
	void onTimeout()
	{
		m_stopTimer.stop();
		qApp->quit();
	}
private:
	QTimer m_stopTimer;

	// Tests
	TEST_FUNCTION(simplePath)
	{
		TEST_START;
		Pathfinder pathfinder;
		pathfinder.addNode("A", { 0, 0 });
		pathfinder.addNode("B", { 1, 0 });
		pathfinder.addNode("C", { 1, 1 });
		pathfinder.addNode("D", { 0, 1 });

		pathfinder.addEdge("A", "B", 5, Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("B", "C", 1, Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("C", "D", 1, Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("D", "A", 1, Pathfinder::Direction::Bidirectional);

		auto path = pathfinder.findPath("A", "C");
		TEST_ASSERT_M(path.size() == 3, "Path size is 3");
		TEST_ASSERT_M(path[0] == "A", "Path[0] == A");
		TEST_ASSERT_M(path[1] == "D", "Path[1] == D");
		TEST_ASSERT_M(path[2] == "C", "Path[2] == C");


	}


	TEST_FUNCTION(simplePath2)
	{
		TEST_START;
		QSFML::Scene* scene = createDefaultScene();
		Pathfinder pathfinder;
		Objects::GameObjectPtr obj = new Objects::GameObject("PathfinderObj");
		scene->addObject(obj);

		pathfinder.addNode("A", { 0, 0 });
		pathfinder.addNode("B", { 100, 0 });
		pathfinder.addNode("C", { 100, 100 });
		pathfinder.addNode("D", { 0, 100 });
		pathfinder.addNode("E", { 200, 150 });

		pathfinder.addEdge("A", "B", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("B", "C", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("C", "D", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("D", "A", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("D", "E", Pathfinder::Direction::Bidirectional);
		//pathfinder.addEdge("A", "E", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("C", "E", Pathfinder::Direction::Bidirectional);
		pathfinder.addEdge("B", "E", Pathfinder::Direction::Bidirectional);

		Pathfinder::Painter* painter = pathfinder.createPainter();
		painter->drawPath("A", "E");
		obj->addComponent(painter);

		Components::MouseFollower* mouseFollower = new Components::MouseFollower();
		obj->addComponent(mouseFollower);
		QObject::connect(mouseFollower, &Components::MouseFollower::mousePosChanged, [&pathfinder, painter](const sf::Vector2f& worldPos,
			const sf::Vector2i& pixelPos)
			{
				Pathfinder::Node node = pathfinder.getNode("A");
				node.position = worldPos;
				pathfinder.setNode("A", node);
				std::unordered_map<std::string, Pathfinder::Node> nodes = pathfinder.getNodes();
				nodes.erase("A");

				// Find the closest node to "A"
				float minDist = std::numeric_limits<float>::max();
				std::string closestNodeID;
				for (auto& node : nodes)
				{
					float dist = std::sqrt(std::pow(node.second.position.x - worldPos.x, 2) + std::pow(node.second.position.y - worldPos.y, 2));
					if (dist < minDist)
					{
						minDist = dist;
						closestNodeID = node.first;
					}
				}
				pathfinder.clearEdges("A");
				pathfinder.addEdge("A", closestNodeID, Pathfinder::Direction::Bidirectional);


				pathfinder.setWeigthsFromNodeDistances();
			});


		qApp->exec();
		delete scene;
	}

	static void connectNearestNeighbours(Pathfinder& pathfinder, size_t connectionCount, const std::string& lastNode)
	{
		std::unordered_map<std::string, Pathfinder::Node> nodes = pathfinder.getNodes();
		nodes.erase(lastNode);
		// Get connections from last node to save them
		std::vector<Pathfinder::Edge> lastNodeConnections;
		auto lastNodeIt = pathfinder.getEdges().find(lastNode);
		if (lastNodeIt != pathfinder.getEdges().end())
			lastNodeConnections = lastNodeIt->second;

		for (auto& firstNode : nodes)
		{
			// Sort by distance ( shortest first )
			std::vector<std::pair<std::string, float>> distances;
			for (auto& node : nodes)
			{
				if (node.first == firstNode.first)
					continue;
				float dist = std::sqrt(std::pow(node.second.position.x - firstNode.second.position.x, 2) + std::pow(node.second.position.y - firstNode.second.position.y, 2));
				distances.push_back({ node.first, dist });
			}
			std::sort(distances.begin(), distances.end(), [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b)
					  {
						  return a.second < b.second;
					  });
			pathfinder.clearEdges(firstNode.first);

			// Add the 5 closest nodes to the last node
			for (int i = 0; i < std::min(distances.size(), connectionCount); ++i)
			{
				pathfinder.addEdge(firstNode.first, distances[i].first, Pathfinder::Direction::Bidirectional);
			}
		}

		// Add connections from last node
		for (auto& edge : lastNodeConnections)
		{
			pathfinder.addEdge(lastNode, edge.destinationNodeID, Pathfinder::Direction::Bidirectional);
		}
		

		// Calculate the weights of the edges based on the distance between the nodes
		pathfinder.setWeigthsFromNodeDistances();
	}

	TEST_FUNCTION(randomPaths)
	{
		TEST_START;
		QSFML::Scene* scene = createDefaultScene();
		Pathfinder pathfinder;

		// Create a gameobject to manage the painting and motion of the nodes
		Objects::GameObjectPtr obj = new Objects::GameObject("PathfinderObj");
		scene->addObject(obj);

		// Nodes which define the start and end of the path
		std::string firstNode;
		std::string lastNode;
		sf::Vector2u area(800, 600);
		float lastNodeForce = 15;
		int nodeCount = 200;
		float targetDistance = sqrt(area.x*area.y/sqrt(nodeCount));
		size_t lastNodeConnectionCount = 5;
		size_t connectionCountPerNode = 7;

		// Add random nodes and edges
		for (int i = 0; i < nodeCount; ++i)
		{
			std::string id = "Node" + std::to_string(i);
			pathfinder.addNode(id, { (float)(rand() % area.x), (float)(rand() % area.y) });
			//if (i > 0)
			//	pathfinder.addEdge("Node" + std::to_string(i - 1), id, Pathfinder::Direction::Bidirectional);
			if (i == 0)
				firstNode = id;
			if (i == nodeCount-1)
				lastNode = id;
		}

		// Connect closest n points
		connectNearestNeighbours(pathfinder, connectionCountPerNode, lastNode);	
		
		
		// Create a painter to draw the graph
		Pathfinder::Painter* painter = pathfinder.createPainter();

		// Define which path the painter shuld highlight
		painter->drawPath(firstNode, lastNode);
		obj->addComponent(painter);

		// Create a mouse follower to move the last node
		Components::MouseFollower* mouseFollower = new Components::MouseFollower();
		obj->addComponent(mouseFollower);
		QObject::connect(mouseFollower, &Components::MouseFollower::mousePosChanged, [obj, &lastNode, lastNodeConnectionCount ,&pathfinder, painter](const sf::Vector2f& worldPos,
			const sf::Vector2i& pixelPos)
			{
				// This function is called when the mouse is moved
				// It moves the last node to the mouse position
				// It also reconnects the last node to the 5 closest nodes

				// Get the last node and update its position
				Pathfinder::Node node = pathfinder.getNode(lastNode);
				node.position = worldPos;
				pathfinder.setNode(lastNode, node);
				std::unordered_map<std::string, Pathfinder::Node> nodes = pathfinder.getNodes();
				nodes.erase(lastNode);
				
				// Sort by distance ( shortest first )
				std::vector<std::pair<std::string, float>> distances;
				for (auto& node : nodes)
				{
					float dist = std::sqrt(std::pow(node.second.position.x - worldPos.x, 2) + std::pow(node.second.position.y - worldPos.y, 2));
					distances.push_back({ node.first, dist });
				}
				std::sort(distances.begin(), distances.end(), [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b)
					{
						return a.second < b.second;
					});


				// Clear all edges that are connected to the last node
				pathfinder.clearEdges(lastNode);

				// Add the 5 closest nodes to the last node
				for (int i = 0; i < std::min(distances.size(), lastNodeConnectionCount); ++i)
				{
					pathfinder.addEdge(lastNode, distances[i].first, Pathfinder::Direction::Bidirectional);
				}

				// Calculate the weights of the edges based on the distance between the nodes
				pathfinder.setWeigthsFromNodeDistances();
			});

		// This update function calculates the forces between the nodes and moves them
		// It simulates edges as springs that apply a force to the nodes
		obj->addUpdateFunction([&pathfinder, lastNode, lastNodeForce, lastNodeConnectionCount, targetDistance, connectionCountPerNode](Objects::GameObject& obj)
			{
				std::unordered_map<std::string, Pathfinder::Node> nodes = pathfinder.getNodes();
				std::unordered_map<std::string, std::vector<Pathfinder::Edge>>edges = pathfinder.getEdges();

				std::unordered_map<std::string, sf::Vector2f> forces;
				float speed = obj.getDeltaT() * 1;
				for (auto& edge : edges)
				{
					sf::Vector2f edgePos1 = nodes[edge.first].position;
					for (auto& edge2 : edge.second)
					{
						sf::Vector2f edgePos2 = nodes[edge2.destinationNodeID].position;
						sf::Vector2f edgeDir = edgePos2 - edgePos1;
						float distance = std::sqrt(std::pow(edgeDir.x, 2) + std::pow(edgeDir.y, 2));

						// Only apply force if the distance is larger than 100
						//distance -= 100;
						if(distance < targetDistance)
							continue;
						float force = distance * distance / 100;

						if (edge2.destinationNodeID == lastNode)
							force *= lastNodeForce/(float)lastNodeConnectionCount;
						
						sf::Vector2f edgeDirNorm = edgeDir / distance;
						

						if (force == std::numeric_limits<float>::infinity() || force == -std::numeric_limits<float>::infinity() || force != force)
							continue;

						forces[edge.first] += edgeDirNorm * force;
					}
				}

				for (auto& node1 : nodes)
				{
					for (auto& node2 : nodes)
					{
						if (node1.first == node2.first)
							continue;
						sf::Vector2f dir = node2.second.position - node1.second.position;
						float distance = QSFML::VectorMath::getLength(dir);
						if(distance > targetDistance)
							continue;
						if (distance < 1)
							distance = 1;
						float force = -targetDistance*10 /(distance);
						if (node2.first == lastNode)
							force *= lastNodeForce;
						sf::Vector2f dirNorm = dir / distance;
						forces[node1.first] += dirNorm * force;
					}
				}


				for (auto& node : nodes)
				{
					// Do not move the last node, since it is connected to the mouse movement
					if (node.first == lastNode)
						continue;
					const auto& forceElement = forces.find(node.first);
					if (forceElement != forces.end())
						node.second.position += forceElement->second * speed;
				}
				pathfinder.setNodes(nodes);
				connectNearestNeighbours(pathfinder, connectionCountPerNode, lastNode);
			});


		scene->start();
		// Start the application to process the eventloop and run the engine
		qApp->exec();
		delete scene;
	}





	QSFML::Scene* createDefaultScene()
	{
		QSFML::SceneSettings settings;
		settings.timing.frameTime = 0;
		QSFML::Scene* scene = new QSFML::Scene(nullptr, settings);
		scene->start();
		m_stopTimer.start();

		scene->addObject(new QSFML::Objects::DefaultEditor());
		return scene;
	}

};
