#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleScene; }
QT_END_NAMESPACE

class ExampleScene : public QMainWindow
{
    Q_OBJECT
    enum class Tutorial
    {
        tutorial_1,
        tutorial_2,
        tutorial_3,
        tutorial_4,
        tutorial_5
    };

public:
    ExampleScene(QWidget* parent = nullptr);
    ~ExampleScene();

private slots:
	void onTimer();
private:
    void setupTutorial_1(QSFML::Scene* scene);
    void setupTutorial_2(QSFML::Scene* scene);
    void setupTutorial_3(QSFML::Scene* scene);
    void setupTutorial_4(QSFML::Scene* scene);
    void setupTutorial_5(QSFML::Scene* scene);

    void setupScene(Tutorial t);
    void closeEvent(QCloseEvent* event) override;

    Ui::ExampleScene* ui;
    QSFML::Scene* m_scene = nullptr;
};
