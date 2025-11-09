#pragma once
#include "QSFML_EditorWidget.h"

class DifferentialEvol : public QSFML::Objects::GameObject
{
public:
	DifferentialEvol();

protected:
	void update() override;

private:

	double objectiveFunction(const std::vector<double>& parameters);
	void initializerFunction(QSFML::Utilities::DifferentialEvolution::Individual& individual);
	sf::Color getColor(double value);

	size_t m_evolutionUpdateCounter = 0;

	QSFML::Components::PointPainter* m_pointPainter;
	QSFML::Components::PixelPainter* m_pixelPainter;
	QSFML::Utilities::DifferentialEvolution m_differentialEvolution;
};