
#include "components/DrawableVector.h"
#include "math.h"



namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(DrawableVector)
            bool DrawableVector::m_flipYaxis = false;

        DrawableVector::DrawableVector(const std::string& name)
            : Drawable(name)
            , m_start(0, 0)
            , m_end(1, 1)
            , m_color(sf::Color::Green)
        {
            updateArrow();
        }
        DrawableVector::DrawableVector(const DrawableVector& other)
            : Drawable(other)
            , m_start(other.m_start)
            , m_end(other.m_end)
            , m_color(other.m_color)
        {
            updateArrow();
        }
        DrawableVector::~DrawableVector()
        {

        }

        void DrawableVector::setPoints(const sf::Vector2f& begin, const sf::Vector2f& end)
        {
            m_start = begin;
            m_end = end;
            updateArrow();
        }
        void DrawableVector::setStart(const sf::Vector2f& begin)
        {
            m_start = begin;
            updateArrow();
        }
        void DrawableVector::setEnd(const sf::Vector2f& end)
        {
            m_end = end;
            updateArrow();
        }
        void DrawableVector::setDirection(const sf::Vector2f& directionVec)
        {
            m_end = m_start + directionVec;
            updateArrow();
        }
        void DrawableVector::setColor(const sf::Color& color)
        {
            m_color = color;
            for (size_t i = 0; i < 6; ++i)
                m_lines[i].color = m_color;
        }

        const sf::Vector2f& DrawableVector::getStart() const
        {
            return m_start;
        }
        const sf::Vector2f& DrawableVector::getEnd() const
        {
            return m_end;
        }
        const sf::Color& DrawableVector::getColor() const
        {
            return m_color;
        }
        sf::Vector2f DrawableVector::getDirection() const
        {
            return (m_end - m_start);
        }


        void DrawableVector::draw(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            target.draw(m_lines, 6, sf::Lines);
        }
        void DrawableVector::setInvertedYAxis(bool doInvert)
        {
            m_flipYaxis = doInvert;
        }
        void DrawableVector::updateArrow()
        {
            sf::Vector2f dir = getDirection();
            sf::Vector2f end = m_end;
            if (m_flipYaxis)
            {
                dir.y = -dir.y;
                end.y = m_start.y + dir.y;
            }
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            float angle = getAngle(dir);

            static const float arrowTipAngle = M_PI * 4.f / 5.f;

            if (length < 30)
                length = 30;
            else if (length > 200)
                length = 200;
            sf::Vector2f arrowTip(0.1f * length, 0);
            sf::Vector2f arrowLeft = getRotated(arrowTip, angle - arrowTipAngle);
            sf::Vector2f arrowRight = getRotated(arrowTip, angle + arrowTipAngle);

            m_lines[0].position = m_start;
            m_lines[1].position = end;

            m_lines[2].position = end;
            m_lines[3].position = end + arrowLeft;

            m_lines[4].position = end;
            m_lines[5].position = end + arrowRight;
        }
        sf::Vector2f DrawableVector::getRotated(const sf::Vector2f& vec,
            float angle) const
        {
            float c = cos(angle);
            float s = sin(angle);
            return sf::Vector2f(c * vec.x - s * vec.y,
                s * vec.x + c * vec.y);
        }
        float DrawableVector::getLength(const sf::Vector2f& vec) const
        {
            return sqrt(vec.x * vec.x + vec.y * vec.y);
        }
        float DrawableVector::getAngle(const sf::Vector2f& vec) const
        {
            float l = getLength(vec);
            if (l == 0)
                return 0;
            if (vec.x < 0)
                return M_PI - asin(vec.y / l);
            return asin(vec.y / l);
        }

    }
}