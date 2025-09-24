#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include "SFML/Graphics/Vertex.hpp"

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_API LinePainter : public Drawable
{
    public:
        struct LineData
        {
            sf::Vector2f start, end;
            sf::Color color;
            float thickness;

			// Baked data
			sf::Vector2f points[4];
			void updateGeometry();
        };
        LinePainter(const std::string &name = "LinePainter");
        LinePainter(const LinePainter &other);
        COMPONENT_DECL(LinePainter);

        void setPoints(const sf::Vector2f &start, const sf::Vector2f &end);
        void setPoints(size_t index, const sf::Vector2f &start, const sf::Vector2f &end);

        void addLine(const sf::Vector2f &start, const sf::Vector2f &end, const sf::Color &col, float thickness);
        void addLine(const sf::Vector2f &start, const sf::Vector2f &end);

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

        void clear();

        /*
        /// <summary>
        /// Setting to switch from relative to global position
        /// </summary>
        /// <description>
        /// Using relative position means that the position of the lines is relative to the parent object.
        /// Using global position means that the position of the lines is relative to the world.
        /// </description>
        /// <param name="useGlobal"></param>
        void useGlobalPosition(bool useGlobal)
        {
			m_useGlobalPosition = useGlobal;
        }

        /// <summary>
        /// Gets the positioning mode
        /// </summary>
        /// <returns>true if the mode is global positioning</returns>
        bool isUsingGlobalPosition() const
        {
            return m_useGlobalPosition;
        }*/

    private:
        void drawComponent(sf::RenderTarget& target,
            sf::RenderStates states) const override;

        float m_thickness;
        sf::Color m_color;
        //bool m_useGlobalPosition = false;

        QSFML::vector<LineData> m_lines;
};
}
}
