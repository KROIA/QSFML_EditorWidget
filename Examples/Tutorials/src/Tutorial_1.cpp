#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   This tutorial creates a green quare shape that can be moved by the mouse.
   It automatically rotates around its center.

   What you will learn:
   - How to create a GameObject
   - How to create a Shape component
   - How to create a MouseFollower component
   - How to add an update function to a GameObject

*/
void ExampleScene::setupTutorial_1(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 1 started");


	// Instantiate a GameObject with the name "MyObject"
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");

    // Create Components
    //   Create a square shape
    QSFML::Components::Shape* shape = new QSFML::Components::Shape();

	// Define the vertices of the square shape
    shape->setPoints({
        {-50, -50},
        {50, -50},
        {50, 50},
        {-50, 50}
                     });
	shape->setFillColor(sf::Color::Green);  // Set the fill color to green
	shape->setFill(true);  // Fill the shape

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

	scene->addObject(object);
}