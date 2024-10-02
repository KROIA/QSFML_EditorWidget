#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "Planet.h"
#include "VectorField.h"

using namespace QSFML;
using namespace QSFML::Objects;

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    m_Scene = nullptr;
    setupScene();
    
    

    
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_Scene;
}

void ExampleScene::setupScene()
{
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    settings.timing.physicsFixedDeltaT = 1;

    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_Scene = new Scene(ui->SceneWidget, settings);

    float worldSize = 2000;

    DefaultEditor* defaultEditor = new DefaultEditor("Editor", sf::Vector2f(worldSize, worldSize));
    defaultEditor->setRenderLayer(RenderLayer::layer_0);
    defaultEditor->setPosition(-sf::Vector2f(worldSize*0.5f, worldSize*0.5f));
    defaultEditor->getCamera()->setMaxMovingBounds(sf::FloatRect(-worldSize * 0.5f, -worldSize * 0.5f, worldSize, worldSize));
    m_Scene->addObject(defaultEditor);
    sf::View view = m_Scene->getDefaultCamera()->getThisCameraView();
    view.setSize(worldSize, worldSize);
    view.setCenter(worldSize * 0.5f, worldSize * 0.5f);
    m_Scene->getDefaultCamera()->setThisCameraView(view);
    m_Scene->getDefaultCamera()->setPosition(0, 1);

    GameObject* planetSystem = new GameObject("PlanetSystem");
    VectorField *vectorField = new VectorField();
    planetSystem->addComponent(vectorField);
    // CreateVectorField

    Planet::setEnableCollision(false);
    Planet::setWorldBounds(sf::FloatRect(-worldSize * 0.5, -worldSize * 0.5, worldSize, worldSize));

    int spacing = 10;
   
    vectorField->m_field.reserve((worldSize / spacing)*(worldSize / spacing));
    for (int x = -worldSize * 0.5; x < worldSize*0.5; x += spacing)
    {
        for (int y = -worldSize * 0.5; y < worldSize*0.5; y += spacing)
        {
            VectorField::FieldElement element;
            element.position = sf::Vector2f(x, y);
            element.direction = sf::Vector2f(0, 0);
            vectorField->m_field.push_back(element);
        }
    }


    int positions = worldSize / 2;
    for (size_t i = 0; i < 10; ++i)
    {
        Planet* planet = new Planet();
        sf::Vector2f randPos = sf::Vector2f(rand() % positions, rand() % positions)-sf::Vector2f(positions *0.5, positions *0.5);
        sf::Vector2f randVel = VectorMath::getNormalized(VectorMath::getRotated(randPos, M_PI_2))*(float)(1);
		planet->setPosition(randPos);
        planet->setVelocity(randVel);
        planet->setMass((rand() % 100)+1);
		planetSystem->addChild(planet);
    }
    planetSystem->setUpdateFunction([planetSystem, vectorField]()
	{
        static bool first = true;
        if (first)
        {
            first = false;
            return;
        }
		const std::vector<Planet*> &planets = Planet::getPlanets();
        for (auto planet : planets)
		{
			planet->applyVelocity();
		}

        // Calculate vector Field
        for(auto & fieldElement : vectorField->m_field)
		{
            sf::Vector2f dir = Planet::calculateGravityPotential(fieldElement.position)*10.f;
            float magnitudeSqr = dir.x * dir.x + dir.y * dir.y;
            const static std::vector<sf::Color> colors = {sf::Color::Blue, sf::Color::Cyan, sf::Color::Green, sf::Color::Yellow, sf::Color(200,200,0), sf::Color::Red};
            fieldElement.color = Color::lerpCubic(colors, pow(magnitudeSqr, 0.3));
            if (magnitudeSqr > 30)
			{
				dir = VectorMath::getNormalized(dir) * 30.f;
			}
            else if (magnitudeSqr < 10)
            {
                dir = VectorMath::getNormalized(dir) * 8.f;
            }
			fieldElement.direction = dir;
		}
	});
    m_Scene->addObject(planetSystem);


    qDebug() << defaultEditor->toString().c_str();
    m_Scene->start();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_Scene)
        m_Scene->stop();
    event->accept();
}

