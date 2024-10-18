#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   This tutorial visualizes a complex forier series in form of rotating vectors

   What you will learn:
   - How to use multiple components
   - How to animate vectors and the x/y projections

*/


struct Coeff
{
    float amplitude = 1; // Amplitude of the complex vector
    float phi = 0; // Phase of the complex vector
	float frequency = 1; // On which frequency the vector rotates
};
// List of Fourier coefficients
static std::vector<Coeff> coeffs;

void ExampleScene::setupTutorial_4(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 1 started");


	// Instantiate a GameObject with the name "MyObject"
    GameObject* object = new GameObject("MyObject");
	Components::PathPainter* pathPainter = new Components::PathPainter("ComplexPath");
	Components::PathPainter* pathPainterX = new Components::PathPainter("XAxisPlot");
	Components::PathPainter* pathPainterY = new Components::PathPainter("YAxisPlot");
	Components::LinePainter* linePainter = new Components::LinePainter("AttachedVectors");
	Components::PointPainter* pointPainter = new Components::PointPainter("ComplexSumPoint");

	object->addComponent(linePainter);
	object->addComponent(pathPainter);
	object->addComponent(pathPainterX);
	object->addComponent(pathPainterY);
	object->addComponent(pointPainter);

	pathPainter->setColor(sf::Color::Blue);
	pathPainterX->setColor(sf::Color::Red);
	pathPainterY->setColor(sf::Color::Green);
	pointPainter->setColor(sf::Color::Red);
	pointPainter->setRadius(3);


	// Fourier for square function
	for (size_t i = 1; i < 20; i++)
	{
		Coeff coeff;
		float N = (i*2-1);

		coeff.amplitude = 200 / (N * M_PI);
		coeff.frequency = N;
		coeff.phi = 0;

		coeffs.push_back(coeff);
	}

	// Fourier for Ramp shape
	/*for (size_t i = 1; i < 10; i++)
	{
		Coeff coeff;
		float N = (i * 2);

		coeff.amplitude = 100 / (N * M_PI);
		coeff.frequency = N;
		coeff.phi = 0;

		coeffs.push_back(coeff);
	}*/

	// Fill the list of lines and create a color gradient for vector 1 to N
	for (size_t i = 0; i < coeffs.size(); ++i)
	{
		sf::Color color = QSFML::Color::lerpLinear({ sf::Color::Green, sf::Color::Yellow, sf::Color::Red }, (float)i / ((float)coeffs.size()));
		linePainter->addLine({ 0, 0 }, { 0, 0 }, color, 1);
	}

	// sort coeff for length (looks bether in the visualisation)
	std::sort(coeffs.begin(), coeffs.end(), [](const Coeff& a, const Coeff& b)
			  {
				  return a.amplitude > b.amplitude;
			  });

	// Move the object to the middle of the screen
	object->setPosition({ 400, 300 });


    // Add an update function to the object that calculates the vectors and plots
    object->addUpdateFunction([pathPainter, pathPainterX, pathPainterY, linePainter, pointPainter](GameObject& obj)
        {
			// Use the age for a time source
			float time = obj.getAge()*5;

			// Calculate the sum of all rotating vectors
			sf::Vector2f sum;
			for (size_t i = 0; i < coeffs.size(); i++)
			{
				Coeff& coeff = coeffs[i];

				// Get a rotated vector with correct amplitude
				sf::Vector2f vec = VectorMath::getRotatedUnitVectorRAD(coeff.phi + coeff.frequency * time) * coeff.amplitude;

				// Update the visual vector 
				linePainter->setPoints(i, sum, sum + vec);

				// Add the new vector to the sum
				sum += vec;
			}

			auto points = pathPainter->getPath();
			auto pointsX = pathPainterX->getPath();
			auto pointsY = pathPainterY->getPath();
			float offset = obj.getDeltaT() * 100;
			points.push_back(sum);
			pointsX.push_back({200, sum.y });
			pointsY.push_back({ sum.x, 200 });
			if (points.size() > 1000)
			{
				points.erase(points.begin());
				pointsX.erase(pointsX.begin());
				pointsY.erase(pointsY.begin());
			}

			
			// Move the plots
			for (size_t i = 0; i < points.size(); ++i)
			{
				pointsX[i].x += offset;
				pointsY[i].y += offset;
			}

			pathPainter->setPath(points);
			pathPainterX->setPath(pointsX);
			pathPainterY->setPath(pointsY);
			pointPainter->setPoint(sum);
            
        });

	scene->addObject(object);
}