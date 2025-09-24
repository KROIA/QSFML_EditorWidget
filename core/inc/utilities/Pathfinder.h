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
		class QSFML_EDITOR_WIDGET_API Pathfinder
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
				QSFML::string destinationNodeID;
				float weight = 1;
			};

			Pathfinder();
			~Pathfinder();

			bool addNode(const QSFML::string& id, const sf::Vector2f& position);
			bool addEdge(const QSFML::string& sourceNodeID, const QSFML::string& destinationNodeID, Direction dir = Direction::Unidirectional);
			bool addEdge(const QSFML::string& sourceNodeID, const QSFML::string& destinationNodeID, float weight, Direction dir);
			bool addEdge(const QSFML::string& sourceNodeID, const QSFML::string& destinationNodeID, float weight);
			void setNodes(const QSFML::unordered_map<QSFML::string, Node>& nodes);
			void setEdges(const QSFML::unordered_map<QSFML::string, QSFML::vector<Edge>>& edges);
			void clearNodes();
			void clearEdges();
			void clearEdges(const QSFML::string& nodeID);
			void clear();

			Node getNode(const QSFML::string& id) const;
			void setNode(const QSFML::string& id, const Node& node);
			void setWeigthsFromNodeDistances();

			float getDistance(const QSFML::string& startNodeID, const QSFML::string& endNodeID) const;
			float getPathDistance(const QSFML::string& startNodeID, const QSFML::string& endNodeID) const;
			float getPathDistance(const QSFML::vector<QSFML::string>& path) const;
			sf::Vector2f lerp(const QSFML::string& startNodeID, const QSFML::string& endNodeID, float t) const;
			sf::Vector2f lerp(const QSFML::vector<QSFML::string>& path, float t) const;

			const QSFML::unordered_map<QSFML::string, Node>& getNodes() const
			{
				return m_nodes;
			}
			const QSFML::unordered_map<QSFML::string, QSFML::vector<Edge>>& getEdges() const
			{
				return m_edges;
			}

			QSFML::vector<QSFML::string> findPath(const QSFML::string& startNodeID, const QSFML::string& endNodeID) const;
			QSFML::vector<sf::Vector2f> findPathPositions(const QSFML::string& startNodeID, const QSFML::string& endNodeID) const;
			QSFML::vector<sf::Vector2f> getPathPositions(const QSFML::vector<QSFML::string> &path) const;


			// Painter 
			class QSFML_EDITOR_WIDGET_API Painter : public Components::Drawable
			{
				friend Pathfinder;
				Painter(Pathfinder* pathfinder, const std::string& name = "PathfinderPainter");
				Painter(const Painter& other) = delete;

			public:
				~Painter();

				void drawPath(const QSFML::string& startNodeID, const QSFML::string& endNodeID);
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

				QSFML::string m_startNodeID;
				QSFML::string m_endNodeID;
				mutable QSFML::vector<QSFML::string> m_path;
				mutable QSFML::unordered_map<QSFML::string, bool> m_pathNodes;
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
			QSFML::unordered_map<QSFML::string, Node> m_nodes;

			// string is the source node id, and vector of edges from this source node
			QSFML::unordered_map<QSFML::string, QSFML::vector<Edge>> m_edges;

			QSFML::vector<Painter*> m_painters;



		};
	}
}