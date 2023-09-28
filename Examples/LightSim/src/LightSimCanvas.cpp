#include "LightSimCanvas.h"
#include "ui_lightSimCanvas.h"
#include <iostream>
#include <QCloseEvent>


using namespace QSFML;
using namespace QSFML::Objects;

LightSimCanvas::LightSimCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LightSimCanvas)
{
    ui->setupUi(this);
    m_canvas = nullptr;

    setupCanvas();    
}

LightSimCanvas::~LightSimCanvas()
{
    delete ui;
    delete m_canvas;
}

void LightSimCanvas::setupCanvas()
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
    // Create a Key button to toggle the RuntimeInfo Text on and off
    QSFML::Components::KeyPressEvent* runtimeInfoKeyToggler = new QSFML::Components::KeyPressEvent("RuntimeInfoToggler", sf::Keyboard::I);
    connect(runtimeInfoKeyToggler, &Components::KeyPressEvent::fallingEdge, defaultEditor, &DefaultEditor::onToggleRuntimeInfo);
    defaultEditor->addComponent(runtimeInfoKeyToggler);
    m_canvas->addObject(defaultEditor);

    m_canvas->applyObjectChanges();
    //qDebug() << defaultEditor->toString().c_str();


    // Setup Laser
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.1));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.12));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.14));
        laser->setColor(sf::Color::Green);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.16));
        laser->setColor(sf::Color::Green);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.18));
        laser->setColor(sf::Color::Blue);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(10, 10));
        laser->setDirection(sf::Vector2f(1, 0.2));
        laser->setColor(sf::Color::Blue);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }


    // Setup mirrors
   /* {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(100, 40));
        mirror->setRotation(M_PI / 2);
        mirror->setWidth(55);
        m_mirrors.push_back(mirror);
        m_canvas->addObject(mirror);
    }*/
     {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(200, 100));
        mirror->setWidth(60);
        mirror->setRotation(0);
        m_mirrors.push_back(mirror);
        m_canvas->addObject(mirror);
    }
     {
         Mirror* mirror = new Mirror();
         mirror->setPos(sf::Vector2f(40, 80));
         mirror->setWidth(70);
         mirror->setRotation(M_PI / 2);
         m_mirrors.push_back(mirror);
         m_canvas->addObject(mirror);
     }
     {
         CurvedMirror* mirror = new CurvedMirror();
         mirror->setPos(sf::Vector2f(100, 35));
         mirror->setOpeningAngle(M_PI/4);
         mirror->setRotation(0);
         mirror->setColor(sf::Color(200, 200, 200));
         mirror->setRadius(50);
         m_canvas->addObject(mirror);
     }

}
void LightSimCanvas::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

