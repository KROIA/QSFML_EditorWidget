#include "components/drawable/Shape.h"
#include "objects/base/GameObject.h"
#include <SFML/OpenGL.hpp>

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
		Shape::Shape(const Shape& other)
		{
			m_fillColor = other.m_fillColor;
			m_outlineColor = other.m_outlineColor;
			m_fill = other.m_fill;
			m_outline = other.m_outline;
			//m_outlineThickness = other.m_outlineThickness;
			m_points = other.m_points;
			//m_transformedPoints = other.m_transformedPoints;
			//m_transformedVertecies = other.m_transformedVertecies;

		}

		Shape::~Shape()
		{

		}

		COMPONENT_IMPL(Shape);

		/*void Shape::setTransform(const sf::Transform& transform)
		{
			m_transform = transform;
			updateTranformedPoints();
		}
		const sf::Transform& Shape::getTransform() const
		{
			return m_transform;
		}*/

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

		void Shape::setPointCount(std::size_t count)
		{
			m_points.resize(count);
		}
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
			//m_transformedPoints[index] = m_transform.transformPoint(point);
			//m_transformedVertecies[index].position = m_transformedPoints[index];
			/*if (index == 0)
			{
				m_transformedVertecies[m_transformedVertecies.size() - 1] = m_transformedVertecies[0];
			}*/
		}
		const sf::Vector2f& Shape::getPoint(std::size_t index) const
		{
			return m_points[index];
		}
		sf::Vector2f Shape::getTransformedPoint(std::size_t index) const
		{
			sf::Transform transform = getGlobalTransform();
			return transform.transformPoint(m_points[index]);
			//return m_transformedPoints[index];

		}

		void Shape::addPoint(const sf::Vector2f& point)
		{
			m_points.push_back(point);
			//m_transformedPoints.push_back(m_transform.transformPoint(point));
			//m_transformedVertecies.push_back(sf::Vertex(m_transform.transformPoint(point), m_fillColor));
			//if (m_points.size() == 1)
			//{
			//	m_transformedVertecies.push_back(m_transformedVertecies[0]);
			//}
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
			//m_transformedPoints.erase(m_transformedPoints.begin() + index);
			//m_transformedVertecies.erase(m_transformedVertecies.begin() + index);
			//if (index == 0)
			//{
			//	m_transformedVertecies[m_transformedVertecies.size()-1] = m_transformedVertecies[0];
			//}
		}

		const std::vector<sf::Vector2f>& Shape::getPoints() const
		{
			return m_points;
		}
		std::vector<sf::Vector2f> Shape::getTransformedPoints() const
		{
			std::vector<sf::Vector2f> transformedPoints;
			sf::Transform transform;
			if (m_parent)
				transform = m_parent->getGlobalTransform() * getTransform();
			else
				transform = getTransform();
			transformedPoints.reserve(m_points.size());
			for (auto& point : m_points)
			{
				transformedPoints.push_back(transform.transformPoint(point));
				//transformedPoints.push_back((point));
			}
			return transformedPoints;
		}

		Utilities::AABB Shape::getLocalBounds() const
		{
			return Utilities::AABB::getFrame(m_points);
		}
		Utilities::AABB Shape::getGlobalBounds() const
		{
			return Utilities::AABB::getFrame(getTransformedPoints());
		}

		void Shape::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if (m_points.size() == 0)
				return;
#ifdef QSFML_USE_GL_DRAW
			QSFML_UNUSED(target);
			QSFML_UNUSED(states);
			sf::Vector2f pos = states.transform.transformPoint({ 0, 0 });

			// Apply SFML transform
			glLoadMatrixf(states.transform.getMatrix());
			
			
			// use gl Calls. Fill the shape if fill is true
			if (m_fill)
			{
				glBegin(GL_TRIANGLE_FAN);
				glColor4ub(m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
				for (const auto& point : m_points)
				{
					glVertex2f(point.x, point.y);
				}				
				glEnd();
			}
			// use gl calls. Draw the outline if outline is true
			if (m_outline)
			{
				
				glLineWidth(m_outlineThickness);
				glBegin(GL_LINE_STRIP);
				
				glColor4ub(m_outlineColor.r, m_outlineColor.g, m_outlineColor.b, m_outlineColor.a);
				for (const auto& point : m_points)
				{
					glVertex2f(point.x, point.y);
				}
				glVertex2f(m_points[0].x, m_points[0].y);
				//glLineWidth(1);
				glEnd();
				glLineWidth(1);
				
			}
#else
			if (m_points.size() == 0)
			{
				return;
			}
			std::vector<sf::Vertex> transformedVertecies;
			transformedVertecies.reserve(m_points.size() + 1);
			for (const auto& point : m_points)
			{
				transformedVertecies.push_back(sf::Vertex(point, m_fillColor));
			}
			transformedVertecies.push_back(sf::Vertex(m_points[0], m_fillColor));
			if (m_fill)
			{
				target.draw(&transformedVertecies[0], transformedVertecies.size(), sf::TriangleFan, states);
			}
			if (m_outline)
			{
				for (auto& vertex : transformedVertecies)
				{
					vertex.color = m_outlineColor;
				}
				target.draw(&transformedVertecies[0], transformedVertecies.size(), sf::LineStrip, states);
			}
#endif
		}

		void Shape::updateTranformedPoints()
		{
			/*m_transformedPoints.clear();
			m_transformedVertecies.clear();
			m_transformedPoints.reserve(m_points.size());
			m_transformedVertecies.reserve(m_points.size()+1);
			for (auto& point : m_points)
			{
				m_transformedPoints.push_back(m_transform.transformPoint(point));
				m_transformedVertecies.push_back(sf::Vertex(m_transform.transformPoint(point), m_fillColor));
			}
			m_transformedVertecies.push_back(m_transformedVertecies[0]);*/
		}
		void Shape::updateVertexColors()
		{
			/*for (auto& vertex : m_transformedVertecies)
			{
				vertex.color = m_fillColor;
			}*/
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
