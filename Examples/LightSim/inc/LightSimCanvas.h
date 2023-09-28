#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LightSimCanvas; }
QT_END_NAMESPACE

class LightSimCanvas : public QMainWindow
{
    Q_OBJECT

public:
    LightSimCanvas(QWidget* parent = nullptr);
    ~LightSimCanvas();

    

private:
    void closeEvent(QCloseEvent* event) override;

    Ui::LightSimCanvas* ui;

    QSFML::Canvas* m_canvas;
};
