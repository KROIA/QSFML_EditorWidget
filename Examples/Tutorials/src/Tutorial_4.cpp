#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   

*/

struct Coeff
{
    float amplitude = 1;
    float phi = 0;
	float frequency = 1;
};
static std::vector<Coeff> coeffs;

void ExampleScene::setupTutorial_4(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 1 started");


	// Instantiate a GameObject with the name "MyObject"
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");
	Components::PathPainter* pathPainter = new Components::PathPainter();
	Components::LinePainter* linePainter = new Components::LinePainter();

	object->addComponent(linePainter);
	object->addComponent(pathPainter);

	pathPainter->setColor(sf::Color::Blue);

	for (size_t i = 0; i < 10; i++)
	{
		Coeff coeff;
		coeff.amplitude = Utilities::RandomEngine::getFloat(10, 50);
		coeff.phi = Utilities::RandomEngine::getFloat(0,M_2_PI);
		coeff.frequency = Utilities::RandomEngine::getFloat(1, 20);
		coeffs.push_back(coeff);
		linePainter->addLine({ 0, 0 }, { 0, 0 }, sf::Color::Green, 1);
	}

	// sort coeff for length
	std::sort(coeffs.begin(), coeffs.end(), [](const Coeff& a, const Coeff& b)
			  {
				  return a.amplitude > b.amplitude;
			  });

	object->setPosition({ 400, 300 });


    // Add an update function to the object
    object->addUpdateFunction([pathPainter, linePainter](GameObject& obj)
        {
			sf::Vector2f pos;
			float time = obj.getAge();
			for (size_t i = 0; i < coeffs.size(); i++)
			{
				Coeff& coeff = coeffs[i];
				sf::Vector2f vec = VectorMath::getRotatedUnitVectorRAD(coeff.phi + coeff.frequency * time) * coeff.amplitude;
				linePainter->setPoints(i, pos, pos + vec);
				pos += vec;
			}
			pathPainter->appenPoint(pos);

            
        });

	scene->addObject(object);
}