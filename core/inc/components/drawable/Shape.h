#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics/Transform.hpp>
#include <vector>
#include "utilities/AABB.h"
#include "components/base/Drawable.h"

namespace QSFML
{
	namespace Components
	{
		class QSFML_EDITOR_WIDGET_EXPORT Shape : public Drawable
		{
		public:
			Shape(const std::string& name = "Shape");
			Shape(const Shape& other);
			~Shape();

			//void setTransform(const sf::Transform& transform);
			//const sf::Transform& getTransform() const;

			void setFillColor(const sf::Color& color);
			const sf::Color& getFillColor() const;

			void setOutlineColor(const sf::Color& color);
			const sf::Color& getOutlineColor() const;

			void setFill(bool fill);
			bool isFilled() const;
			void setOutline(bool outline);
			bool hasOutline() const;

			//void setOutlineThickness(float thickness);
			//float getOutlineThickness() const;

			void setPointCount(std::size_t count);
			std::size_t getPointCount() const;

			void setPoint(std::size_t index, const sf::Vector2f& point);
			const sf::Vector2f& getPoint(std::size_t index) const;
			sf::Vector2f getTransformedPoint(std::size_t index) const;

			void addPoint(const sf::Vector2f& point);
			void addPoints(const std::vector<sf::Vector2f>& points);
			void setPoints(const std::vector<sf::Vector2f>& points);
			void removePoint(std::size_t index);

			const std::vector<sf::Vector2f>& getPoints() const;
			std::vector<sf::Vector2f> getTransformedPoints() const;
			

			Utilities::AABB getLocalBounds() const;
			Utilities::AABB getGlobalBounds() const;

			//void drawComponent(sf::RenderTarget& target,
			//	sf::RenderStates states) const override;

			void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;
		private:
			void updateTranformedPoints();
			void updateVertexColors();
			std::vector<sf::Vector2f> m_points;
			//std::vector<sf::Vector2f> m_transformedPoints;
			//std::vector<sf::Vertex> m_transformedVertecies;
			//sf::Transform m_transform;
			sf::Color m_fillColor;
			sf::Color m_outlineColor;
			//float m_outlineThickness;
			bool m_fill;
			bool m_outline;

		};
	}
}
