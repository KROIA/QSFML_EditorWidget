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

private:
    void setupScene();
    void closeEvent(QCloseEvent* event) override;

	QSFML::Objects::GameObject* customizedObjectFactory(const std::string& name);
	QSFML::Objects::GameObject* nastedObjectFactory(const std::string& name);


    Ui::ExampleScene* ui;

    QSFML::Scene* m_scene;
};
