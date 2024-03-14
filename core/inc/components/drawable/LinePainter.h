#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include "SFML/Graphics/Vertex.hpp"

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT LinePainter : public Drawable
{
    public:
        struct LineData
        {
            sf::Vector2f start, end;
            sf::Color color;
            float thickness;
        };
        LinePainter(const std::string &name = "LinePainter");
        LinePainter(const LinePainter &other);
        COMPONENT_DECL(LinePainter);

        void set(const sf::Vector2f &start, const sf::Vector2f &end);
        void set(size_t index, const sf::Vector2f &start, const sf::Vector2f &end);
        const sf::Vector2f &getStartPos() const;
        const sf::Vector2f &getStartPos(size_t index) const;
        const sf::Vector2f &getEndPos() const;
        const sf::Vector2f &getEndPos(size_t index) const;

        void setThickness(float thickness);
        void setThickness(size_t index, float thickness);
        float getThickness() const;
        float getThickness(size_t index) const;

        void setColor(const sf::Color &color);
        void setColor(size_t index, const sf::Color &color);
        const sf::Color &getColor() const;
        const sf::Color &getColor(size_t index) const;

        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const override;

    private:
        //void updateShape();
        //void updateColor();

        //sf::Vertex m_relativeVertices[4];
        float m_thickness;
        sf::Color m_color;
        //sf::Vector2f m_begin;
        //sf::Vector2f m_end;

        std::vector<LineData> m_lines;
};
}
}
