#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include <vector>

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_EXPORT PixelPainter : public Drawable
        {
        public:
            PixelPainter(const std::string& name = "PixelPainter");
            PixelPainter(const PixelPainter& other);
            COMPONENT_DECL(PixelPainter);
            ~PixelPainter();

            void setPixelSize(float size);
            float getPixelSize() const;

            void setPixelCount(const sf::Vector2u& count);
            const sf::Vector2u& getPixelCount() const;

            void setPosition(const sf::Vector2f& pos);
            const sf::Vector2f& getPosition() const;

            void setPixel(const sf::Vector2u& pos, const sf::Color &c);
            const sf::Color& getPixel(const sf::Vector2u& pos) const;

            void clear();

            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override;

        private:
            sf::Vector2f m_position;
            sf::Vector2u m_pixelCount;
            float m_pixelSize;

            sf::Uint8* m_pixels;
            mutable sf::Texture m_texture;
        };
    }
}
