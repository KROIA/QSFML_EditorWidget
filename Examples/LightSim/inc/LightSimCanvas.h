#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

#include "Mirror.h"
#include "CurvedMirror.h"
#include "Laser.h"
#include "ConvexLense.h"


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
    void setupCanvas(); 
    void closeEvent(QCloseEvent* event) override;

    Ui::LightSimCanvas* ui;

    QSFML::Canvas* m_canvas;

    std::vector<Mirror*> m_mirrors;
    std::vector<Laser*> m_lasers;
    std::vector<ConvexLense*> m_convexLenses;

};
