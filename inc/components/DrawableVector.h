#pragma once

#include "Drawable.h"

namespace QSFML
{
namespace Components
{
class DrawableVector: public Drawable
{
    public:
    DrawableVector(const std::string &name = "DrawableVector");

    void setPoints(const sf::Vector2f &begin, const sf::Vector2f &end);
    void setStart(const sf::Vector2f &begin);
    void setEnd(const sf::Vector2f &end);
    void setDirection(const sf::Vector2f &directionVec);
    void setColor(const sf::Color &color);

    const sf::Vector2f &getStart() const;
    const sf::Vector2f &getEnd() const;
    sf::Vector2f getDirection() const;
    const sf::Color &getColor() const;


    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override;

    protected:
    inline sf::Vector2f getRotated(const sf::Vector2f &vec,
                                   float angle) const;
    inline float getLength(const sf::Vector2f &vec) const;
    inline float getAngle(const sf::Vector2f &vec) const;

    sf::Vector2f m_start, m_end;
    sf::Color m_color;
};
}
}
