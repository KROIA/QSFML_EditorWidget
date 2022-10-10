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

        void setSize(const sf::IntRect &size);
        const sf::IntRect &getsize() const;

        void setLineColor(const sf::Color &color);
        const sf::Color &getLineColor() const;

    private:
        sf::IntRect m_gridArea;
        unsigned int m_gridSpacing;

        DrawableComp *m_draw;

        friend DrawableComp;

};
class BackgroundGrid::DrawableComp: public DrawableComponent
{
    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

        BackgroundGrid *m_grid;

        sf::Color m_lineColor;
};
