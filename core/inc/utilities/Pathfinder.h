#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT Pathfinder
		{
			friend class Painter;
		public:
			enum Direction
			{
				Unidirectional,
				Bidirectional
			};
			struct Node;
			struct Edge;
			struct Node
			{
				sf::Vector2f position;
			};
			struct Edge
			{
				std::string destinationNodeID;
				float weight = 1;
			};

			Pathfinder();
			~Pathfinder();

			bool addNode(const std::string& id, const sf::Vector2f& position);
			bool addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, Direction dir = Direction::Unidirectional);
			bool addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, float weight, Direction dir);
			bool addEdge(const std::string& sourceNodeID, const std::string& destinationNodeID, float weight);
			void setNodes(const std::unordered_map<std::string, Node>& nodes);
			void setEdges(const std::unordered_map<std::string, std::vector<Edge>>& edges);
			void clearNodes();
			void clearEdges();
			void clearEdges(const std::string& nodeID);
			void clear();

			Node getNode(const std::string& id) const;
			void setNode(const std::string& id, const Node& node);
			void setWeigthsFromNodeDistances();

			float getDistance(const std::string& startNodeID, const std::string& endNodeID) const;
			float getPathDistance(const std::string& startNodeID, const std::string& endNodeID) const;
			float getPathDistance(const std::vector<std::string>& path) const;
			sf::Vector2f lerp(const std::string& startNodeID, const std::string& endNodeID, float t) const;
			sf::Vector2f lerp(const std::vector<std::string>& path, float t) const;

			const std::unordered_map<std::string, Node>& getNodes() const
			{
				return m_nodes;
			}
			const std::unordered_map<std::string, std::vector<Edge>>& getEdges() const
			{
				return m_edges;
			}

			std::vector<std::string> findPath(const std::string& startNodeID, const std::string& endNodeID) const;
			std::vector<sf::Vector2f> findPathPositions(const std::string& startNodeID, const std::string& endNodeID) const;
			std::vector<sf::Vector2f> getPathPositions(const std::vector<std::string> &path) const;


			// Painter 
			class QSFML_EDITOR_WIDGET_EXPORT Painter : public Components::Drawable
			{
				friend Pathfinder;
				Painter(Pathfinder* pathfinder, const std::string& name = "PathfinderPainter");
				Painter(const Painter& other) = delete;

			public:
				~Painter();

				void drawPath(const std::string& startNodeID, const std::string& endNodeID);
				void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;
				void enablePathPoints(bool enable) { m_enablePathPoints = enable; }
				bool isPathPointsEnabled() const { return m_enablePathPoints; }
				void enablePath(bool enable) { m_enablePathLines = enable; }
				bool isPathEnabled() const { return m_enablePathLines; }

				void setEdgeColor(const sf::Color& color) { m_edgeColor = color; }
				const sf::Color& getEdgeColor() const { return m_edgeColor; }
				void setNodeColor(const sf::Color& color) { m_nodeColor = color; }
				const sf::Color& getNodeColor() const { return m_nodeColor; }
				void setPathColor(const sf::Color& color) { m_pathColor = color; }
				const sf::Color& getPathColor() const { return m_pathColor; }
				void setPathPointColor(const sf::Color& color) { m_pathPointColor = color; }
				const sf::Color& getPathPointColor() const { return m_pathPointColor; }
				void setNodeRadius(float radius) { m_nodeRadius = radius; }
				float getNodeRadius() const { return m_nodeRadius; }
				void setPathPointRadius(float radius) { m_pathPointRadius = radius; }
				float getPathPointRadius() const { return m_pathPointRadius; }
				void setPathPointDistance(float distance) { m_pathPointDistance = distance; }
				float getPathPointDistance() const { return m_pathPointDistance; }
			private:
				void updatePath() const;
				Pathfinder* m_pathfinder = nullptr;
				mutable bool m_pathChanged = true;

				bool m_enablePathPoints = true;
				bool m_enablePathLines = true;

				std::string m_startNodeID;
				std::string m_endNodeID;
				mutable std::vector<std::string> m_path;
				mutable std::unordered_map<std::string, bool> m_pathNodes;
				mutable float m_t = 0;


				sf::Color m_edgeColor = sf::Color::White;
				sf::Color m_nodeColor = sf::Color::White;
				sf::Color m_pathColor = sf::Color::Green;
				sf::Color m_pathPointColor = sf::Color::Red;
				float m_nodeRadius = 5;
				float m_pathPointRadius = 3;
				float m_pathPointDistance = 50;
				float m_pathPointSpeed = 1000; // world space / second
			};

			Painter* createPainter();

		private:
			void painterDeleted(Painter* painter);
			void onPathChanged();
				
			// string is the node id
			std::unordered_map<std::string, Node> m_nodes;

			// string is the source node id, and vector of edges from this source node
			std::unordered_map<std::string, std::vector<Edge>> m_edges;

			std::vector<Painter*> m_painters;


		};
	}
}