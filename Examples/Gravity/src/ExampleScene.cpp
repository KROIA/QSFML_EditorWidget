#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "Planet.h"
#include <QDir>
#include <fstream>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
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
    // Create a timer to capture the screen
    //QTimer* timer = new QTimer(this);
    // Create the directory for the screenshots
    //QDir().mkdir("screenshots");
	//connect(timer, &QTimer::timeout, this, &ExampleScene::onScreenCapture, Qt::DirectConnection);
    //timer->start(50);
	Utilities::CameraRecorder* recorder = new Utilities::CameraRecorder(m_scene->getDefaultCamera(),4);
	QTimer* timer = new QTimer(this);
	timer->singleShot(1000, [recorder]()
		{
			recorder->startCapture(1000, 0.01, "screenshots/gravity");
			//recorder->startCapture(100, 0.01, "screenshots");
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
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(400, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.00;
    settings.timing.physicsFixedDeltaT = 1;
	settings.timing.physicsDeltaTScale = 2;

    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_scene = new Scene(ui->SceneWidget, settings);

    float worldSize = 2000;

    DefaultEditor* defaultEditor = new DefaultEditor("Editor", sf::Vector2f(worldSize, worldSize));
    defaultEditor->setRenderLayer(RenderLayer::layer_0);
    defaultEditor->setPosition(-sf::Vector2f(worldSize*0.5f, worldSize*0.5f));
    defaultEditor->getCamera()->setMaxMovingBounds(sf::FloatRect(-worldSize * 0.5f, -worldSize * 0.5f, worldSize, worldSize));
    m_scene->addObject(defaultEditor);
    sf::View view = m_scene->getDefaultCamera()->getThisCameraView();
    view.setSize(worldSize, worldSize);
    view.setCenter(worldSize * 0.5f, worldSize * 0.5f);
    m_scene->getDefaultCamera()->setThisCameraView(view);
    m_scene->getDefaultCamera()->setPosition(0, 1);

    GameObject* planetSystem = new GameObject("PlanetSystem");
    QSFML::Components::VectorFieldPainter *vectorField = new QSFML::Components::VectorFieldPainter();
    planetSystem->addComponent(vectorField);
    // CreateVectorField

    Planet::setEnableCollision(false);
    Planet::setWorldBounds(sf::FloatRect(-worldSize * 0.5, -worldSize * 0.5, worldSize, worldSize));

    int spacing = 10;
   
    std::vector<QSFML::Components::VectorFieldPainter::Element> field;
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


    int positions = worldSize / 2;
    for (size_t i = 0; i < 10; ++i)
    {
        Planet* planet = new Planet();
        sf::Vector2f randPos = sf::Vector2f(rand() % positions, rand() % positions)-sf::Vector2f(positions *0.5, positions *0.5);
        sf::Vector2f randVel = VectorMath::getNormalized(VectorMath::getRotatedRAD(randPos, M_PI_2))*(float)(1);
		planet->setPosition(randPos);
        planet->setVelocity(randVel);
        planet->setMass((rand() % 100)+1);
		planetSystem->addChild(planet);
    }
    planetSystem->addUpdateFunction([planetSystem, vectorField](GameObject &)
	{
        /*static bool first = true;
        if (first)
        {
            first = false;
            return;
        }*/
		const std::vector<Planet*> &planets = Planet::getPlanets();
        for (auto planet : planets)
		{
			planet->applyVelocity();
		}

        // Calculate vector Field
        for(auto & fieldElement : *vectorField)
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
    m_scene->addObject(planetSystem);
    planetSystem->setUpdateOrder(
        {
            QSFML::Objects::GameObject::UpdateSequenceElement::childs,
            QSFML::Objects::GameObject::UpdateSequenceElement::components,
			QSFML::Objects::GameObject::UpdateSequenceElement::customUpdateFunctions
        });


    qDebug() << defaultEditor->toString().c_str();
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

					 std::vector<ThreadData::ImageData> images = data.images;
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

