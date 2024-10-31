#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleScene; }
QT_END_NAMESPACE

class ExampleScene : public QMainWindow
{
    Q_OBJECT

public:
    ExampleScene(QWidget* parent = nullptr);
    ~ExampleScene();

    private slots:
   // void onScreenCapture();
private:
    void setupGeneralObjects();
    void setupVectorField();
    void setupGradientPainter();
    void setupScene_solarSystem();
    void setupScene_gravityAssist_accelerate();
    void setupScene_gravityAssist_decelerate();
    void closeEvent(QCloseEvent* event) override;


    Ui::ExampleScene* ui;
	
    QSFML::Scene* m_scene;
    QSFML::Objects::GameObjectPtr m_planetSystem;
	int m_worldSize;
};
