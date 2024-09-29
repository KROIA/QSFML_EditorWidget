#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleCanvas; }
QT_END_NAMESPACE

class ExampleCanvas : public QMainWindow
{
    Q_OBJECT

public:
    ExampleCanvas(QWidget* parent = nullptr);
    ~ExampleCanvas();

private:
    void setupCanvas();
    void closeEvent(QCloseEvent* event) override;


    Ui::ExampleCanvas* ui;

    QSFML::Canvas* m_canvas;
};
