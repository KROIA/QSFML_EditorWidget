#pragma once

#include "CanvasObject.h"
#include "DrawableComponent.h"


class BackgroundGrid: public CanvasObject
{
        class DrawableComp;
    public:
        BackgroundGrid(const std::string &name = "",
                       CanvasObject *parent = nullptr);
        virtual ~BackgroundGrid();

    private:


};
class BackgroundGrid::DrawableComp: public DrawableComponent
{
    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
};
