#include "sandbox.h"
#include "ui_sandbox.h"

#include "DrawableVector.h"
#include "AABBDisplayer.h"
#include "SandboxObject.h"
#include "MouseCollider.h"
#include <iostream>

#include <QTimer>
#include <QCloseEvent>

using namespace QSFML;
using namespace QSFML::Objects;

SandBox::SandBox(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SandBox)
{
    ui->setupUi(this);

    m_canvas_1 = nullptr;
    m_canvas_2 = nullptr;
    
    {
        CanvasSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        settings.timing.frameTime = 0;
        //settings.updateControlls.enableMultithreading = false;
        //settings.updateControlls.enablePaintLoop = false;
        //settings.updateControlls.enableEventLoop = false;
        //settings.updateControlls.enableUpdateLoop = false;
        m_canvas_1 = new Canvas(ui->canvasWidget_1,settings);

        DefaultEditor *defaultEditor = new DefaultEditor();

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        m_canvas_1->addObject(defaultEditor);
        m_canvas_1->addObject(m_vecDisplay);
        //m_canvas->addObject(grid);
        qDebug() << defaultEditor->toString().c_str();
        qDebug() << m_vecDisplay->toString().c_str();

        SandboxObject *sbObj = new SandboxObject();
        m_canvas_1->addObject(sbObj);

        m_pointPainter = new QSFML::Components::PointPainter();
        QSFML::Objects::CanvasObject* canvasObject = new QSFML::Objects::CanvasObject();
        canvasObject->addComponent(m_pointPainter);
        m_canvas_1->addObject(canvasObject);

        QTimer* timer = new QTimer(this);

        connect(timer, &QTimer::timeout, this, &SandBox::onTimerFinished);
        timer->start(1000);
    }
    
    {

        CanvasSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        //settings.timing.frameTime = 0;
       //settings.updateControlls.enableMultithreading = false;
       //settings.updateControlls.enablePaintLoop = false;
       //settings.updateControlls.enableEventLoop = false;
       //settings.updateControlls.enableUpdateLoop = false;
        m_canvas_2 = new Canvas(ui->canvasWidget_2,settings);

       DefaultEditor *defaultEditor = new DefaultEditor();
       defaultEditor->setRenderLayer(RenderLayer::layer_1);

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        //m_canvas_2->addObject(obj);
        //m_canvas_2->addObject(m_vecDisplay);
        m_canvas_2->addObject(defaultEditor);



        LineChart *m_chart = new LineChart();
        m_chart->setDataPoints({0,1,-1,0.5,-0.5,0});
        m_chart->setOrigin(sf::Vector2f(50,50));
        m_chart->setSize(sf::Vector2f(200,100));
        m_canvas_2->addObject(m_chart);

        
        MouseCollider* mouseCollider = new MouseCollider();
        m_canvas_2->addObject(mouseCollider);



        QSFML::Objects::CanvasObject* obj = new QSFML::Objects::CanvasObject();
        QSFML::Components::Shape *shape = new QSFML::Components::Shape();
        QSFML::Utilities::Ray *testRay = new QSFML::Utilities::Ray(sf::Vector2f(0, 0), sf::Vector2f(1, 1));
        obj->setPositionAbsolute(sf::Vector2f(100, 100));
        //delete testRay->createRayPainter();

        obj->addComponent(testRay->createRayPainter());
        

        shape->setPoints(
            {
                sf::Vector2f(0,0),
                sf::Vector2f(100,0),
                sf::Vector2f(100,100),
                sf::Vector2f(0,100)
            });

        sf::Transform t = shape->getTransform();
        t.translate(obj->getPositionAbsolute());
        shape->setTransform(t);
        obj->setUpdateFunction([shape, testRay, obj]()
		{
			sf::Transform t = shape->getTransform();
            t.rotate(0.1);
            shape->setTransform(t);


            testRay->setDirection(obj->getMouseWorldPosition() - testRay->getPos());
            testRay->normalize();
            float d1;
            size_t d2;
            testRay->raycast(*shape, d1, d2);
		});
        obj->setRenderLayer(RenderLayer::layer_2);
        shape->setFillColor(sf::Color::Red);
        shape->setOutlineColor(sf::Color::Blue);
        //shape->setOutlineThickness(5);
        
        shape->setFill(true);
        
        obj->addComponent(shape);
        m_canvas_2->addObject(obj);
       
    }

    QSFML::Utilities::Ray func1(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
    sf::Vector2f point = sf::Vector2f(2, 1);
    float factor = func1.getShortestDistanceFactor(point);
    sf::Vector2f pos = func1.getPoint(factor);
    float distance = func1.getShortestDistance(point);
    std::cout << "PosFactor: " << factor << " pos: " << pos.x << " " << pos.y << " distance: "<< distance<<"\n";
    
    

    
}

SandBox::~SandBox()
{
    delete m_canvas_1;
    delete m_canvas_2;
    delete ui;
}

void SandBox::onTimerFinished()
{
    std::vector<sf::Vector2f> points;
    
    for (size_t i = 0; i < 20; ++i)
    {
        points.push_back(QSFML::Utilities::RandomEngine::getVector({ 0,0 }, { 100,100 }));
    }
    m_pointPainter->setPoints(points);
}

void SandBox::closeEvent(QCloseEvent* event)
{
    if (m_canvas_1)
        m_canvas_1->stop();
    if (m_canvas_2)
        m_canvas_2->stop();
    //Canvas::stopEventLoop();
    event->accept();
}