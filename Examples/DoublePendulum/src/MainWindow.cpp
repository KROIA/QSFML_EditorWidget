#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QCloseEvent>
#include <QDebug>



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
			settings.timing.physicsFixedDeltaT = float(ui->speed_slider->value())/100000.f;
        m_canvas->setSettings(settings);
	}
}
void MainWindow::on_restart_pushButton_clicked()
{
	if (m_canvas)
	{
        createPendulums();
	}
}
void MainWindow::on_speed_slider_valueChanged(int value)
{
    auto settings = m_canvas->getSettings();
    if (settings.timing.physicsFixedDeltaT > 0)
        settings.timing.physicsFixedDeltaT = float(value) / 100000.f;
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
void MainWindow::on_damping_verticalSlider_valueChanged(int value)
{
	if (m_canvas)
	{
        float damping = (float)value/ui->damping_verticalSlider->maximum();
        for (int i = 0; i < m_pendulums.size(); ++i)
		{
			m_pendulums[i]->setDamping(damping, damping);
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
    settings.timing.frameTime = 0.00;

    settings.timing.physicsFixedDeltaT = 0.1;
    settings.updateControlls.enableMultithreading = false;
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

    ui->pendulumCount_spinBox->setValue(2);
    createPendulums();
    
}
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_canvas)
        m_canvas->stop();
    event->accept();
}

void MainWindow::createPendulums()
{
    for (size_t i = 0; i < m_pendulums.size(); ++i)
    {
        m_canvas->deleteObject(m_pendulums[i]);
    }
    m_pendulums.clear();
    double angle1 = M_PI_2;
    double angle2 = M_PI_2;
    int count = ui->pendulumCount_spinBox->value();
    for (int i = 0; i < count; ++i)
    {
        Pendulum* pendulum = nullptr;
        if (i % 2 == 0)
        {
            pendulum = new Pendulum();
            pendulum->setColor(sf::Color(255,0,0));
        }
        else
        {
            pendulum = new WikiPendulum();
            pendulum->setColor(sf::Color(0,255,0));
		}

        m_pendulums.push_back(pendulum);
        pendulum->setPositionAbsolute(sf::Vector2f(500, 500));
        pendulum->setStart(angle1, angle2);
        
        /*
        int r = (1 + sin(angle1)) * 127.5;
        int g = (1 + sin(angle1 + M_PI * 2 / 3.f)) * 127.5;
        int b = (1 + sin(angle1 + M_PI * 4 / 3.f)) * 127.5;
        pendulum->setColor(sf::Color(r, g, b));
        */

        if (i % 2 != 0)
        {
            angle1 += M_PI * 2 / (double)count;
            angle2 += M_PI * 2 / (double)count;
        }
        
        //angle2 += 0.01;
        m_canvas->addObject(pendulum);

        float length = ui->L1_verticalSlider->value();
        float damping = (float)ui->damping_verticalSlider->value() / ui->damping_verticalSlider->maximum();
        pendulum->setLength(length, length);
        pendulum->setDamping(damping, damping);

    }
}

