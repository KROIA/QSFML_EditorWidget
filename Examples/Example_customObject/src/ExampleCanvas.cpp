#include "ExampleCanvas.h"
#include "ui_exampleCanvas.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "CustomObject.h"

using namespace QSFML;
using namespace QSFML::Objects;

ExampleCanvas::ExampleCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleCanvas)
{
    ui->setupUi(this);
    m_canvas = nullptr;
    setupCanvas();

	// Create a custom object and add it to the canvas
	CustomObject* customObject = new CustomObject();
	m_canvas->addObject(customObject);

    
}

ExampleCanvas::~ExampleCanvas()
{
    delete ui;
    delete m_canvas;
}

void ExampleCanvas::setupCanvas()
{
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
	settings.timing.frameTime = 0.02; // 50 fps
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_canvas->addObject(defaultEditor);
}
void ExampleCanvas::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

