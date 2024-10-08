#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>

using namespace QSFML;
using namespace QSFML::Objects;

void imageComponent(GameObjectPtr obj);
void linePainterComponent(GameObjectPtr obj);
void pathPainterComponent(GameObjectPtr obj);
void pixelPainterComponent(GameObjectPtr obj);
void pointPainterComponent(GameObjectPtr obj);
void rectPainterComponent(GameObjectPtr obj);
void shapeComponent(GameObjectPtr obj);
void textComponent(GameObjectPtr obj);


ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    m_Scene = nullptr;
    setupScene();
    
	GameObjectPtr obj = new GameObject();

    // Drawable components
    imageComponent(obj);
    //linePainterComponent(obj);
    //pathPainterComponent(obj);
    //pixelPainterComponent(obj);
    //pointPainterComponent(obj);
    //rectPainterComponent(obj);
    //shapeComponent(obj);
    //textComponent(obj);


    obj->setPosition(m_Scene->getViewCenterPosition());
    m_Scene->addObject(obj);    
    m_Scene->applyObjectChanges();
    qDebug() << obj->toString().c_str();
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
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_Scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_Scene->addObject(defaultEditor);
    m_Scene->start();
    qDebug() << defaultEditor->toString().c_str();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_Scene)
        m_Scene->stop();
    event->accept();
}

