#include "GridPainter.h"

GridPainter::GridPainter(const std::string& name)
	: Drawable(name)
{

}

GridPainter::GridPainter(const Drawable& other)
{

}
GridPainter::~GridPainter()
{

}

void GridPainter::setGridSize(const sf::Vector2u& size)
{
	m_gridSize = size;
	updateShape();
}
void GridPainter::setCellSize(const sf::Vector2f& size)
{
	m_cellSize = size;
	updateShape();
}
void GridPainter::setColor(const sf::Color& color)
{
	m_color = color;
	updateShape();
}
void GridPainter::setPosition(const sf::Vector2f& pos)
{
	m_pos = pos;
	updateShape();
}
void GridPainter::setCell(const sf::Vector2u& pos, const CellVisualisation& cell)
{
	m_cells[pos.y * m_gridSize.x + pos.x] = cell;
}
GridPainter::CellVisualisation& GridPainter::getCell(const sf::Vector2u& pos)
{
	return m_cells[pos.y * m_gridSize.x + pos.x];
}

void GridPainter::updateShape()
{
	m_horizontalLines.clear();
	m_verticalLines.clear();
	for (int i = 0; i < m_gridSize.x+1; i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(m_pos.x + i * m_cellSize.x, m_pos.y)),
			sf::Vertex(sf::Vector2f(m_pos.x + i * m_cellSize.x, m_pos.y + m_gridSize.y * m_cellSize.y))
		};
		line[0].color = m_color;
		line[1].color = m_color;
		m_verticalLines.push_back(line[0]);
		m_verticalLines.push_back(line[1]);
	}
	for (int i = 0; i < m_gridSize.y+1; i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(m_pos.x, m_pos.y + i * m_cellSize.y)),
			sf::Vertex(sf::Vector2f(m_pos.x + m_gridSize.x * m_cellSize.x, m_pos.y + i * m_cellSize.y))
		};
		line[0].color = m_color;
		line[1].color = m_color;
		m_horizontalLines.push_back(line[0]);
		m_horizontalLines.push_back(line[1]);
	}

	m_cells.clear();
	m_cells.resize(m_gridSize.x * m_gridSize.y);
	for (int i = 0; i < m_gridSize.x; i++)
	{
		for (int j = 0; j < m_gridSize.y; j++)
		{
			CellVisualisation cell;
			cell.m_color = sf::Color::Black;
			m_cells.push_back(cell);
		}
	}
}


void GridPainter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_horizontalLines.data(), m_horizontalLines.size(), sf::Lines, states);
	target.draw(m_verticalLines.data(), m_verticalLines.size(), sf::Lines, states);

	sf::RectangleShape cellRect;
	cellRect.setSize(m_cellSize);
	for (int i = 0; i < m_gridSize.x; i++)
	{
		for (int j = 0; j < m_gridSize.y; j++)
		{
			const CellVisualisation &cell = m_cells[j * m_gridSize.x + i];
			cellRect.setPosition(m_pos.x + i * m_cellSize.x, m_pos.y + j * m_cellSize.y);
			cellRect.setFillColor(cell.m_color);
			target.draw(cellRect, states);
		}
	}
}
