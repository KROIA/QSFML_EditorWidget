#include "ExampleScene.h"
#include "ui_exampleScene.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include <QTimer>

using namespace QSFML;
using namespace QSFML::Objects;

ExampleScene::ExampleScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExampleScene)
{
    ui->setupUi(this);
    setupScene();

	// QTimer* timer = new QTimer(this);
	// connect(timer, &QTimer::timeout, this, &ExampleScene::onTimer);
	// timer->start(30);
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_scene;
}

void ExampleScene::setupScene()
{
    SceneSettings settings;
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.00;
    m_scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);
    m_scene->start();
}
void ExampleScene::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

void ExampleScene::onTimer()
{
    static int counter = 0;
	sf::Image image = m_scene->captureScreen();
	image.saveToFile("screenshots/screenshot" + std::to_string(counter) + ".png");
	++counter;
}

