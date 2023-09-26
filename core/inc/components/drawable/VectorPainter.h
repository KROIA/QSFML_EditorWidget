#pragma once

#include "QSFML_base.h"
#include "components/base/Drawable.h"

#include <SFML/Graphics.hpp>

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT VectorPainter: public Drawable
{
    public:
    VectorPainter(const std::string &name = "VectorPainter");
    VectorPainter(const VectorPainter &other);
    ~VectorPainter();
    COMPONENT_DECL(VectorPainter);

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

    static void setInvertedYAxis(bool doInvert);

    protected:
    void updateArrow();
    inline sf::Vector2f getRotated(const sf::Vector2f &vec,
                                   float angle) const;
    inline float getLength(const sf::Vector2f &vec) const;
    inline float getAngle(const sf::Vector2f &vec) const;

    sf::Vector2f m_start, m_end;
    sf::Color m_color;
    static bool m_flipYaxis;

    sf::Vertex m_lines[6];
};
}
}
