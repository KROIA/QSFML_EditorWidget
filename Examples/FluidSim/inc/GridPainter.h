#pragma once
#include "QSFML_EditorWidget.h"


class GridPainter : public QSFML::Components::Drawable
{
public:
    struct CellVisualisation
	{
		sf::Color m_color;
	};
    

    GridPainter(const std::string& name = "GridPainter");
    GridPainter(const Drawable& other);
    ~GridPainter();

    void setGridSize(const sf::Vector2u& size);
    void setCellSize(const sf::Vector2f& size);
    void setColor(const sf::Color& color);
    void setPosition(const sf::Vector2f& pos);

    void setCell(const sf::Vector2u & pos, const CellVisualisation& cell);
    CellVisualisation& getCell(const sf::Vector2u & pos);

    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void updateShape();
    sf::Vector2u m_gridSize;
    sf::Vector2f m_cellSize;
    sf::Color m_color;
    sf::Vector2f m_pos;

    std::vector<sf::Vertex> m_horizontalLines;
    std::vector<sf::Vertex> m_verticalLines;


    std::vector<CellVisualisation> m_cells;
};
