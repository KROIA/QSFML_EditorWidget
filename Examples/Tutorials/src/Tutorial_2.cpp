#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   This tutorial creates a green quare shape that rotates around its center.
   A raycaster is used to cast a ray from the origin {0, 0} in to the direction of the mouse.
   The ray is used to detect collisions with the square shape.

   What you will learn:
   - How to use the Ray class to cast rays on to shapes

*/

void ExampleScene::setupTutorial_2(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 2 started");


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

	// The Ray is not a component, it is just a math construct to enable raycasting
    // We create the ray on the heap and not considering to delete it. 
	// This is okey for this example, but in a real application you should delete the ray object when you are done with it.
    // To manage such objects better, create a derived class from GameObject for that object that makes raycasting.
	// Then use the rays as members of that class.
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

			// Cast the ray on to the shape
            if (ray->raycast(*shape, distanceFactor, collidedEdgeIndex))
            {
				// If we hit the shape, we can get the collision point
				sf::Vector2f collisionPoint = ray->getPoint(distanceFactor);
                float distance = VectorMath::getLength(ray->getPosition() - collisionPoint);
	    		obj.logInfo("Collided with edge: " + std::to_string(collidedEdgeIndex) + 
                    " at distance: " + std::to_string(distance) + 
                    " Collision point = {"+std::to_string(collisionPoint.x)+","+std::to_string(collisionPoint.y)+"}");
            }
	    });


	scene->addObject(target);
	scene->addObject(laserObj);
}