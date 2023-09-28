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
    m_canvas->addObject(defaultEditor);

    m_canvas->applyObjectChanges();
    qDebug() << defaultEditor->toString().c_str();

    
}

LightSimCanvas::~LightSimCanvas()
{
    delete ui;
    delete m_canvas;
}


void LightSimCanvas::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

