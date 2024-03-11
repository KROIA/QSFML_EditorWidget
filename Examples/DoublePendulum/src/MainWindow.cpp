#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QCloseEvent>

using namespace QSFML;
using namespace QSFML::Objects;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_canvas = nullptr;
    setupCanvas();
    
    

    
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_canvas;
}

void MainWindow::setupCanvas()
{
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.01;
    settings.timing.physicsFixedDeltaT = 0.1;
    settings.updateControlls.enableMultithreading = true;
    settings.updateControlls.threadSettings.threadCount = 32;
    settings.updateControlls.threadSettings.objectGroups = 64;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_canvas->addObject(defaultEditor);
    qDebug() << defaultEditor->toString().c_str();

    double angle1 = M_PI_2;
    double angle2 = M_PI;
    for (int i = 0; i < 1; ++i)
    {
        Pendulum * pendulum = new Pendulum();
        pendulum->setStart(angle1, angle2);
        angle2 += 0.001;
        m_canvas->addObject(pendulum);
    }
    
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

