#include "FluidGrid.h"

CLONE_FUNC_IMPL(FluidGrid)

FluidGrid::FluidGrid(sf::Vector2u tiles,
	sf::Vector2f tileSize, 
	const std::string& name,
	CanvasObject* parent)
	: CanvasObject(name, parent)
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
void FluidGrid::diffuse(int b, std::vector<CellData>& x, std::vector<CellData>& x0, float diff, float dt)
{
	 
}
void FluidGrid::advect(int b, std::vector<CellData>& d, std::vector<CellData>& d0,
	std::vector<CellData>& u, std::vector<CellData>& v, float dt)
{

		
}
void FluidGrid::project(std::vector<CellData>&u, std::vector<CellData>&v, std::vector<CellData>&p, std::vector<CellData>&div)
{

}

void FluidGrid::setBoundary(int b, std::vector<CellData>& x)
{

}
void FluidGrid::linearSolve(int b, std::vector<CellData>& x, std::vector<CellData>& x0, float a, float c)
{


}
void FluidGrid::addSource(std::vector<CellData>& x, std::vector<CellData>& s, float dt)
{

}
