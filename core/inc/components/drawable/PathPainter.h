#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include "SFML/Graphics/Vertex.hpp"

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_API PathPainter : public Drawable
        {
            public:
            PathPainter(const std::string& name = "PathPainter");
            PathPainter(const PathPainter& other);
            COMPONENT_DECL(PathPainter);

			void setPath(const QSFML::vector<sf::Vector2f>& path);
            QSFML::vector<sf::Vector2f> getPath() const;

			void appenPoint(const sf::Vector2f& point);
			void popPointAtEnd(size_t count = 1);
			void popPointAtStart(size_t count = 1);

            const sf::Vector2f& getPoint(size_t index) const;
			size_t getPointCount() const
			{
				return m_vertecies.size();
			}
			void clear()
			{
				m_vertecies.clear();
			}
			void reserve(size_t size)
			{
				m_vertecies.reserve(size);
			}

			void setThickness(float thickness) { m_thickness = thickness; }
           // void setThickness(size_t index, float thickness);
		   float getThickness() const { return m_thickness; }
           // float getThickness(size_t index) const;

            void setColor(const sf::Color& color);
            void setColor(size_t index, const sf::Color& color);
            const sf::Color& getColor() const;
            const sf::Color& getColor(size_t index) const;

			void setFadeColor(const QSFML::vector<sf::Color>& colors)
			{
				m_fadeColors = colors;
				m_enableFadeColor = true;
			}
			const QSFML::vector<sf::Color>& getFadeColor() const
			{
				return m_fadeColors;
			}
			void enableFadeColor(bool enable)
			{
				m_enableFadeColor = enable;
			}
			bool isEnableFadeColor() const
			{
				return m_enableFadeColor;
			}

            private:
            void drawComponent(sf::RenderTarget& target,
                               sf::RenderStates states) const override;

            float m_thickness;
            sf::Color m_color;
            bool m_enableFadeColor;
			QSFML::vector<sf::Color> m_fadeColors;

            mutable QSFML::vector<sf::Vertex> m_vertecies;

        };
    }
}
