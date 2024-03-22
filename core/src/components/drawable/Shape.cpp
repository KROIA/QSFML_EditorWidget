#include "components/drawable/Shape.h"

namespace QSFML
{
	namespace Components
	{
		Shape::Shape(const std::string& name)
			: Drawable(name)
			, m_fillColor(sf::Color::White)
			, m_outlineColor(sf::Color::White)
			, m_fill(false)
			, m_outline(true)
			//, m_outlineThickness(1)
		{
		}


		Shape::~Shape()
		{

		}

		void Shape::setTransform(const sf::Transform& transform)
		{
			m_transform = transform;
			updateTranformedPoints();
		}
		const sf::Transform& Shape::getTransform() const
		{
			return m_transform;
		}

		void Shape::setFillColor(const sf::Color& color)
		{
			m_fillColor = color;
			updateVertexColors();
		}
		const sf::Color& Shape::getFillColor() const
		{
			return m_fillColor;
		}

		void Shape::setOutlineColor(const sf::Color& color)
		{
			m_outlineColor = color;
		}
		const sf::Color& Shape::getOutlineColor() const
		{
			return m_outlineColor;
		}

		/*void Shape::setOutlineThickness(float thickness)
		{
			m_outlineThickness = thickness;
		}
		float Shape::getOutlineThickness() const
		{
			return m_outlineThickness;
		}*/

		std::size_t Shape::getPointCount() const
		{
			return m_points.size();
		}
		void Shape::setFill(bool fill)
		{
			m_fill = fill;
		}
		bool Shape::isFilled() const
		{
			return m_fill;
		}
		void Shape::setOutline(bool outline)
		{
			m_outline = outline;
		}
		bool Shape::hasOutline() const
		{
			return m_outline;
		}

		void Shape::setPoint(std::size_t index, const sf::Vector2f& point)
		{
			m_points[index] = point;
			m_transformedPoints[index] = m_transform.transformPoint(point);
			m_transformedVertecies[index].position = m_transformedPoints[index];
			if (index == 0)
			{
				m_transformedVertecies[m_transformedVertecies.size() - 1] = m_transformedVertecies[0];
			}
		}
		const sf::Vector2f& Shape::getPoint(std::size_t index) const
		{
			return m_points[index];
		}
		const sf::Vector2f& Shape::getTransformedPoint(std::size_t index) const
		{
			return m_transformedPoints[index];
		}

		void Shape::addPoint(const sf::Vector2f& point)
		{
			m_points.push_back(point);
			m_transformedPoints.push_back(m_transform.transformPoint(point));
			m_transformedVertecies.push_back(sf::Vertex(m_transform.transformPoint(point), m_fillColor));
			if (m_points.size() == 1)
			{
				m_transformedVertecies.push_back(m_transformedVertecies[0]);
			}
		}
		void Shape::addPoints(const std::vector<sf::Vector2f>& points)
		{
			m_points.insert(m_points.end(), points.begin(), points.end());
			updateTranformedPoints();
		}
		void Shape::setPoints(const std::vector<sf::Vector2f>& points)
		{
			m_points = points;
			updateTranformedPoints();
		}
		void Shape::removePoint(std::size_t index)
		{
			m_points.erase(m_points.begin() + index);
			m_transformedPoints.erase(m_transformedPoints.begin() + index);
			m_transformedVertecies.erase(m_transformedVertecies.begin() + index);
			if (index == 0)
			{
				m_transformedVertecies[m_transformedVertecies.size()-1] = m_transformedVertecies[0];
			}
		}

		const std::vector<sf::Vector2f>& Shape::getPoints() const
		{
			return m_points;
		}
		const std::vector<sf::Vector2f>& Shape::getTransformedPoints() const
		{
			return m_transformedPoints;
		}

		Utilities::AABB Shape::getLocalBounds() const
		{
			return Utilities::AABB::getFrame(m_points);
		}
		Utilities::AABB Shape::getGlobalBounds() const
		{
			return Utilities::AABB::getFrame(m_transformedPoints);
		}

		void Shape::updateTranformedPoints()
		{
			m_transformedPoints.clear();
			m_transformedVertecies.clear();
			m_transformedPoints.reserve(m_points.size());
			m_transformedVertecies.reserve(m_points.size()+1);
			for (auto& point : m_points)
			{
				m_transformedPoints.push_back(m_transform.transformPoint(point));
				m_transformedVertecies.push_back(sf::Vertex(m_transform.transformPoint(point), m_fillColor));
			}
			m_transformedVertecies.push_back(m_transformedVertecies[0]);
		}
		void Shape::updateVertexColors()
		{
			for (auto& vertex : m_transformedVertecies)
			{
				vertex.color = m_fillColor;
			}
		}
		/*void Shape::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if (m_fill)
			{
				target.draw(&m_transformedVertecies[0], m_transformedVertecies.size(), sf::TriangleFan, states);
			}
			if (m_outline)
			{
				std::vector<sf::Vertex> outline = m_transformedVertecies;
				for (auto& vertex : outline)
				{
					vertex.color = m_outlineColor;
				}
				target.draw(&outline[0], outline.size(), sf::LineStrip, states);
			}
		}*/
	}
}
