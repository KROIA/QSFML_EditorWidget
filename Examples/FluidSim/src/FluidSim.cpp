#include "FluidSim.h"
#include "ui_FluidSim.h"
#include <iostream>
#include <QCloseEvent>

#include "FluidGrid.h"

using namespace QSFML;
using namespace QSFML::Objects;

FluidSim::FluidSim(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FluidSim)
{
    ui->setupUi(this);
    m_canvas = nullptr;
    setupCanvas();
    
    

    
}

FluidSim::~FluidSim()
{
    delete ui;
    delete m_canvas;
}

void FluidSim::setupCanvas()
{
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget, settings);

    //DefaultEditor* defaultEditor = new DefaultEditor();
    //m_canvas->addObject(defaultEditor);
    
    
   // qDebug() << defaultEditor->toString().c_str();

    sf::Vector2u tiles = { 100,100 };
    sf::Vector2f tileSize = { 10,10 };

    VisibleCamera* camera = new VisibleCamera();
    camera->setMaxMovingBounds(sf::FloatRect({0,0}, { tiles.x * tileSize.x, tiles.y * tileSize.y }));
    m_canvas->addObject(camera);

    FluidGrid* fluidGrid = new FluidGrid(tiles, tileSize);
    fluidGrid->setRenderLayer(QSFML::RenderLayer::layer_1);
    m_canvas->addObject(fluidGrid);


}
void FluidSim::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

