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
	
    m_scene = nullptr;
    setupScene();
	Log::UI::NativeConsoleView::createStaticInstance();
	Log::UI::NativeConsoleView::getStaticInstance()->show();
    
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

void ExampleScene::setupScene()
{
    SceneSettings settings;
    settings.layout.fixedSize = sf::Vector2u(400, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.01;
    settings.timing.physicsFixedDeltaT = 1;
	settings.timing.physicsDeltaTScale = 2;

    m_scene = new Scene(ui->SceneWidget, settings);

    float worldSize = 2000;

    DefaultEditor* defaultEditor = new DefaultEditor("Editor", sf::FloatRect(-worldSize/2, -worldSize/2, worldSize, worldSize));
    defaultEditor->setRenderLayer(RenderLayer::layer_0);
    m_scene->addObject(defaultEditor);

	// The planet system object updates the movement of the planets
    GameObject* planetSystem = new GameObject("PlanetSystem");
    QSFML::Components::VectorFieldPainter *vectorField = new QSFML::Components::VectorFieldPainter();
    planetSystem->addComponent(vectorField);

    Planet::setEnableCollision(false);
    //Planet::setWorldBounds(sf::FloatRect(-worldSize * 0.5, -worldSize * 0.5, worldSize, worldSize));

    int spacing = 10;
   
    // Building the vector field buffer
    QSFML::vector<QSFML::Components::VectorFieldPainter::Element> field;
    field.reserve((worldSize / spacing)*(worldSize / spacing));
    for (int x = -worldSize * 0.5; x < worldSize*0.5; x += spacing)
    {
        for (int y = -worldSize * 0.5; y < worldSize*0.5; y += spacing)
        {
            QSFML::Components::VectorFieldPainter::Element element;
            element.position = sf::Vector2f(x, y);
            element.direction = sf::Vector2f(10, 5);
            field.push_back(element);
        }
    }
    vectorField->setField(field);

	// Create planets with a random position and velocity
    int positionRange = worldSize / 2;
	unsigned int numPlanets = 5;
    Planet* sun = new Planet();
    sun->setMass(1000);
    planetSystem->addChild(sun);
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
        planetSystem->addChild(planet);
    }

	// The update function is called every frame and calculates the vector field
    planetSystem->addUpdateFunction([planetSystem, vectorField](GameObject &)
	{
        // The dynamics for each planet is already calculated in each planets update function a
		// and just has to be applied here. This ensures that the planets movement is in sync with each other
		const QSFML::vector<Planet*> &planets = Planet::getPlanets();
        for (auto planet : planets)
		{
			planet->applyVelocity();
		}

        // Calculate vector Field
        for(auto & fieldElement : *vectorField)
		{
            sf::Vector2f dir = Planet::calculateForce(fieldElement.position)*10.f;
            float magnitudeSqr = dir.x * dir.x + dir.y * dir.y;
            const static QSFML::vector<sf::Color> colors = {sf::Color::Blue, sf::Color::Cyan, sf::Color::Green, sf::Color::Yellow, sf::Color(200,200,0), sf::Color::Red};
            fieldElement.color = Color::lerpCubic(colors, pow(magnitudeSqr, 0.3));
            dir = VectorMath::getNormalized(dir)*10.f;
			fieldElement.direction = dir;
		}
	});

    planetSystem->addDrawFunction(
        [planetSystem, worldSize](const GameObject&, sf::RenderTarget& target, sf::RenderStates states)
        {
            //Draw contour line
            const size_t numLines = 10;
            const size_t vertexCount = 1000;
            const float stepsize = 10;


            sf::VertexArray contourLines(sf::Lines);
            contourLines.resize(vertexCount);

            float startX = -worldSize/2;
			float xIncrement = worldSize / numLines;
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
					color.a = 255 - (i*255 / vertexCount);
                }
                target.draw(contourLines, states);
            }

        });

    m_scene->addObject(planetSystem);

    // Define in which order the gameObject updates
    planetSystem->setUpdateOrder(
        {
            QSFML::Objects::GameObject::UpdateSequenceElement::childs,
            QSFML::Objects::GameObject::UpdateSequenceElement::components,
			QSFML::Objects::GameObject::UpdateSequenceElement::customUpdateFunctions
        });


    planetSystem->updateObjectChanges();
    qDebug() << planetSystem->toString().c_str();
    m_scene->start();
}


/*
void ExampleScene::onScreenCapture()
{
    static size_t counter = 0;

	if (counter >= 100)
	{
		return;
	}
    
	if (m_threadData.size() == 0)
	{
        size_t threadCount = 6;
		m_threadData.resize(threadCount);
        for (size_t i = 0; i < threadCount; ++i)
        {
			m_threadData[i].thread = new std::thread([this, i]()
			{
				 ThreadData& data = m_threadData[i];
				 while (true)
				 {
					 std::unique_lock<std::mutex> lock(data.mutex);
					 data.condition.wait(lock, [this, &data] {return !data.images.empty(); });

					 QSFML::vector<ThreadData::ImageData> images = data.images;
					 data.images.clear();
                     data.images.reserve(10);
					 lock.unlock();
					 for (auto image : images)
					 {
						 image.image->saveToFile("screenshots/" + std::to_string(image.index) + ".jpg");
                         delete image.image;
					 }
					// data.log.logInfo("Saved " + std::to_string(images.size()) + " images");
				 }
			});
            m_threadData[i].log.setParentID(m_scene->getSceneLogger().getID());
			m_threadData[i].log.setName("Thread " + std::to_string(i));
        }
	}

	size_t currentIndex = counter % m_threadData.size();
	ThreadData& data = m_threadData[currentIndex];

	sf::Image* image = new sf::Image();
	m_scene->captureScreen(*image);
    //delete image;
    //return;
    {
		std::lock_guard<std::mutex> lock(data.mutex);
		data.images.push_back({ image, counter });
        data.condition.notify_one();
    }
    ++counter;
}
*/
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

