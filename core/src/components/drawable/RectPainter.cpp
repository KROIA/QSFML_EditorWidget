#include "components/drawable/RectPainter.h"

namespace QSFML
{
    namespace Components
    {
        RectPainter::RectPainter(const std::string& name)
            : Drawable(name)
        {
            m_rectShape.setFillColor(sf::Color(0, 0, 0, 0));
        }
        RectPainter::RectPainter(const RectPainter& other)
            : Drawable(other)
            , m_box(other.m_box)
            , m_rectShape(other.m_rectShape)
        {
            
        }
        COMPONENT_IMPL(RectPainter);

        void RectPainter::setRect(const Utilities::AABB& aabb)
        {
            m_box = aabb;
            m_rectShape.setPosition(m_box.TL());
            m_rectShape.setSize(m_box.getSize());
        }
        const Utilities::AABB& RectPainter::getRect() const
        {
            return m_box;
        }

        void RectPainter::setOutlineThickness(float t)
        {
            m_rectShape.setOutlineThickness(t);
        }
        float RectPainter::getOutlineThickness() const
        {
            return m_rectShape.getOutlineThickness();
        }

        void RectPainter::setOutlineColor(const sf::Color& color)
        {
            m_rectShape.setOutlineColor(color);
        }
        const sf::Color& RectPainter::getOutlineColor() const
        {
            return m_rectShape.getOutlineColor();
        }

		void RectPainter::setFillColor(const sf::Color& color)
		{
			m_rectShape.setFillColor(color);
		}
		const sf::Color& RectPainter::getFillColor() const
		{
			return m_rectShape.getFillColor();
		}

        void RectPainter::drawComponent(sf::RenderTarget& target,
                               sf::RenderStates states) const
        {
            target.draw(m_rectShape, states);
        }
    }
}
