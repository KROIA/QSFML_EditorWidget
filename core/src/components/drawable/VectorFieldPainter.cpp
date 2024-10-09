#include "components/drawable/VectorFieldPainter.h"
#include <SFML/OpenGL.hpp>

namespace QSFML
{
	namespace Components
	{
		COMPONENT_IMPL(VectorFieldPainter);

		VectorFieldPainter::VectorFieldPainter(const std::string& name)
			: Drawable(name)
		{

		}

		void VectorFieldPainter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
			QSFML_UNUSED(target);
			// Apply SFML transform
			glLoadMatrixf(states.transform.getMatrix());
			glBegin(GL_LINES);
			for (const auto& el : m_field)
			{
				const sf::Vector2f start = el.position;
				const sf::Vector2f end = el.position + el.direction;
				//if (isVisible(start, target) || isVisible(end, target))
				//{
				glColor4ub(el.color.r, el.color.g, el.color.b, el.color.a);
				glVertex2f(start.x, start.y);
				glVertex2f(end.x, end.y);
				//}
			}
			glEnd();
		}

	}
}