#include "LightSimScene.h"
#include "ui_lightSimScene.h"
#include <iostream>
#include <QCloseEvent>

//#define ENABLE_SCREEN_CAPTURE

using namespace QSFML;
using namespace QSFML::Objects;

LightSimScene::LightSimScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LightSimScene)
{
    ui->setupUi(this);
    m_scene = nullptr;

    setupScene();    

#ifdef ENABLE_SCREEN_CAPTURE
    Utilities::CameraRecorder* recorder = new Utilities::CameraRecorder(m_scene->getDefaultCamera(), 4);
    QTimer* singleShotTimer = new QTimer(this);
    singleShotTimer->singleShot(1000, [recorder]()
        {
            recorder->startCapture(500, 0.03, "screenshots/LightSim");
        });
    singleShotTimer->start();
#endif
}

LightSimScene::~LightSimScene()
{
    delete ui;
    delete m_scene;
}

void LightSimScene::setupScene()
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
    m_scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    // Create a Key button to toggle the RuntimeInfo Text on and off
    QSFML::Components::KeyPressEvent* runtimeInfoKeyToggler = new QSFML::Components::KeyPressEvent("RuntimeInfoToggler", sf::Keyboard::I);
    connect(runtimeInfoKeyToggler, &Components::KeyPressEvent::fallingEdge, defaultEditor, &DefaultEditor::onToggleRuntimeInfo);
    defaultEditor->addComponent(runtimeInfoKeyToggler);
    m_scene->addObject(defaultEditor);

    m_scene->applyObjectChanges();
    //qDebug() << defaultEditor->toString().c_str();

        // Setup mirrors
    /*
   {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(300, 600));
        mirror->setRotation(M_PI / 2);
        mirror->setWidth(55);
        m_mirrors.push_back(mirror);
        m_scene->addObject(mirror);
    }
     {
        Mirror* mirror = new Mirror();
        mirror->setPos(sf::Vector2f(200, 400));
        mirror->setWidth(60);
        mirror->setRotation(0);
        m_mirrors.push_back(mirror);
        m_scene->addObject(mirror);
    }
     {
         Mirror* mirror = new Mirror();
         mirror->setPos(sf::Vector2f(800, 300));
         mirror->setWidth(70);
         mirror->setRotation(M_PI / 2);
         m_mirrors.push_back(mirror);
         m_scene->addObject(mirror);
     }
     {
         CurvedMirror* mirror = new CurvedMirror();
         mirror->setPos(sf::Vector2f(800, 200));
         mirror->setOpeningAngle(M_PI/4);
         mirror->setRotation(0);
         mirror->setColor(sf::Color(200, 200, 200));
         mirror->setRadius(50);
         m_scene->addObject(mirror);
     }*/

    //float refraction = 1.39;
    float refraction = 1.5;
    float brennweite;
     {
        ConvexLens* lense = new ConvexLens();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPosition(sf::Vector2f(200, 200));
        lense->setFocusLength(60);
        lense->setThickness(50);
        lense->setRotation(0);
        lense->setColor(sf::Color(200, 200, 200));
        lense->setDiameter(300);
        lense->setRefractionIndexInside(refraction);
        m_scene->addObject(lense);
        m_convexLenss.push_back(lense);
        brennweite = lense->getFocusLength();
        
    }
    {
        ConcaveLens* lense = new ConcaveLens();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPosition(sf::Vector2f(260, 201));
        lense->setFocusLength(40);
        lense->setThickness(50);
        lense->setRotation(0);
        lense->setColor(sf::Color(200, 200, 200));
        lense->setDiameter(300);
        lense->setRefractionIndexInside(refraction);
        m_scene->addObject(lense);
        //m_convexLenss.push_back(lense);
        brennweite = lense->getFocusLength();

    }
    /* {
        SimpleLens* lense = new SimpleLens();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPos(sf::Vector2f(200, 200));
        lense->setFocusLength(60);
        lense->setRotation(M_PI_2);
        lense->setColor(sf::Color(200, 200, 200));
        lense->setDiameter(254);
        m_scene->addObject(lense);
       // m_convexLenss.push_back(lense);
       // brennweite = lense->getFocusLength();
    }
    {
        SimpleLens* lense = new SimpleLens();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPos(sf::Vector2f(300, 201));
        lense->setFocusLength(-20);
        lense->setRotation(M_PI_2);
        lense->setColor(sf::Color(200, 200, 200));
        lense->setDiameter(254);
        m_scene->addObject(lense);
        //m_convexLenss.push_back(lense);
        //brennweite = lense->getFocusLength();
    }*/
   {
        ConvexLens* lense = new ConvexLens();
        //lense->setPos(sf::Vector2f(400, 200));
        lense->setPosition(sf::Vector2f(600, 250));
        lense->setFocusLength(200);
        lense->setThickness(50);
        lense->setRotation(0);
        lense->setColor(sf::Color(200, 200, 200));
        lense->setDiameter(200);
        lense->setRefractionIndexInside(refraction);
        
        m_scene->addObject(lense);
        m_convexLenss.push_back(lense);
        
    }
    
        {
             ConvexLens* lense = new ConvexLens();
             lense->setPosition(sf::Vector2f(400, 300));
             lense->setFocusLength(100);
             lense->setRefractionIndexInside(1.5);
             lense->setRotation(0);
             lense->setColor(sf::Color(200, 200, 200));
             lense->setDiameter(100);
             lense->setRotation(M_PI / 10);
			 
             m_scene->addObject(lense);
             m_convexLenss.push_back(lense);
             //lense->addUpdateFunction([lense](GameObject& obj)
             //    {
             //        lense->setRotation(lense->getRotation() + 3 * obj.getDeltaT());
             //    });
             
         }
         
    sf::Vector2f laserPos(0, 180);
    // Setup Laser
     /*{
        Laser* laser = new Laser();
        laser->setPosition(sf::Vector2f(0,110));
        laser->setDirection(sf::Vector2f(200,200) - laser->getPos());
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
      {
          Laser* laser = new Laser();
          laser->setPosition(sf::Vector2f(0, 110));
          laser->setDirection(sf::Vector2f(1,0));
          m_scene->addObject(laser);
          m_lasers.push_back(laser);
      }*/
   /* {
        Laser* laser = new Laser();
        laser->setPosition(sf::Vector2f(-200, 10));
        laser->setDirection(sf::Vector2f(1, 0));
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }*/
   /* {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0));
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
   {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.1));
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
   {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.12));
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.14));
        laser->setColor(sf::Color::Green);
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.16));
        laser->setColor(sf::Color::Green);
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.18));
        laser->setColor(sf::Color::Blue);
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }
    {
        Laser* laser = new Laser();
        laser->setPosition(laserPos);
        laser->setDirection(sf::Vector2f(1, 0.2));
        laser->setColor(sf::Color::Blue);
        m_scene->addObject(laser);
        m_lasers.push_back(laser);
    }*/
  {
        /*sf::Vector2f offset(0, 10);
        sf::Vector2f startPos(10, 100);
        for (int i = 0; i < 20; ++i)
        {
            Laser* laser = new Laser();
            laser->setPosition(startPos + (float)i * offset);
            laser->setDirection(sf::Vector2f(1, 0));
            laser->setColor(sf::Color::Green);
            m_scene->addObject(laser);
            m_lasers.push_back(laser);
        }*/
    }

      {
        sf::Vector2f offset(0, 5);
        sf::Vector2f startPos(10, 100);
        sf::Vector2f dir(1, -0.1);
            for (int i = 0; i < 10; ++i)
            {
                Laser* laser = new Laser();
                laser->setPosition(startPos + (float)i * offset);
                laser->setDirection(dir);
                dir.y += 0.05;
                laser->setColor(sf::Color::Yellow);
                m_scene->addObject(laser);
                m_lasers.push_back(laser);

                laser->addUpdateFunction([laser, dir](GameObject& obj)
                    {
                        sf::Vector2f _dir = dir;
                        float angle = std::sin(laser->getAge())*0.5;
						_dir = VectorMath::getRotatedRAD(_dir, angle);
                        laser->setDirection(_dir);
                    });
            }
    }

   // GlasRectangle* rect = new GlasRectangle();
   // m_scene->addObject(rect);
   // return;
    


  m_scene->start();
}
void LightSimScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

