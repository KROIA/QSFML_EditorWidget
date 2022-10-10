#pragma once

#include "CanvasObject.h"
#include "Drawable.h"

namespace QSFML
{
namespace Objects
{
class BackgroundGrid: public CanvasObject
{
        class DrawableComp;
    public:
        BackgroundGrid(const std::string &name = "",
                       CanvasObject *parent = nullptr);
        virtual ~BackgroundGrid();

        void setSize(const sf::IntRect &size);
        const sf::IntRect &getSize() const;

        void setLineColor(const std::vector<sf::Color> &alternatingColors);
        const std::vector<sf::Color> &getLineColor() const;

        void setGridSpacing(unsigned int spacing);
        unsigned int getGridSpacing() const;

    private:
        sf::IntRect m_gridArea;
        unsigned int m_gridSpacing;
        std::vector<sf::Color> m_alternatingColors;

        DrawableComp *m_draw;

        friend DrawableComp;

};
class BackgroundGrid::DrawableComp: public Components::Drawable
{
    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

        void drawGrid(sf::RenderTarget& target,
                      const sf::IntRect &area,
                      unsigned int spacing,
                      const std::vector<sf::Color> &alternatingColors) const;

        BackgroundGrid *m_grid;
};
}
}
