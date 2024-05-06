#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include "Pendulum.h"
#include "WikiPendulum.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_startStop_pushButton_clicked();
    void on_restart_pushButton_clicked();
    void on_speed_slider_valueChanged(int value);
    void on_L1_verticalSlider_valueChanged(int value);
    void on_L2_verticalSlider_valueChanged(int value);
    void on_damping_verticalSlider_valueChanged(int value);
    void on_enableLines_checkBox_stateChanged(int arg1);
	void on_enablePath_checkBox_stateChanged(int arg1);
	void on_enableEnergyLabel_checkBox_stateChanged(int arg1);

private:
    void setupCanvas();
    void closeEvent(QCloseEvent* event) override;
    void createPendulums();


    Ui::MainWindow* ui;

    QSFML::Canvas* m_canvas;
    std::vector<Pendulum*> m_pendulums;

    //Pendulum* m_pendulum;
};
