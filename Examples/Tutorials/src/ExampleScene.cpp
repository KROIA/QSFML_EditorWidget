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
	Log::UI::NativeConsoleView::createStaticInstance();
	Log::UI::NativeConsoleView::getStaticInstance()->show();
    setupScene(Tutorial::tutorial_4);

	// QTimer* timer = new QTimer(this);
	// connect(timer, &QTimer::timeout, this, &ExampleScene::onTimer);
	// timer->start(30);
}

ExampleScene::~ExampleScene()
{
    delete ui;
    delete m_scene;
}

void ExampleScene::setupScene(Tutorial t)
{
    if (!m_scene)
    {
        SceneSettings settings;
        settings.contextSettings.antialiasingLevel = 8;
        settings.timing.frameTime = 0.01;
        m_scene = new Scene(ui->SceneWidget, settings);
    }
    else
    {
		m_scene->stop();
        m_scene->clearObjects();
    }

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);

    m_scene->applyObjectChanges();
    switch (t)
    {
		case Tutorial::tutorial_1: setupTutorial_1(m_scene); break;
		case Tutorial::tutorial_2: setupTutorial_2(m_scene); break;
		case Tutorial::tutorial_3: setupTutorial_3(m_scene); break;
		case Tutorial::tutorial_4: setupTutorial_4(m_scene); break;
		case Tutorial::tutorial_5: setupTutorial_5(m_scene); break;
    }

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

