#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   This tutorial creates multiple green quare shapes that rotate around its center.
   A raycaster is used to cast a ray from the cyan object to the green object.
   The cyan laser object rotates in a sine wave pattern and casts a ray on to its front.
   The laser object is following the mouse cursor.

   What you will learn:
   - How to use the Ray class to cast rays on to shapes

*/

// Shapes to apply a raycast on
static std::vector<Components::Shape*> targetShapes;

static GameObject* createTarget(const sf::Vector2f &targetPos);
static GameObject* createLaserObject();

void ExampleScene::setupTutorial_3(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 3 started");

	scene->addObject(createTarget(sf::Vector2f(300,300)));
	scene->addObject(createTarget(sf::Vector2f(500,500)));
	scene->addObject(createTarget(sf::Vector2f(100,500)));
	scene->addObject(createLaserObject());
}


GameObject* createTarget(const sf::Vector2f& targetPos)
{
    // Instantiate a GameObject with the given name
    GameObject* target = new GameObject("Target");

    // Create Components
    //   Create a square shape
    Components::Shape *targetShape = new Components::Shape();
    targetShape->setPoints({
        {-50, -50},
        {50, -50},
        {50, 50},
        {-50, 50}
        });
    targetShape->setFillColor(sf::Color::Green);
    targetShape->setFill(true);
	targetShapes.push_back(targetShape);


    // Add the components to the object
    target->addComponent(targetShape);


    // Add an update function to the object
    target->addUpdateFunction([](GameObject& obj)
        {
            // param obj is a reference to the object (replacement for the this pointer)
            obj.rotate(100 * obj.getDeltaT());
        });
    target->setPosition(targetPos);

    return target;
}

GameObject* createLaserObject()
{


    GameObject* laserObj = new GameObject("Laser");

    Components::Shape* shape = new Components::Shape();
    shape->setPoints({
        {-50, -50},
        {50, -30},
        {50, 30},
        {-50, 50}
        });
    shape->setFillColor(sf::Color::Cyan);
    shape->setFill(true);
	laserObj->addComponent(shape);

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
        [laserObj](const sf::Vector2f& worldPos, const sf::Vector2i& pixelPos)
        {
            laserObj->setPosition(worldPos);
        });
    laserObj->addComponent(mouseFollower);

    laserObj->addUpdateFunction([ray](GameObject& obj)
        {
			obj.setRotation(std::sin(obj.getAge()*10) * 90);

			ray->setPosition(obj.getGlobalPosition());
            ray->setDirection(VectorMath::getRotatedUnitVector((float)(obj.getRotation())));

            float distanceFactor = 0;
            size_t shapeIndex = 0;
            size_t collidedEdgeIndex = 0;

            // Cast the ray on to the shape
            if (ray->raycast(targetShapes, distanceFactor, shapeIndex, collidedEdgeIndex))
            {
                // If we hit the shape, we can get the collision point
                sf::Vector2f collisionPoint = ray->getPoint(distanceFactor);
                float distance = VectorMath::getLength(ray->getPosition() - collisionPoint);
                obj.logInfo("Collided with edge: " + std::to_string(collidedEdgeIndex) +
                    " at distance: " + std::to_string(distance) +
                    " Collision point = {" + std::to_string(collisionPoint.x) + "," + std::to_string(collisionPoint.y) + "}");
            }
        });
    return laserObj;
}