#pragma once

#include "Drawable.h"
#include "CameraController.h"

class VisibleCamera: public CameraController, public Drawable
{
    public:
        VisibleCamera(){}
        ~VisibleCamera(){}

        void draw(const sf::Vector2f &drawPos = sf::Vector2f(0,0)) override
        {
            sf::View view = getView();
            sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());
            float offset = 0.1;
            viewRect.height-=2*offset;
            viewRect.width-=2*offset;
            viewRect.left+=offset;
            viewRect.top+=offset;

            drawRect(viewRect, sf::Color(0,255,0));
            sf::FloatRect bounds = getMaxMovingBounds();
            bounds.height-=2*offset;
            bounds.width-=2*offset;
            bounds.left+=offset;
            bounds.top+=offset;
            drawRect(bounds, sf::Color(255,0,0));

        }
        void drawRect(const sf::FloatRect &rect, const sf::Color &color)
        {
            sf::Vector2f TL(rect.left,      rect.top);
            sf::Vector2f TR(TL.x+rect.width,    TL.y);
            sf::Vector2f BR(TR.x,   TR.y+rect.height);
            sf::Vector2f BL(TL.x,               BR.y);

            sf::Vertex line[] =
            {
                sf::Vertex(TL,color),
                sf::Vertex(TR,color),
                sf::Vertex(BR,color),
                sf::Vertex(BL,color),
                sf::Vertex(TL,color),
            };
            Drawable::draw(line, 5, sf::LineStrip);
        }

        void internalOnCanvasParentChange(QSFML_Canvas *newParent) override
        {
            CameraController::internalOnCanvasParentChange(newParent);
            Drawable::internalOnCanvasParentChange(newParent);
        }
        void internalOnParentChange(CanvasObject *newParent) override
        {
            CameraController::internalOnParentChange(newParent);
            Drawable::internalOnParentChange(newParent);
        }
};
