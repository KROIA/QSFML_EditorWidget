#include "BackgroundGrid.h"



BackgroundGrid::BackgroundGrid(const std::string &name,
                               CanvasObject *parent)
    : CanvasObject(name,parent)
{
    m_draw = new DrawableComp();
    m_draw->m_grid = this;
    m_gridArea = sf::IntRect(0,0,1000,800);
    m_gridSpacing = 10;
    addComponent(m_draw);
}
BackgroundGrid::~BackgroundGrid()
{}

void BackgroundGrid::setSize(const sf::IntRect &size)
{
    m_gridArea = size;
}
const sf::IntRect &BackgroundGrid::getsize() const
{
    return m_gridArea;
}
void BackgroundGrid::setLineColor(const sf::Color &color)
{
    m_draw->m_lineColor = color;
}
const sf::Color &BackgroundGrid::getLineColor() const
{
    return m_draw->m_lineColor;
}

void BackgroundGrid::DrawableComp::draw(sf::RenderTarget& target,
                                        sf::RenderStates states) const
{
    sf::Vector2f start(m_grid->m_gridArea.left,m_grid->m_gridArea.top);
    sf::Vector2f end(m_grid->m_gridArea.left,
                     m_grid->m_gridArea.top+m_grid->m_gridArea.height);
    sf::Vector2f VcurrentStart = start;
    sf::Vector2f VcurrentEnd = end;

    sf::Vector2f HcurrentStart = start;
    sf::Vector2f HcurrentEnd = end;

    size_t spacing = m_grid->m_gridSpacing;

    size_t verticalCount = m_grid->m_gridArea.height  / spacing;
    size_t horizontalCount = m_grid->m_gridArea.width / spacing;

    for(size_t x=0; x<horizontalCount; ++x)
    {

        sf::Vertex line[] =
        {
            sf::Vertex(VcurrentStart),
            sf::Vertex(VcurrentEnd)
        };


        VcurrentStart.x += spacing;
        VcurrentEnd.x   += spacing;

        target.draw(line, 2, sf::Lines);

        sf::Vertex line2[] =
        {
            sf::Vertex(sf::Vector2f(VcurrentStart.y,VcurrentStart.x)),
            sf::Vertex(sf::Vector2f(VcurrentEnd.y,VcurrentEnd.x))
        };
        target.draw(line2, 2, sf::Lines);
    }
    /*for(size_t y=0; y<verticalCount; ++y)
    {



        sf::Vertex line[] =
        {
            sf::Vertex(HcurrentStart),
            sf::Vertex(HcurrentEnd)
        };
        HcurrentStart.y += spacing;
        HcurrentEnd.y   += spacing;

        target.draw(line, 2, sf::Lines);

    }*/
}
