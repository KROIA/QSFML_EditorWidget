#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

void ExampleScene::setupTutorial_1(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 1 started");


    // Instantiate a GameObject with the given name
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");

    // Create Components
    //   Create a square shape
    QSFML::Components::Shape* shape = new QSFML::Components::Shape();
    shape->setPoints({
        {-50, -50},
        {50, -50},
        {50, 50},
        {-50, 50}
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
                                 //obj.logInfo("Events: " + std::to_string(events.size()));
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


	scene->addObject(object);
}