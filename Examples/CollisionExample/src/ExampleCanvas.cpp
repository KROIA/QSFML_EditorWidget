#include "ExampleCanvas.h"
#include "ui_exampleCanvas.h"
#include <iostream>
#include <QTimer>
#include <QCloseEvent>

using namespace QSFML;
using namespace QSFML::Objects;

ExampleCanvas::ExampleCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleCanvas)
{
    ui->setupUi(this);

    
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();
    // Create a Key button to toggle the RuntimeInfo Text on and off
    QSFML::Components::KeyPressEvent* runtimeInfoKeyToggler = new QSFML::Components::KeyPressEvent("RuntimeInfoToggler", sf::Keyboard::I);
    connect(runtimeInfoKeyToggler, &Components::KeyPressEvent::fallingEdge, defaultEditor, &DefaultEditor::onToggleRuntimeInfo);
    defaultEditor->addComponent(runtimeInfoKeyToggler);
    m_canvas->addObject(defaultEditor);

    
    


    m_collisionChecker = new CollisionChecker();
    m_canvas->addObject(m_collisionChecker);


    m_canvas->applyObjectChanges();

    qDebug() << defaultEditor->toString().c_str();
    qDebug() << m_collisionChecker->toString().c_str();
    

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ExampleCanvas::onTimerFinished);
    timer->start(1000);
    
}

ExampleCanvas::~ExampleCanvas()
{
    delete ui;
    delete m_canvas;
}

void ExampleCanvas::on_intersecting_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::intersecting);
}
void ExampleCanvas::on_containing_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::contains);
}
void ExampleCanvas::on_performanceTest_radioButton_clicked()
{
    m_collisionChecker->setMode(CollisionChecker::Mode::performanceTest);
}
void ExampleCanvas::onTimerFinished()
{
    system("cls");
    QSFML::Stats stats = QSFML::StatsManager::getLastStats();
    qDebug() << "  Tick:                  " << m_canvas->getTick();
    qDebug() << "  FPS:                   " << m_canvas->getFPS();
    qDebug() << "  Frametime:             " << m_canvas->getFrametime()*1000.f << " ms";
    
    stats.print();
}
void ExampleCanvas::closeEvent(QCloseEvent* event)
{
    //Canvas::stopEventLoop();
    event->accept();
}