#include "BackgroundGrid.h"



BackgroundGrid::BackgroundGrid(const std::string &name,
                               CanvasObject *parent)
    : CanvasObject(name,parent)
{
    addComponent(new DrawableComp());
}
BackgroundGrid::~BackgroundGrid()
{}

void BackgroundGrid::DrawableComp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2f start(0,0);
    sf::Vector2f end(500,0);
    for(int i=0; i<10; ++i)
    {
        end.y = start.y;
        sf::Vertex line[] =
        {
            sf::Vertex(start),
            sf::Vertex(end)
        };

        target.draw(line, 2, sf::Lines);
        start.y += 20;
    }
}
