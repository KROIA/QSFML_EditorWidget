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

        void RectPainter::setColor(const sf::Color& color)
        {
            m_rectShape.setOutlineColor(color);
        }
        const sf::Color& RectPainter::getColor() const
        {
            return m_rectShape.getOutlineColor();
        }

        void RectPainter::draw(sf::RenderTarget& target,
                               sf::RenderStates states) const
        {
            target.draw(m_rectShape);
        }
    }
}
