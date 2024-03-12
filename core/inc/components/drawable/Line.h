#pragma once

#include "QSFML_base.h"
#include "components/base/Drawable.h"

#include "SFML/Graphics/Vertex.hpp"

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT Line : public Drawable
{
    public:
        Line(const std::string &name = "Line");
        Line(const Line &other);
        COMPONENT_DECL(Line);

        void setStartPos(const sf::Vector2f &pos);
        const sf::Vector2f &getStartPos() const;

        void setEndPos(const sf::Vector2f &pos);
        const sf::Vector2f &getEndPos() const;

        void setThickness(float thickness);
        float getThickness() const;

        void setColor(const sf::Color &color);
        const sf::Color &getColor() const;

        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const override;

    private:
        void updateShape();
        void updateColor();

        sf::Vertex m_relativeVertices[4];
        float m_thickness;
        sf::Color m_color;
        sf::Vector2f m_begin;
        sf::Vector2f m_end;
};
}
}