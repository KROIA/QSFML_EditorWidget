#include "ExampleCanvas.h"
#include "ui_exampleCanvas.h"
#include <iostream>
#include "CollisionChecker.h"

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
    qDebug() << defaultEditor->toString().c_str();

    m_canvas->addObject(new CollisionChecker());
    
}

ExampleCanvas::~ExampleCanvas()
{
    delete ui;
    delete m_canvas;
}

