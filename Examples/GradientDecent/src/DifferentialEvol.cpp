#include "DifferentialEvol.h"

DifferentialEvol::DifferentialEvol()
	: GameObject("DifferentialEvol")
	, m_pointPainter(new QSFML::Components::PointPainter("Painter"))
	, m_pixelPainter(new QSFML::Components::PixelPainter())
	, m_differentialEvolution(2)
{
	

	sf::Vector2u size(1000, 1000);
	m_pixelPainter->setPixelCount(size);
	m_pixelPainter->setPixelSize(1);
	m_pixelPainter->clear();
	addComponent(m_pixelPainter);
	addComponent(m_pointPainter);

	m_differentialEvolution.setPopulationSize(100);
	m_differentialEvolution.setFitnessFunction(std::bind(&DifferentialEvol::objectiveFunction, this, std::placeholders::_1));
	m_differentialEvolution.setIndividualInitializer(std::bind(&DifferentialEvol::initializerFunction, this, std::placeholders::_1));
	m_differentialEvolution.resetPopulation();

	sf::Vector2f offset(-(float)size.x / 200.f, -(float)size.y / 200.f);
	for (unsigned int i = 0; i < size.x; ++i)
	{
		for (unsigned int j = 0; j < size.y; ++j)
		{
			sf::Vector2f pos = sf::Vector2f((float)i / 100.f, (float)j / 100.f) + offset;
			double value = objectiveFunction({ pos.x, pos.y });
			m_pixelPainter->setPixel(sf::Vector2u(i,j), getColor(value));
		}
	}
	m_pixelPainter->setPosition(sf::Vector2f( -250, -250));
}

void DifferentialEvol::update()
{
	//if (m_evolutionUpdateCounter % 10 == 0)
	{
		m_differentialEvolution.evolve();
		m_evolutionUpdateCounter = 0;
	}
	++m_evolutionUpdateCounter;
	const auto& best = m_differentialEvolution.getBestIndividual();
	const auto& all = m_differentialEvolution.getPopulation();
	m_pointPainter->clear();

	for (const auto& it : all)
	{
		sf::Color color = sf::Color::Blue;
		if (&it == &best)
			color = sf::Color::Green;
		m_pointPainter->addPoint(sf::Vector2f(it[0] * 100, it[1] * 100), 5, color);
	}
}



double DifferentialEvol::objectiveFunction(const std::vector<double>& parameters)
{
	double x1 = parameters[0];
	double x2 = parameters[1];

	double height = sin(x1 * 2) * sin(x2 * 2) + 0.001 * (x1 * x1 + x2 * x2);
	if (x1 < 0 && x2 < 0)
		height += 0.1 * (-5 + (x1 + 2.5) * (x1 + 2.5) + (x2 + 2.5) * (x2 + 2.5));
	height -= 0.5;
	//height *= 2;
	return -height;
}
void DifferentialEvol::initializerFunction(QSFML::Utilities::DifferentialEvolution::Individual& individual)
{
	individual[0] = QSFML::Utilities::RandomEngine::getFloat(2, 5);
	individual[1] = QSFML::Utilities::RandomEngine::getFloat(2, 5);
}

sf::Color DifferentialEvol::getColor(double value)
{
	// Color fade depending on value
	/*sf::Color color1 = sf::Color::Red;
	sf::Color color2 = sf::Color::Green;
	sf::Color color;
	value *= 0.05;
	color.r = (sf::Uint8)(color1.r + (color2.r - color1.r) * value);
	color.g = (sf::Uint8)(color1.g + (color2.g - color1.g) * value);
	color.b = (sf::Uint8)(color1.b + (color2.b - color1.b) * value);
	if(color.r < 5)
	{
		color.r = 0;
		color.g = 0;
		color.b = 0;
	}
	color.a = 255;*/



	return QSFML::Color::lerpLinear({ sf::Color::Magenta, sf::Color::Cyan, sf::Color::Green, sf::Color::Yellow }, value);
}