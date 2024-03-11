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
void MainWindow::on_startStop_pushButton_clicked()
{
	if (m_canvas)
	{
        auto settings = m_canvas->getSettings();
        if(settings.timing.physicsFixedDeltaT > 0)
			settings.timing.physicsFixedDeltaT = 0;
		else
			settings.timing.physicsFixedDeltaT = float(ui->speed_slider->value())/1000.f;
        m_canvas->setSettings(settings);
	}
}
void MainWindow::on_restart_pushButton_clicked()
{
	if (m_canvas)
	{
        double angle1 = M_PI_2;
        double angle2 = M_PI;
        for (int i = 0; i < m_pendulums.size(); ++i)
        {
            m_pendulums[i]->setStart(angle1, angle1);
            angle1 += M_PI * 2 / (double)m_pendulums.size();
        }
	}
}
void MainWindow::on_speed_slider_valueChanged(int value)
{
    auto settings = m_canvas->getSettings();
    if (settings.timing.physicsFixedDeltaT > 0)
        settings.timing.physicsFixedDeltaT = float(value) / 1000.f;
    m_canvas->setSettings(settings);
}
void MainWindow::on_L1_verticalSlider_valueChanged(int value)
{
    if (m_canvas)
    {
        for (int i = 0; i < m_pendulums.size(); ++i)
        {
            m_pendulums[i]->setLength(value, ui->L2_verticalSlider->value());
        }
    }
}
void MainWindow::on_L2_verticalSlider_valueChanged(int value)
{
    if (m_canvas)
    {
        for (int i = 0; i < m_pendulums.size(); ++i)
        {
            m_pendulums[i]->setLength(ui->L1_verticalSlider->value(), value);
        }
    }
}
void MainWindow::on_enableLines_checkBox_stateChanged(int arg1)
{
	if (m_canvas)
	{
		for (int i = 0; i < m_pendulums.size(); ++i)
		{
			m_pendulums[i]->setLinesEnabled(arg1);
		}
	}

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
    settings.timing.physicsFixedDeltaT = 0;
    //settings.colors.defaultBackground = sf::Color(0,0,0);
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_canvas = new Canvas(ui->canvasWidget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor("Editor",sf::Vector2f(1000, 1000));
    defaultEditor->getGrid()->setEnabled(false);
    m_canvas->addObject(defaultEditor);

    qDebug() << defaultEditor->toString().c_str();

    double angle1 = 0;
    double angle2 = 0;
    int count = 10000;
    for (int i = 0; i < count; ++i)
    {
        Pendulum * pendulum = new Pendulum();
        m_pendulums.push_back(pendulum);
        pendulum->setStart(angle1, angle1);
        angle1 += M_PI*2/(double)count;
        m_canvas->addObject(pendulum);

        m_pendulums[i]->setLength(ui->L1_verticalSlider->value(), 
                                  ui->L2_verticalSlider->value());
    }
    
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

