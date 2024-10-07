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

			const std::unordered_map<std::string, Node>& getNodes() const
			{
				return m_nodes;
			}
			const std::unordered_map<std::string, std::vector<Edge>>& getEdges() const
			{
				return m_edges;
			}

			std::vector<std::string> findPath(const std::string& startNodeID, const std::string& endNodeID);



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

			private:
				void updatePath() const;
				Pathfinder* m_pathfinder = nullptr;
				mutable bool m_pathChanged = true;

				std::string m_startNodeID;
				std::string m_endNodeID;
				mutable std::vector<std::string> m_path;
				mutable std::unordered_map<std::string, bool> m_pathNodes;

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