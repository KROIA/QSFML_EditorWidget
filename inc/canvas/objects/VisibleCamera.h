#pragma once

#include "DrawableComponent.h"
#include "CameraController.h"

class VisibleCamera : public CameraController
{
        class DrawableComp;
    public:
        VisibleCamera(const std::string &name = "",
                      CanvasObject *parent = nullptr)
            :   CameraController(name,parent)
        {
            DrawableComp *draw = new DrawableComp();
            draw->setCam(this);
            addComponent(draw);
        }
        ~VisibleCamera(){}



        /*void internalOnCanvasParentChange(QSFML_Canvas *newParent) override
        {
            CameraController::internalOnCanvasParentChange(newParent);
            DrawableComponent::internalOnCanvasParentChange(newParent);
        }
        void internalOnParentChange(CanvasObject *newParent) override
        {
            CameraController::internalOnParentChange(newParent);
            DrawableComponent::internalOnParentChange(newParent);
        }*/

    private:


        class DrawableComp : public DrawableComponent
        {
            public:

                void setCam(VisibleCamera *cam)
                {
                    m_cam = cam;
                }
            protected:
                void draw(sf::RenderTarget& target, sf::RenderStates states) const final
                {
                    sf::View view = m_cam->getView();
                    sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());
                    float offset = 0.1;
                    viewRect.height-=2*offset;
                    viewRect.width-=2*offset;
                    viewRect.left+=offset;
                    viewRect.top+=offset;

                    drawRect(target, viewRect, sf::Color(0,255,0));
                    sf::FloatRect bounds = m_cam->getMaxMovingBounds();
                    bounds.height-=2*offset;
                    bounds.width-=2*offset;
                    bounds.left+=offset;
                    bounds.top+=offset;
                    drawRect(target, bounds, sf::Color(255,0,0));

                }
                void drawRect(sf::RenderTarget& target, const sf::FloatRect &rect, const sf::Color &color) const
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
                    target.draw(line, 5, sf::LineStrip);
                }


            private:
                VisibleCamera *m_cam;
        };
};
