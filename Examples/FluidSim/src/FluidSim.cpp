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
    m_Scene = nullptr;
    setupScene();
    
    

    
}

FluidSim::~FluidSim()
{
    delete ui;
    delete m_Scene;
}

void FluidSim::setupScene()
{
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_Scene = new Scene(ui->SceneWidget, settings);

    //DefaultEditor* defaultEditor = new DefaultEditor();
    //m_Scene->addObject(defaultEditor);
    
    
   // qDebug() << defaultEditor->toString().c_str();

    sf::Vector2u tiles = { 100,100 };
    sf::Vector2f tileSize = { 10,10 };

    VisibleCamera* camera = new VisibleCamera();
    camera->setMaxMovingBounds(sf::FloatRect({0,0}, { tiles.x * tileSize.x, tiles.y * tileSize.y }));
    m_Scene->addObject(camera);

    FluidGrid* fluidGrid = new FluidGrid(tiles, tileSize);
    fluidGrid->setRenderLayer(QSFML::RenderLayer::layer_1);
    m_Scene->addObject(fluidGrid);


}
void FluidSim::closeEvent(QCloseEvent* event)
{
    if (m_Scene)
        m_Scene->stop();
    event->accept();
}

