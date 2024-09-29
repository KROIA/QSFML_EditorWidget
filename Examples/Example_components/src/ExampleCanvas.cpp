#include "ExampleCanvas.h"
#include "ui_exampleCanvas.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>

using namespace QSFML;
using namespace QSFML::Objects;

void imageComponent(CanvasObject* obj);
void linePainterComponent(CanvasObject* obj);
void pathPainterComponent(CanvasObject* obj);
void pixelPainterComponent(CanvasObject* obj);
void pointPainterComponent(CanvasObject* obj);


ExampleCanvas::ExampleCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleCanvas)
{
    ui->setupUi(this);
    m_canvas = nullptr;
    setupCanvas();
    
	CanvasObject* obj = new CanvasObject();

    // Drawable components
    //imageComponent(obj);
    //linePainterComponent(obj);
    //pathPainterComponent(obj);
    //pixelPainterComponent(obj);
    pointPainterComponent(obj);

    obj->setPosition(m_canvas->getViewCenterPosition());
    m_canvas->addObject(obj);    
    m_canvas->applyObjectChanges();
    qDebug() << obj->toString().c_str();
}

ExampleCanvas::~ExampleCanvas()
{
    delete ui;
    delete m_canvas;
}

void ExampleCanvas::setupCanvas()
{
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_canvas->addObject(defaultEditor);
    qDebug() << defaultEditor->toString().c_str();
}
void ExampleCanvas::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

void imageComponent(CanvasObject* obj)
{
	QSFML::Components::Image* image = new QSFML::Components::Image();
    image->loadFromFile("../Examples/Example_components/resources/player.png");
    image->setScale(2,2);
    obj->addComponent(image);
}
void linePainterComponent(CanvasObject* obj)
{
    QSFML::Components::LinePainter* linePainter = new QSFML::Components::LinePainter();
	linePainter->addLine(sf::Vector2f(0, 0), sf::Vector2f(100, 100));
    linePainter->addLine(sf::Vector2f(50, 100), sf::Vector2f(200, 0));

	linePainter->setThickness(5);
	linePainter->setColor(sf::Color::Red);
	obj->addComponent(linePainter);
}
void pathPainterComponent(CanvasObject* obj)
{
    QSFML::Components::PathPainter* pathPainter = new QSFML::Components::PathPainter();

	pathPainter->setColor(sf::Color::Red);
	obj->addComponent(pathPainter);
    obj->setUpdateFunction([pathPainter]()
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
void pixelPainterComponent(CanvasObject* obj)
{
    QSFML::Components::PixelPainter* pixelPainter = new QSFML::Components::PixelPainter();
    pixelPainter->setPixelCount(sf::Vector2u(10, 10));
    pixelPainter->setPixelSize(10);


	obj->addComponent(pixelPainter);
	obj->setUpdateFunction([pixelPainter, obj]()
		{
            sf::Vector2u pos = sf::Vector2u(rand() % pixelPainter->getPixelCount().x, rand() % pixelPainter->getPixelCount().y);
			pixelPainter->setPixel(pos, sf::Color(rand() % 255, rand() % 255, rand() % 255));
		});

}
void pointPainterComponent(CanvasObject* obj)
{
    QSFML::Components::PointPainter* pointPainter = new QSFML::Components::PointPainter();
    pointPainter->setRadius(5);
    pointPainter->setColor(sf::Color::Red);
    obj->addComponent(pointPainter);
    obj->setUpdateFunction([pointPainter]()
        {
            pointPainter->addPoint(sf::Vector2f(rand() % 1000, rand() % 1000)- sf::Vector2f(500,500));
        });

}