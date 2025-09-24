#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"
#include "utilities/AABB.h"

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_API RectPainter : public Drawable
        {
        public:
            RectPainter(const std::string& name = "RectPainter");
            RectPainter(const RectPainter& other);
            COMPONENT_DECL(RectPainter);

            void setRect(const Utilities::AABB& aabb);
            const Utilities::AABB& getRect() const;

            void setOutlineThickness(float t);
            float getOutlineThickness() const;

            void setOutlineColor(const sf::Color& color);
            const sf::Color& getOutlineColor() const;

			void setFillColor(const sf::Color& color);
			const sf::Color& getFillColor() const;

            void drawComponent(sf::RenderTarget& target,
                sf::RenderStates states) const override;

        protected:
            Utilities::AABB m_box;

            sf::RectangleShape m_rectShape;
        };
    }
}
