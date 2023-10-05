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

        // Setup mirrors
  /* {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(300, 600));
        mirror->setRotation(M_PI / 2);
        mirror->setWidth(55);
        m_mirrors.push_back(mirror);
        m_canvas->addObject(mirror);
    }
     {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(200, 400));
        mirror->setWidth(60);
        mirror->setRotation(0);
        m_mirrors.push_back(mirror);
        m_canvas->addObject(mirror);
    }
     {
         Mirror* mirror = new Mirror();
         mirror->setPos(sf::Vector2f(800, 300));
         mirror->setWidth(70);
         mirror->setRotation(M_PI / 2);
         m_mirrors.push_back(mirror);
         m_canvas->addObject(mirror);
     }
     {
         CurvedMirror* mirror = new CurvedMirror();
         mirror->setPos(sf::Vector2f(800, 200));
         mirror->setOpeningAngle(M_PI/4);
         mirror->setRotation(0);
         mirror->setColor(sf::Color(200, 200, 200));
         mirror->setRadius(50);
         m_canvas->addObject(mirror);
     }
*/
    //float refraction = 1.39;
    float refraction = 1.5;
    float brennweite;
   /* {
        ConvexLense* lense = new ConvexLense();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPos(sf::Vector2f(100, 200));
        lense->setFocusLength(200);
        lense->setThickness(80);
        lense->setRotation(0);
        lense->setColor(sf::Color(200, 200, 200));
        lense->serDiameter(254);
        lense->setRefractionIndexInside(refraction);
        m_canvas->addObject(lense);
        m_convexLenses.push_back(lense);
        brennweite = lense->getFocusLength();
    }*/
   {
        ConvexLense* lense = new ConvexLense();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPos(sf::Vector2f(600, 200));
        lense->setFocusLength(200);
        lense->setThickness(50);
        lense->setRotation(1);
        lense->setColor(sf::Color(200, 200, 200));
        lense->serDiameter(200);
        lense->setRefractionIndexInside(refraction);
        m_canvas->addObject(lense);
        m_convexLenses.push_back(lense);
    }
    
        /* {
             ConvexLense* lense = new ConvexLense();
             lense->setPos(sf::Vector2f(600, 200));
             lense->setFocusLength(100);
             lense->setRefractionIndex(1.5);
             lense->setRotation(0);
             lense->setColor(sf::Color(200, 200, 200));
             lense->serDiameter(100);
             lense->setRotation(M_PI / 10);
             m_canvas->addObject(lense);
             m_convexLenses.push_back(lense);
         }*/

    sf::Vector2f laserPos(0, 180);
    // Setup Laser
     /*{
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(0,110));
        laser->setDirection(sf::Vector2f(200,200) - laser->getPos());
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
      {
          Laser* laser = new Laser();
          laser->setPos(sf::Vector2f(0, 110));
          laser->setDirection(sf::Vector2f(1,0));
          m_canvas->addObject(laser);
          m_lasers.push_back(laser);
      }*/
   /* {
        Laser* laser = new Laser();
        laser->setPos(sf::Vector2f(-200, 10));
        laser->setDirection(sf::Vector2f(1, 0));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }*/
    /* {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
   {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.1));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
   {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.12));
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.14));
        laser->setColor(sf::Color::Green);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.16));
        laser->setColor(sf::Color::Green);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.18));
        laser->setColor(sf::Color::Blue);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPos(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.2));
        laser->setColor(sf::Color::Blue);
        m_canvas->addObject(laser);
        m_lasers.push_back(laser);
    }*/
  {
        sf::Vector2f offset(0, 1);
        sf::Vector2f startPos(20, 150);
        for (int i = 0; i < 200; ++i)
        {
            Laser* laser = new Laser();
            laser->setPos(startPos + (float)i * offset);
            laser->setDirection(sf::Vector2f(1, 0));
            laser->setColor(sf::Color::Green);
            m_canvas->addObject(laser);
            m_lasers.push_back(laser);
        }
    }

    /* {
        sf::Vector2f offset(0, 5);
        sf::Vector2f startPos(10, 100);
        sf::Vector2f dir(1, -0.1);
            for (int i = 0; i < 10; ++i)
            {
                Laser* laser = new Laser();
                laser->setPos(startPos + (float)i * offset);
                laser->setDirection(dir);
                dir.y += 0.05;
                laser->setColor(sf::Color::Yellow);
                m_canvas->addObject(laser);
                m_lasers.push_back(laser);
            }
    }*/

   // GlasRectangle* rect = new GlasRectangle();
   // m_canvas->addObject(rect);
   // return;
    



}
void LightSimCanvas::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

