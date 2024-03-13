
#include "components/drawable/VectorPainter.h"
#include "utilities/VectorOperations.h"

#define _USE_MATH_DEFINES
#include <math.h>



namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(VectorPainter)
            bool VectorPainter::m_flipYaxis = false;

        VectorPainter::VectorPainter(const std::string& name)
            : Drawable(name)
            , m_start(0, 0)
            , m_end(1, 1)
            , m_color(sf::Color::Green)
        {
            updateArrow();
        }
        VectorPainter::VectorPainter(const VectorPainter& other)
            : Drawable(other)
            , m_start(other.m_start)
            , m_end(other.m_end)
            , m_color(other.m_color)
        {
            updateArrow();
        }
        VectorPainter::~VectorPainter()
        {

        }

        void VectorPainter::setPoints(const sf::Vector2f& begin, const sf::Vector2f& end)
        {
            m_start = begin;
            m_end = end;
            updateArrow();
        }
        void VectorPainter::setStart(const sf::Vector2f& begin)
        {
            m_start = begin;
            updateArrow();
        }
        void VectorPainter::setEnd(const sf::Vector2f& end)
        {
            m_end = end;
            updateArrow();
        }
        void VectorPainter::setDirection(const sf::Vector2f& directionVec)
        {
            m_end = m_start + directionVec;
            updateArrow();
        }
        void VectorPainter::setAngle(float angle)
        {
            float length = VectorMath::getLength(m_end - m_start);
            m_end = m_start + VectorMath::getRotatedUnitVector(angle) * length;
            updateArrow();
        }
        void VectorPainter::setColor(const sf::Color& color)
        {
            m_color = color;
            for (size_t i = 0; i < 6; ++i)
                m_lines[i].color = m_color;
        }

        const sf::Vector2f& VectorPainter::getStart() const
        {
            return m_start;
        }
        const sf::Vector2f& VectorPainter::getEnd() const
        {
            return m_end;
        }
        const sf::Color& VectorPainter::getColor() const
        {
            return m_color;
        }
        sf::Vector2f VectorPainter::getDirection() const
        {
            return (m_end - m_start);
        }


        void VectorPainter::draw(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            target.draw(m_lines, 6, sf::Lines);
        }
        void VectorPainter::setInvertedYAxis(bool doInvert)
        {
            m_flipYaxis = doInvert;
        }
        void VectorPainter::updateArrow()
        {
            sf::Vector2f dir = getDirection();
            sf::Vector2f end = m_end;
            if (m_flipYaxis)
            {
                dir.y = -dir.y;
                end.y = m_start.y + dir.y;
            }
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            float angle = VectorMath::getAngle(dir);

            static const float arrowTipAngle = M_PI * 4.f / 5.f;

            if (length < 30)
                length = 30;
            else if (length > 200)
                length = 200;
            sf::Vector2f arrowTip(0.1f * length, 0);
            sf::Vector2f arrowLeft = VectorMath::getRotated(arrowTip, angle - arrowTipAngle);
            sf::Vector2f arrowRight = VectorMath::getRotated(arrowTip, angle + arrowTipAngle);

            m_lines[0].position = m_start;
            m_lines[1].position = end;

            m_lines[2].position = end;
            m_lines[3].position = end + arrowLeft;

            m_lines[4].position = end;
            m_lines[5].position = end + arrowRight;
        }
    }
}