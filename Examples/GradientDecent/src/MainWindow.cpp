#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>
#include "Gradient.h"
#include "DifferentialEvol.h"

using namespace QSFML;
using namespace QSFML::Objects;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = nullptr;
    setupScene();
    
    

    
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;
}

void MainWindow::setupScene()
{
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    m_scene = new Scene(ui->SceneWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);
    qDebug() << defaultEditor->toString().c_str();


	// Gradient Decent
    //Gradient *gradient = new Gradient();
    //m_scene->addObject(gradient);
    //gradient->setStart(sf::Vector2f(0, -1));


	// Differential Evolution
	DifferentialEvol* de = new DifferentialEvol();
	m_scene->addObject(de);




	m_scene->start();
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_scene)
        m_scene->stop();
    event->accept();
}

