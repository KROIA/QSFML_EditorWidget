#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"
#include "utilities/AABB.h"

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_EXPORT RectPainter : public Drawable
        {
        public:
            RectPainter(const std::string& name = "RectPainter");
            RectPainter(const RectPainter& other);
            COMPONENT_DECL(RectPainter);

            void setRect(const Utilities::AABB& aabb);
            const Utilities::AABB& getRect() const;

            void setColor(const sf::Color& color);
            const sf::Color& getColor() const;

            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override;

        private:
            Utilities::AABB m_box;

            sf::RectangleShape m_rectShape;
        };
    }
}
