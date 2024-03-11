#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include "Pendulum.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void setupCanvas();
    void closeEvent(QCloseEvent* event) override;


    Ui::MainWindow* ui;

    QSFML::Canvas* m_canvas;

    //Pendulum* m_pendulum;
};
