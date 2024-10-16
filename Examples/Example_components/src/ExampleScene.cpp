#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>

using namespace QSFML;
using namespace QSFML::Objects;



ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    m_scene = nullptr;
    setupScene();


    if (m_objects.size() > 0)
        m_objects[0].checkBox->setChecked(true);
    // Add a vertical spacer to the list view
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->scrollAreaWidgetContents->layout()->addItem(verticalSpacer);
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_scene;
}

void ExampleScene::setupScene()
{
    SceneSettings settings;
    settings.layout.fixedSize = sf::Vector2u(800, 500);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    m_scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);
    m_scene->start();
    qDebug() << defaultEditor->toString().c_str();

    // Create Objects
    imageComponent();
    linePainterComponent();
    pathPainterComponent();
    pixelPainterComponent();
    pointPainterComponent();
    rectPainterComponent();
    shapeComponent();
    textComponent();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

void ExampleScene::addNewObjectToScene(QSFML::Objects::GameObjectPtr obj)
{
	ObjectData data;
	data.obj = obj;
	data.checkBox = new QCheckBox(obj->getName().c_str());
	ui->scrollAreaWidgetContents->layout()->addWidget(data.checkBox);

	connect(data.checkBox, &QCheckBox::stateChanged, [this, data](int state)
		{
			if (state == Qt::Checked)
			{
				data.obj->setEnabled(true);
			}
			else
			{
                data.obj->setEnabled(false);
			}
		});
	data.checkBox->setChecked(false);
	obj->setEnabled(false);
	m_scene->addObject(obj);
	m_objects.push_back(data);
}

void ExampleScene::imageComponent()
{
    GameObjectPtr obj = new GameObject("Image Component Obj");
	QSFML::Components::Image* image = new QSFML::Components::Image();
    image->loadFromFile("../Examples/Example_components/resources/player.png");
    image->setScale(2,2);
    obj->addComponent(image);
    addNewObjectToScene(obj);
}
void ExampleScene::linePainterComponent()
{
	GameObjectPtr obj = new GameObject("Line Painter Component Obj");
    QSFML::Components::LinePainter* linePainter = new QSFML::Components::LinePainter();
	linePainter->addLine(sf::Vector2f(0, 0), sf::Vector2f(50, 50));
    linePainter->addLine(sf::Vector2f(20, 80), sf::Vector2f(100, 0));

	linePainter->setThickness(5);
	linePainter->setColor(sf::Color::Red);
	obj->addComponent(linePainter);
    addNewObjectToScene(obj);
}
void ExampleScene::pathPainterComponent()
{
	GameObjectPtr obj = new GameObject("Path Painter Component Obj");
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

                lastDirAngle = QSFML::VectorMath::getAngleRAD(dir);
            }

            lastDirAngle += (rand() % (int)(M_PI * 25)-(M_PI * 12.5)) / 100.f;
            pathPainter->appenPoint(lastPoint + QSFML::VectorMath::getRotatedUnitVectorRAD(lastDirAngle)*10.f);
            if (pathPainter->getPointCount() > 1000)
                pathPainter->popPointAtStart(1);

        });
    addNewObjectToScene(obj);
}
void ExampleScene::pixelPainterComponent()
{
	GameObjectPtr obj = new GameObject("Pixel Painter Component Obj");
    QSFML::Components::PixelPainter* pixelPainter = new QSFML::Components::PixelPainter();
    pixelPainter->setPixelCount(sf::Vector2u(10, 10));
    pixelPainter->setPixelSize(10);


	obj->addComponent(pixelPainter);
	obj->addUpdateFunction([pixelPainter, obj](GameObject&)
		{
            sf::Vector2u pos = sf::Vector2u(rand() % pixelPainter->getPixelCount().x, rand() % pixelPainter->getPixelCount().y);
			pixelPainter->setPixel(pos, sf::Color(rand() % 255, rand() % 255, rand() % 255));
		});
    addNewObjectToScene(obj);
}
void ExampleScene::pointPainterComponent()
{
	GameObjectPtr obj = new GameObject("Point Painter Component Obj");
    QSFML::Components::PointPainter* pointPainter = new QSFML::Components::PointPainter();
    pointPainter->setRadius(5);
    pointPainter->setColor(sf::Color::Red);
    obj->addComponent(pointPainter);
    obj->addUpdateFunction([pointPainter](GameObject&)
        {
            pointPainter->addPoint(sf::Vector2f(rand() % 1000, rand() % 1000)- sf::Vector2f(500,500));
        });
    addNewObjectToScene(obj);
}
void ExampleScene::rectPainterComponent()
{
	GameObjectPtr obj = new GameObject("Rect Painter Component Obj");
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
    addNewObjectToScene(obj);
}
void ExampleScene::shapeComponent()
{
	GameObjectPtr obj = new GameObject("Shape Component Obj");
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
							   static QSFML::vector<sf::Vector2f> lastPoints = shape->getPoints();
							   static QSFML::vector<sf::Vector2f> targetPoints = shape->getPoints();
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
                               
							   QSFML::vector<sf::Vector2f> newPoints;
							   newPoints.reserve(lastPoints.size());
							   for (size_t i = 0; i < lastPoints.size(); i++)
							   {
								   newPoints.push_back(QSFML::VectorMath::lerp(lastPoints[i], targetPoints[i], t));
							   }
                               shape->setPoints(newPoints);
                               auto aabb = shape->getLocalBounds();
                               boundingBox->setRect(aabb);

                           });
    addNewObjectToScene(obj);
}

void ExampleScene::textComponent()
{
	GameObjectPtr obj = new GameObject("Text Component Obj");
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
    addNewObjectToScene(obj);
}