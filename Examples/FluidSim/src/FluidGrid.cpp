#include "FluidGrid.h"

OBJECT_IMPL(FluidGrid)

FluidGrid::FluidGrid(sf::Vector2u tiles,
	sf::Vector2f tileSize, 
	const std::string& name,
	QSFML::Objects::GameObjectPtr parent)
	: GameObject(name, parent)
	, m_gridSize(tiles)
	, m_cellSize(tileSize)
	, m_cells(m_cells1)
{

	m_gridPainter = new GridPainter();
	m_gridPainter->setGridSize(m_gridSize);
	m_gridPainter->setCellSize(m_cellSize);
	m_gridPainter->setColor(sf::Color(128, 128, 128));
	addComponent(m_gridPainter);


	m_cells1.resize(m_gridSize.x * m_gridSize.y);
	m_cells2.resize(m_gridSize.x * m_gridSize.y);
}
FluidGrid::~FluidGrid()
{

}



void FluidGrid::update()
{
	
}
size_t FluidGrid::getCellIndex(const sf::Vector2u& cell) const
{
	return cell.x + cell.y * m_gridSize.x;
}
void FluidGrid::swapCells()
{
	if (&m_cells == &m_cells1)
	{
		m_cells = m_cells2;
	}
	else
	{
		m_cells = m_cells1;
	}
}


void FluidGrid::calculateDensity()
{


}
void FluidGrid::calculateVelocity()
{

}
void FluidGrid::applyBoundaryConditions()
{


}
void FluidGrid::diffuse(int b, QSFML::vector<CellData>& x, QSFML::vector<CellData>& x0, float diff, float dt)
{
	 
}
void FluidGrid::advect(int b, QSFML::vector<CellData>& d, QSFML::vector<CellData>& d0,
	QSFML::vector<CellData>& u, QSFML::vector<CellData>& v, float dt)
{

		
}
void FluidGrid::project(QSFML::vector<CellData>&u, QSFML::vector<CellData>&v, QSFML::vector<CellData>&p, QSFML::vector<CellData>&div)
{

}

void FluidGrid::setBoundary(int b, QSFML::vector<CellData>& x)
{

}
void FluidGrid::linearSolve(int b, QSFML::vector<CellData>& x, QSFML::vector<CellData>& x0, float a, float c)
{


}
void FluidGrid::addSource(QSFML::vector<CellData>& x, QSFML::vector<CellData>& s, float dt)
{

}
