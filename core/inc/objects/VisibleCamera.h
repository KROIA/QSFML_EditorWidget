#pragma once

#include "components/base/Drawable.h"
#include "objects/CameraController.h"
#include "QSFML_base.h"

namespace QSFML
{
namespace Objects
{
class QSFML_EDITOR_WIDGET_EXPORT VisibleCamera : public CameraController
{
        class DrawableComp;
    public:
        VisibleCamera(const std::string& name = "",
            CanvasObject* parent = nullptr);
        VisibleCamera(const VisibleCamera& other);
        ~VisibleCamera(){}
        OBJECT_DECL(VisibleCamera)

    private:
        DrawableComp *m_draw;

        class DrawableComp : public Components::Drawable
        {
            public:

                void setCam(VisibleCamera* cam);
            protected:
                void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
                void drawRect(sf::RenderTarget& target, const sf::FloatRect& rect, const sf::Color& color) const;


            private:
                VisibleCamera *m_cam;
        };
};



}
}
