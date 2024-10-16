#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QTimer>
#include <QCloseEvent>
#include <QDebug>


//#define ENABLE_SCREEN_CAPTURE

using namespace QSFML;
using namespace QSFML::Objects;

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
    , m_collisionChecker(nullptr)
{
    ui->setupUi(this);
    ui->consoleFrame->layout()->addWidget(&m_console);
    m_console.show();
    
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.05;
    settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_scene = new Scene(ui->SceneWidget,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();
    // Create a Key button to toggle the RuntimeInfo Text on and off
    //QSFML::Components::KeyPressEvent* runtimeInfoKeyToggler = new QSFML::Components::KeyPressEvent("RuntimeInfoToggler", sf::Keyboard::I);
    //connect(runtimeInfoKeyToggler, &Components::KeyPressEvent::fallingEdge, defaultEditor, &DefaultEditor::onToggleRuntimeInfo);
    //defaultEditor->addComponent(runtimeInfoKeyToggler);
    m_scene->addObject(defaultEditor);

    
    


   m_collisionChecker = new CollisionChecker();
   m_scene->addObject(m_collisionChecker);


    m_scene->applyObjectChanges();

    qDebug() << defaultEditor->toString().c_str();
    if(m_collisionChecker)
        qDebug() << m_collisionChecker->toString().c_str();
    

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ExampleScene::onTimerFinished);
    timer->start(1000);
    m_scene->start();


#ifdef ENABLE_SCREEN_CAPTURE
    // Create a timer to capture the screen
    //QTimer* timer = new QTimer(this);
    // Create the directory for the screenshots
    //QDir().mkdir("screenshots");
    //connect(timer, &QTimer::timeout, this, &ExampleScene::onScreenCapture, Qt::DirectConnection);
    //timer->start(50);
    Utilities::CameraRecorder* recorder = new Utilities::CameraRecorder(m_scene->getDefaultCamera(), 4);
    QTimer* singleShotTimer = new QTimer(this);
    singleShotTimer->singleShot(1000, [recorder]()
        {
            recorder->startCapture(500, 0.03, "screenshots/CollisionExample");
            //recorder->startCapture(100, 0.01, "screenshots");
        });
    singleShotTimer->start();



#endif
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_scene;
}

void ExampleScene::on_intersecting_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::intersecting);
}
void ExampleScene::on_containing_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::contains);
}
void ExampleScene::on_performanceTest_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::performanceTest);
}
void ExampleScene::onTimerFinished()
{
    return;
    system("cls");
    QSFML::Utilities::Stats stats = m_scene->getLastStats();
    //qDebug() << "  Tick:                  " << m_scene->getTick();
    //qDebug() << "  FPS:                   " << m_scene->getFPS();
    //qDebug() << "  Frametime:             " << m_scene->getFrametime()*1000.f << " ms";
    

    //stats.print();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    //Scene::stopEventLoop();
    m_console.close();
    event->accept();
}
