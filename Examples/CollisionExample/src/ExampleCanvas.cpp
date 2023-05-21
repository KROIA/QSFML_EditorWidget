#include "ExampleCanvas.h"
#include "ui_exampleCanvas.h"
#include <iostream>
#include <QTimer>

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
    settings.timing.frameTime = 16;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();
    m_canvas->addObject(defaultEditor);
    

    m_collisionChecker = new CollisionChecker();
    m_canvas->addObject(m_collisionChecker);

    m_canvas->applyObjectChanges();

    qDebug() << defaultEditor->toString().c_str();
    qDebug() << m_collisionChecker->toString().c_str();
    

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ExampleCanvas::onTimerFinished);
    timer->start(100);
    
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
    QSFML::Stats stats = QSFML::StatsManager::getStats(m_canvas);
    system("cls");
    stats.print();
}
