#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDir>

#include "CustomObject.h"
#include "MinimalObject.h"

using namespace QSFML;
using namespace QSFML::Objects;

//#define ENABLE_SCREEN_CAPTURE

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    m_scene = nullptr;

	// Create a logger console
	Log::UI::NativeConsoleView::createStaticInstance();
    Log::UI::NativeConsoleView::getStaticInstance()->show();
    setupScene();

	
#ifdef ENABLE_SCREEN_CAPTURE
    // Create a timer to capture the screen
	QTimer* timer = new QTimer(this);
	// Create the directory for the screenshots
	QDir().mkdir("screenshots");
	connect(timer, &QTimer::timeout, [this]()
		{
			// Take 100 screenshots and save them to the screenshots directory
            static int counter = 0;
            if (counter > 100)
            {
                return;
            }
			sf::Image img = m_scene->captureScreen();
			img.saveToFile("screenshots\\"+std::to_string(counter) + ".png");
			++counter;
		});
    timer->start(1);
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
    settings.contextSettings.antialiasingLevel = 8;
	settings.timing.frameTime = 0.01; // Time in seconds
    m_scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);

    // Create a custom object and add it to the Scene
    // m_scene->addObject(new MinimalObject());

    // m_scene->addObject(new CustomObject());

    m_scene->addObject(customizedObjectFactory("Customized Object"));


	// Apply the changes to the scene to be able to print the objects tree now
	m_scene->applyObjectChanges();
    m_scene->getSceneLogger().logInfo("\n"+m_scene->getObjectsTreeString());
	m_scene->start();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}


QSFML::Objects::GameObject* ExampleScene::customizedObjectFactory(const std::string& name)
{
    // Instantiate a GameObject with the given name
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject(name);

    // Create Components
	//   Create a square shape
    QSFML::Components::Shape* shape = new QSFML::Components::Shape();
    shape->setPoints({
        {-50, -50},
        {50, -50},
        {50, 50},
        {-50, 50},
        {-50, -50}
        });
    shape->setFillColor(sf::Color::Green);
    shape->setFill(true);

    //   Create a mouse follower component
    QSFML::Components::MouseFollower* mouseFollower = new QSFML::Components::MouseFollower();

   
	// Create a lamda function to handle the mouse move event
	QObject::connect(mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged, 
                     [object](const sf::Vector2f& worldPos, const sf::Vector2i& pixelPos) 
        {
			// Set the position of this object to the mouse position
		    object->setPosition(worldPos);
		});

	// Add the components to the object
	object->addComponent(shape);
	object->addComponent(mouseFollower);

	// Add an update function to the object
    object->addUpdateFunction([](GameObject& obj)
        {
			// param obj is a reference to the object (replacement for the this pointer)
            obj.rotate(1);
        });

	// Add an event function to the object
    object->addEventFunction([](GameObject& obj, const std::unordered_map<CameraWindow*, std::vector<sf::Event>>& events)
        {
            // Custom event handler
            obj.logInfo("Events: " + std::to_string(events.size()));
        });
    
	// Add a draw function to the object
	object->addDrawFunction([](const GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
		{
			// Custom draw function
			sf::CircleShape circle(25);
			circle.setFillColor(sf::Color::Red);
            circle.setOrigin(25, 25);
			circle.setPosition(50, 0);
			target.draw(circle, states);
		});

	// Set the draw order of the object
    object->setDrawOrder({
        QSFML::Objects::GameObject::DrawSequenceElement::customDrawFunctions,
        QSFML::Objects::GameObject::DrawSequenceElement::components,
        QSFML::Objects::GameObject::DrawSequenceElement::childs
        });

	return object;
}