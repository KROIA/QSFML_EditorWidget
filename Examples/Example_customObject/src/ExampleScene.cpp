#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "CustomObject.h"

using namespace QSFML;
using namespace QSFML::Objects;

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    m_Scene = nullptr;
    setupScene();

	// Create a custom object and add it to the Scene
	CustomObject* customObject = new CustomObject();
	m_Scene->addObject(customObject);

    
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_Scene;
}

void ExampleScene::setupScene()
{
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
	settings.timing.frameTime = 0.02; // 50 fps
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_Scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_Scene->addObject(defaultEditor);
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_Scene)
        m_Scene->stop();
    event->accept();
}