void imageComponent(GameObjectPtr obj)
{
	QSFML::Components::Image* image = new QSFML::Components::Image();
    image->loadFromFile("../Examples/Example_components/resources/player.png");
    image->setScale(2,2);
    obj->addComponent(image);
}
void linePainterComponent(GameObjectPtr obj)
{
    QSFML::Components::LinePainter* linePainter = new QSFML::Components::LinePainter();
	linePainter->addLine(sf::Vector2f(0, 0), sf::Vector2f(100, 100));
    linePainter->addLine(sf::Vector2f(50, 100), sf::Vector2f(200, 0));

	linePainter->setThickness(5);
	linePainter->setColor(sf::Color::Red);
	obj->addComponent(linePainter);
}
void pathPainterComponent(GameObjectPtr obj)
{
    QSFML::Components::PathPainter* pathPainter = new QSFML::Components::PathPainter();

	pathPainter->setColor(sf::Color::Red);
	obj->addComponent(pathPainter);
    obj->addUpdateFunction([pathPainter](GameObject&)
        {
            sf::Vector2f lastPoint;
            float lastDirAngle = 0;
            if (pathPainter->getPointCount() > 1)
            {
                lastPoint = pathPainter->getPoint(pathPainter->getPointCount() - 1);
                sf::Vector2f dir = lastPoint - pathPainter->getPoint(pathPainter->getPointCount() - 2);

                lastDirAngle = QSFML::VectorMath::getAngle(dir);
            }

            lastDirAngle += (rand() % (int)(M_PI * 25)-(M_PI * 12.5)) / 100.f;
            pathPainter->appenPoint(lastPoint + QSFML::VectorMath::getRotatedUnitVector(lastDirAngle)*10.f);
            if (pathPainter->getPointCount() > 1000)
                pathPainter->popPointAtStart(1);

        });
}
void pixelPainterComponent(GameObjectPtr obj)
{
    QSFML::Components::PixelPainter* pixelPainter = new QSFML::Components::PixelPainter();
    pixelPainter->setPixelCount(sf::Vector2u(10, 10));
    pixelPainter->setPixelSize(10);


	obj->addComponent(pixelPainter);
	obj->addUpdateFunction([pixelPainter, obj](GameObject&)
		{
            sf::Vector2u pos = sf::Vector2u(rand() % pixelPainter->getPixelCount().x, rand() % pixelPainter->getPixelCount().y);
			pixelPainter->setPixel(pos, sf::Color(rand() % 255, rand() % 255, rand() % 255));
		});

}
void pointPainterComponent(GameObjectPtr obj)
{
    QSFML::Components::PointPainter* pointPainter = new QSFML::Components::PointPainter();
    pointPainter->setRadius(5);
    pointPainter->setColor(sf::Color::Red);
    obj->addComponent(pointPainter);
    obj->addUpdateFunction([pointPainter](GameObject&)
        {
            pointPainter->addPoint(sf::Vector2f(rand() % 1000, rand() % 1000)- sf::Vector2f(500,500));
        });

}
void rectPainterComponent(GameObjectPtr obj)
{
	QSFML::Components::RectPainter* rectPainter = new QSFML::Components::RectPainter();
	rectPainter->setRect(QSFML::Utilities::AABB(0, 0, 100, 100));
	rectPainter->setFillColor(sf::Color::Red);

	obj->addComponent(rectPainter);
	obj->addUpdateFunction([rectPainter](GameObject&)
						   {
                               static QSFML::Utilities::AABB lastRect(rand() % 1000, rand() % 1000, rand() % 500, rand() % 500);
							   static QSFML::Utilities::AABB targetRect(rand() % 1000, rand() % 1000, rand() % 500, rand() % 500);
							   static float t = 0;
                               static sf::Color lastColor = sf::Color::Red;
                               static sf::Color targetColor = sf::Color::Blue;
							   t += 0.01;
							   if (t > 1)
							   {
								   t = 0;
								   lastRect = targetRect;
                                   lastColor = targetColor;
								   targetRect = QSFML::Utilities::AABB(rand() % 1000-500, rand() % 1000-500, rand() % 500, rand() % 500);
                                   targetColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
							   }
							   QSFML::Utilities::AABB newRect = QSFML::Utilities::AABB::lerp(lastRect, targetRect, t);
							   sf::Color newColor = QSFML::Color::lerpLinear(lastColor, targetColor, t);
							   rectPainter->setRect(newRect);
							   rectPainter->setFillColor(newColor);
						   });

}
void shapeComponent(GameObjectPtr obj)
{
    QSFML::Components::Shape* shape = new QSFML::Components::Shape();
    shape->setFillColor(sf::Color::Green);
    shape->setOutlineColor(sf::Color::Blue);
	shape->setOutline(true);
	shape->setFill(true);
    shape->setPointCount(3);
    shape->setPoint(0, sf::Vector2f(0, 0));
    shape->setPoint(1, sf::Vector2f(100, 0));
    shape->setPoint(2, sf::Vector2f(50, 100));

    QSFML::Components::RectPainter* boundingBox = new QSFML::Components::RectPainter();
    boundingBox->setOutlineColor(sf::Color::Red);
    boundingBox->setOutlineThickness(3);
    obj->addComponent(shape);
    obj->addComponent(boundingBox);
    obj->addUpdateFunction([shape, boundingBox](GameObject&)
                           {
                               static float t = 0;
							   static std::vector<sf::Vector2f> lastPoints = shape->getPoints();
							   static std::vector<sf::Vector2f> targetPoints = shape->getPoints();
                               t += 0.01;
                               if (t > 1)
                               {
                                   t = 0;
								   lastPoints = targetPoints;
								   for (auto& point : targetPoints)
								   {
									   point = sf::Vector2f(rand() % 1000 - 500, rand() % 1000 - 500);
								   }
                               }
                               
							   std::vector<sf::Vector2f> newPoints;
							   newPoints.reserve(lastPoints.size());
							   for (size_t i = 0; i < lastPoints.size(); i++)
							   {
								   newPoints.push_back(QSFML::VectorMath::lerp(lastPoints[i], targetPoints[i], t));
							   }
                               shape->setPoints(newPoints);
                               auto aabb = shape->getLocalBounds();
                               boundingBox->setRect(aabb);

                           });

}

void textComponent(GameObjectPtr obj)
{
    QSFML::Components::Text* text = new QSFML::Components::Text();
    text->setText("Hello");
    text->setScale(1);
    text->setOrigin(QSFML::Utilities::Origin::Center);
    obj->addComponent(text);
    obj->addUpdateFunction([text](GameObject&)
                           {
                               static float rotation = 0;
                               rotation += 0.01;
                               text->setRotation(rotation);
                           });
}