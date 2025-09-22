#pragma once
#include "QSFML_EditorWidget.h"
#include "GridPainter.h"


class FluidGrid : public QSFML::Objects::GameObject
{
public:
	FluidGrid(sf::Vector2u tiles,
			  sf::Vector2f tileSize,
			  const std::string& name = "FluidGrid",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	~FluidGrid();

	OBJECT_DECL(FluidGrid);

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
	void diffuse(int b, QSFML::vector<CellData>& x, QSFML::vector<CellData>& x0, float diff, float dt);
	void advect(int b, QSFML::vector<CellData>& d, QSFML::vector<CellData>& d0, QSFML::vector<CellData>& u, QSFML::vector<CellData>& v, float dt);
	void project(QSFML::vector<CellData>& u, QSFML::vector<CellData>& v, QSFML::vector<CellData>& p, QSFML::vector<CellData>& div);
	void setBoundary(int b, QSFML::vector<CellData>& x);
	void linearSolve(int b, QSFML::vector<CellData>& x, QSFML::vector<CellData>& x0, float a, float c);
	void addSource(QSFML::vector<CellData>& x, QSFML::vector<CellData>& s, float dt);




	sf::Vector2u m_gridSize;
	sf::Vector2f m_cellSize;

	GridPainter *m_gridPainter;

	QSFML::vector<CellData> &m_cells;

	QSFML::vector<CellData> m_cells1;
	QSFML::vector<CellData> m_cells2;

	
};
