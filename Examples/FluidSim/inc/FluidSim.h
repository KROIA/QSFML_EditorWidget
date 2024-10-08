#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FluidSim; }
QT_END_NAMESPACE

class FluidSim : public QMainWindow
{
    Q_OBJECT

public:
    FluidSim(QWidget* parent = nullptr);
    ~FluidSim();

private:
    void setupScene();
    void closeEvent(QCloseEvent* event) override;


    Ui::FluidSim* ui;

    QSFML::Scene* m_scene;
};
