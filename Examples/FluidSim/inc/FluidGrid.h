#pragma once
#include "QSFML_EditorWidget.h"
#include "GridPainter.h"


class FluidGrid : public QSFML::Objects::CanvasObject
{
public:
	FluidGrid(sf::Vector2u tiles,
			  sf::Vector2f tileSize,
			  const std::string& name = "FluidGrid",
			  CanvasObject* parent = nullptr);
	~FluidGrid();

	CLONE_FUNC_DEC(FluidGrid);

private:
	struct CellData
	{
		sf::Vector2f velocity;
		float density;

		CellData()
		{
			velocity = sf::Vector2f(0, 0);
			density = 0;
		}
	};
	void update() override;

	size_t getCellIndex(const sf::Vector2u& cell) const;
	void swapCells();

	// Flud simulation
	void calculateDensity();
	void calculateVelocity();
	void applyBoundaryConditions();
	void diffuse(int b, std::vector<CellData>& x, std::vector<CellData>& x0, float diff, float dt);
	void advect(int b, std::vector<CellData>& d, std::vector<CellData>& d0, std::vector<CellData>& u, std::vector<CellData>& v, float dt);
	void project(std::vector<CellData>& u, std::vector<CellData>& v, std::vector<CellData>& p, std::vector<CellData>& div);
	void setBoundary(int b, std::vector<CellData>& x);
	void linearSolve(int b, std::vector<CellData>& x, std::vector<CellData>& x0, float a, float c);
	void addSource(std::vector<CellData>& x, std::vector<CellData>& s, float dt);




	sf::Vector2u m_gridSize;
	sf::Vector2f m_cellSize;

	GridPainter *m_gridPainter;

	std::vector<CellData> &m_cells;

	std::vector<CellData> m_cells1;
	std::vector<CellData> m_cells2;

	
};