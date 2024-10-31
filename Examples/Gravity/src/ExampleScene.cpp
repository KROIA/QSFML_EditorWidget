#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "Planet.h"
#include <QDir>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

//#define ENABLE_SCREEN_CAPTURE

using namespace QSFML;
using namespace QSFML::Objects;

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    Log::UI::NativeConsoleView::createStaticInstance();
    Log::UI::NativeConsoleView::getStaticInstance()->show();
	
    
    m_worldSize = 2000;
	
    setupGeneralObjects();

    setupVectorField();
    //setupScene_solarSystem();
    setupScene_gravityAssist_accelerate();
    //setupScene_gravityAssist_decelerate();
	
   
    m_scene->start();
#ifdef ENABLE_SCREEN_CAPTURE
	Utilities::CameraRecorder* recorder = new Utilities::CameraRecorder(m_scene->getDefaultCamera(),8);
	QTimer* timer = new QTimer(this);
	timer->singleShot(1000, [recorder]()
		{
			// Take 1000 screenshots with 0.01 seconds delay
			recorder->startCapture(100, 0.4, "screenshots/gravity");
		});
	timer->start();
#endif    
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_scene;
}

void ExampleScene::setupGeneralObjects()
{
    SceneSettings settings;
    settings.layout.fixedSize = sf::Vector2u(400, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    settings.timing.physicsFixedDeltaT = 0.5;
    settings.timing.physicsDeltaTScale = 2;

    m_scene = new Scene(ui->SceneWidget, settings);


    m_planetSystem = new GameObject("PlanetSystem");

    DefaultEditor* defaultEditor = new DefaultEditor("Editor", sf::FloatRect(-m_worldSize / 2, -m_worldSize / 2, m_worldSize, m_worldSize));
    defaultEditor->setRenderLayer(RenderLayer::layer_0);
    defaultEditor->getGrid()->setEnabled(false);
    m_scene->addObject(defaultEditor);


    // The update function is called every frame and calculates the vector field
    m_planetSystem->addUpdateFunction([](GameObject&)
        {
            // The dynamics for each planet is already calculated in each planets update function a
            // and just has to be applied here. This ensures that the planets movement is in sync with each other
            const QSFML::vector<Planet*>& planets = Planet::getPlanets();
            for (auto planet : planets)
            {
                planet->applyVelocity();
            }
        });

	m_planetSystem->addDrawFunction(
		[](const GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
		{
			// Draw Kinetic Energy using ImGui
			//ImGui::Begin("Kinetic Energy");
			const QSFML::vector<Planet*>& planets = Planet::getPlanets();
			for (auto planet : planets)
			{
                if (planet->isShowingPlots())
                    planet->drawPlots();
			}
			//ImGui::End();
		});

    // Define in which order the gameObject updates
    m_planetSystem->setUpdateOrder(
        {
            QSFML::Objects::GameObject::UpdateSequenceElement::childs,
            QSFML::Objects::GameObject::UpdateSequenceElement::components,
            QSFML::Objects::GameObject::UpdateSequenceElement::customUpdateFunctions
        });
	m_planetSystem->setRenderLayer(RenderLayer::layer_1);
    m_planetSystem->updateObjectChanges();
    qDebug() << m_planetSystem->toString().c_str();

    m_scene->addObject(m_planetSystem);
}
void ExampleScene::setupVectorField()
{
	GameObject* vectorFieldObject = new GameObject("VectorField");
    QSFML::Components::VectorFieldPainter* vectorField = new QSFML::Components::VectorFieldPainter();
    vectorFieldObject->addComponent(vectorField);

    Planet::setEnableCollision(false);
    //Planet::setWorldBounds(sf::FloatRect(-worldSize * 0.5, -worldSize * 0.5, worldSize, worldSize));

    int spacing = 10;
    float vecNormalScale = spacing / 2;

    // Building the vector field buffer
    QSFML::vector<QSFML::Components::VectorFieldPainter::Element> field;
    field.reserve((m_worldSize / spacing) * (m_worldSize / spacing));
    for (int x = -m_worldSize * 0.5; x < m_worldSize * 0.5; x += spacing)
    {
        for (int y = -m_worldSize * 0.5; y < m_worldSize * 0.5; y += spacing)
        {
            QSFML::Components::VectorFieldPainter::Element element;
            element.position = sf::Vector2f(x, y);
            element.direction = sf::Vector2f(10, 5);
            field.push_back(element);
        }
    }
    vectorField->setField(field);

	vectorFieldObject->addUpdateFunction(
        [vectorField, vecNormalScale](GameObject& obj)
		{
            // Calculate vector Field
            for (auto& fieldElement : *vectorField)
            {
                sf::Vector2f dir = Planet::calculateForce(fieldElement.position) * 10.f;
                float magnitudeSqr = dir.x * dir.x + dir.y * dir.y;
                const static QSFML::vector<sf::Color> colors = { sf::Color::Blue, sf::Color::Cyan, sf::Color::Green, sf::Color::Yellow, sf::Color(200,200,0), sf::Color::Red };
                fieldElement.color = Color::lerpCubic(colors, pow(magnitudeSqr, 0.3));
                dir = VectorMath::getNormalized(dir) * vecNormalScale;
                fieldElement.direction = dir;
            }
		});

	m_scene->addObject(vectorFieldObject);
}
void ExampleScene::setupGradientPainter()
{
    m_planetSystem->addDrawFunction(
        [this](const GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
        {
            //Draw contour line
            const size_t numLines = 10;
            const size_t vertexCount = 1000;
            const float stepsize = 10;


            sf::VertexArray contourLines(sf::Lines);
            contourLines.resize(vertexCount);

            float startX = -m_worldSize / 2;
            float xIncrement = m_worldSize / numLines;
            for (size_t j = 0; j < numLines; ++j)
            {
                // Start lines at equal spaced over the X-Axis
                sf::Vector2f start = sf::Vector2f(startX + j * xIncrement, 0);

                // Start with color white
                sf::Color color = sf::Color::White;

                for (size_t i = 0; i < vertexCount; ++i)
                {
                    sf::Vector2f gradient = Planet::calculateForce(start);
                    sf::Vector2f end = start + QSFML::VectorMath::getNormalized(QSFML::VectorMath::getRotatedRAD(gradient, M_PI_2)) * stepsize;
                    contourLines[i].position = start;
                    contourLines[i].color = color;
                    contourLines[i + 1].position = end;
                    contourLines[i + 1].color = color;
                    start = end;
                    i++;

                    // Fade the color to transparent
                    color.a = 255 - (i * 255 / vertexCount);
                }
                target.draw(contourLines, states);
            }
        });
}
void ExampleScene::setupScene_solarSystem()
{ 
	// Create planets with a random position and velocity
    int positionRange = m_worldSize / 2;
	unsigned int numPlanets = 5;
    Planet* sun = new Planet();
    sun->setMass(1000);
    m_planetSystem->addChild(sun);
    for (size_t i = 0; i < numPlanets; ++i)
    {
        Planet* planet = new Planet();
		float distanceFromCenter = (i + 1) * positionRange / numPlanets;
		float mass = (rand() % 10) + 1;
        sf::Vector2f randPos = VectorMath::getRotatedUnitVector((float)(rand() % 360)) * distanceFromCenter;
        sf::Vector2f randVel = VectorMath::getNormalized(VectorMath::getRotatedRAD(randPos, M_PI_2)) * std::powf(sun->getMass()*Planet::G/distanceFromCenter, 0.5f);
        planet->setPosition(randPos);
        planet->setVelocity(randVel);
        planet->setMass(mass);
        m_planetSystem->addChild(planet);
    }
}
void ExampleScene::setupScene_gravityAssist_accelerate()
{
    Planet* largePlanet1 = new Planet("largePlanet1");
    largePlanet1->setMass(1000);
    largePlanet1->setPosition(sf::Vector2f(0, 1100));
	largePlanet1->setVelocity(sf::Vector2f(0, -2));
    //largePlanet1->showPlots(true);
    m_planetSystem->addChild(largePlanet1);

    Planet* largePlanet2 = new Planet("largePlanet2");
    largePlanet2->setMass(1000);
    largePlanet2->setPosition(sf::Vector2f(900,-1000));
    largePlanet2->setVelocity(sf::Vector2f(-2, 0));
    m_planetSystem->addChild(largePlanet2);

    Planet* largePlanet3 = new Planet("largePlanet3");
    largePlanet3->setMass(1000);
    largePlanet3->setPosition(sf::Vector2f(-1200, -2100));
    largePlanet3->setVelocity(sf::Vector2f(0, 2));
    m_planetSystem->addChild(largePlanet3);

    Planet* largePlanet4 = new Planet("largePlanet4");
    largePlanet4->setMass(1000);
    largePlanet4->setPosition(sf::Vector2f(-3000, -0));
    largePlanet4->setVelocity(sf::Vector2f(1, 1));
    m_planetSystem->addChild(largePlanet4);
   

	Planet* spaceShip = new Planet("spaceShip");
	spaceShip->setMass(0.1);
	spaceShip->setPosition(sf::Vector2f(-m_worldSize / 2, 800));
	spaceShip->setVelocity(sf::Vector2f(3, 0));
    spaceShip->showPlots(true);
	m_planetSystem->addChild(spaceShip);

}
void ExampleScene::setupScene_gravityAssist_decelerate()
{
    Planet* largePlanet = new Planet();
    largePlanet->setMass(1000);
    largePlanet->setPosition(sf::Vector2f(0, 0));
    largePlanet->setVelocity(sf::Vector2f(0, 0));
    //largePlanet->showKineticEnergy(true);
    m_planetSystem->addChild(largePlanet);

    Planet* moon = new Planet();
    moon->setMass(10);
    moon->setPosition(sf::Vector2f(600, 0));
    moon->setVelocity(sf::Vector2f(0, -3));
    //moon->showKineticEnergy(true);
    m_planetSystem->addChild(moon);


    Planet* spaceShip = new Planet();
    spaceShip->setMass(0.001);
    spaceShip->setPosition(sf::Vector2f(-m_worldSize / 2-300, 650));
    spaceShip->setVelocity(sf::Vector2f(1, -1));
	spaceShip->showPlots(true);
    m_planetSystem->addChild(spaceShip);
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

