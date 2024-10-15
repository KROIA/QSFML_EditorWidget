#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

void ExampleScene::setupTutorial_2(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 1 started");


    // Instantiate a GameObject with the given name
    QSFML::Objects::GameObject* target = new QSFML::Objects::GameObject("Target");

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

    
    // Add the components to the object
    target->addComponent(shape);
   

    // Add an update function to the object
    target->addUpdateFunction([](GameObject& obj)
                              {
                                  // param obj is a reference to the object (replacement for the this pointer)
                                  obj.rotate(100 * obj.getDeltaT());
                              });
	target->setPosition(300, 300);


	GameObject* laserObj = new GameObject("Laser");
	QSFML::Utilities::Ray* ray = new QSFML::Utilities::Ray();
	ray->setPosition(0, 0);
    auto* rayPainter = ray->createRayPainter();
	laserObj->addComponent(rayPainter);
	
    //   Create a mouse follower component
    QSFML::Components::MouseFollower* mouseFollower = new QSFML::Components::MouseFollower();


    // Create a lamda function to handle the mouse move event
    QObject::connect(mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
        [ray](const sf::Vector2f& worldPos, const sf::Vector2i& pixelPos)
        {
	    	 ray->setDirection(worldPos);
        });
    laserObj->addComponent(mouseFollower);

	laserObj->addUpdateFunction([ray, shape](GameObject& obj)
	    {
            float distanceFactor = 0;
            size_t collidedEdgeIndex = 0;
            if (ray->raycast(*shape, distanceFactor, collidedEdgeIndex))
            {
                float distance = VectorMath::getLength(ray->getPosition() - ray->getPoint(distanceFactor));
	    		obj.logInfo("Collided with edge " + std::to_string(collidedEdgeIndex) + " at distance " + std::to_string(distance));
            }
	    });


	scene->addObject(target);
	scene->addObject(laserObj);
}