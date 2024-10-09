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

    Ui::ExampleScene* ui;
    QSFML::Scene* m_scene = nullptr;
};
