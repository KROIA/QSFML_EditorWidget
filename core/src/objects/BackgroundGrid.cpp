#include "objects/BackgroundGrid.h"

using namespace QSFML::Objects;

namespace QSFML
{
    namespace Objects
    {
        OBJECT_IMPL(BackgroundGrid)
        BackgroundGrid::BackgroundGrid(const std::string& name, GameObjectPtr parent)
            : GameObject(name, parent)
        {
            m_draw = new DrawableComp();
            m_draw->m_grid = this;
            setSize(sf::IntRect(0, 0, 500, 500));
            setGridSpacing(10);
            setLineColor({ sf::Color(130,130,130),sf::Color(100,100,100) });
            addComponent(m_draw);
        }
        BackgroundGrid::BackgroundGrid(const BackgroundGrid& other)
            : GameObject(other)
        {
            m_gridArea = other.m_gridArea;
            m_gridSpacing = other.m_gridSpacing;
            m_alternatingColors = other.m_alternatingColors;

            m_draw = new DrawableComp(*other.m_draw);
            m_draw->m_grid = this;

        }
        BackgroundGrid::~BackgroundGrid()
        {}

        void BackgroundGrid::setSize(const sf::IntRect& size)
        {
            m_gridArea = size;
        }
        const sf::IntRect& BackgroundGrid::getSize() const
        {
            return m_gridArea;
        }
        void BackgroundGrid::setLineColor(const std::vector<sf::Color>& alternatingColors)
        {
            m_alternatingColors = alternatingColors;
        }
        const std::vector<sf::Color>& BackgroundGrid::getLineColor() const
        {
            return m_alternatingColors;
        }
        void BackgroundGrid::setGridSpacing(unsigned int spacing)
        {
            m_gridSpacing = spacing;
        }
        unsigned int BackgroundGrid::getGridSpacing() const
        {
            return m_gridSpacing;
        }
        void BackgroundGrid::update()
        {

        }

        BackgroundGrid::DrawableComp::DrawableComp(const std::string& name)
            : Drawable(name)
        {
            m_grid = nullptr;
        }
        BackgroundGrid::DrawableComp::DrawableComp(const Drawable& other)
            : Drawable(other)
        {
            m_grid = nullptr;
        }
        void BackgroundGrid::DrawableComp::drawComponent(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            drawGrid(target, m_grid->m_gridArea, m_grid->m_gridSpacing,
                m_grid->m_alternatingColors, states);
        }
        void BackgroundGrid::DrawableComp::drawGrid(sf::RenderTarget& target,
            const sf::IntRect& area,
            unsigned int spacing,
            const std::vector<sf::Color>& alternatingColors,
            sf::RenderStates states) const
        {
            sf::Vector2f start((float)area.left, (float)area.top);
            sf::Vector2f end((float)area.left,
                (float)(area.top + area.height));
            sf::Vector2f VcurrentStart = start;
            sf::Vector2f VcurrentEnd = end;

            sf::Vector2f HcurrentStart = start;
            sf::Vector2f HcurrentEnd = start + sf::Vector2f((float)area.width, 0.f);


            size_t verticalCount = area.height / spacing;
            size_t horizontalCount = area.width / spacing;

            size_t colorIndex = 0;
            size_t colorCount = alternatingColors.size();

            for (size_t x = 0; x <= horizontalCount; ++x)
            {

                sf::Vertex line[] =
                {
                    sf::Vertex(VcurrentStart,alternatingColors[colorIndex]),
                    sf::Vertex(VcurrentEnd,alternatingColors[colorIndex])
                };
                colorIndex = (colorIndex + 1) % colorCount;


                VcurrentStart.x += spacing;
                VcurrentEnd.x += spacing;

                target.draw(line, 2, sf::Lines, states);

            }
            colorIndex = 0;
            for (size_t y = 0; y <= verticalCount; ++y)
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(HcurrentStart,alternatingColors[colorIndex]),
                    sf::Vertex(HcurrentEnd,alternatingColors[colorIndex])
                };
                colorIndex = (colorIndex + 1) % colorCount;

                HcurrentStart.y += spacing;
                HcurrentEnd.y += spacing;

                target.draw(line, 2, sf::Lines, states);

            }
        }
    }
}
