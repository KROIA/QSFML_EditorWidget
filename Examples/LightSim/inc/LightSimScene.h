#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

#include "Mirror.h"
#include "CurvedMirror.h"
#include "Laser.h"
#include "ConvexLens.h"
#include "ConcaveLens.h"
#include "SimpleLens.h"
#include "GlasRectangle.h"



QT_BEGIN_NAMESPACE
namespace Ui { class LightSimScene; }
QT_END_NAMESPACE

class LightSimScene : public QMainWindow
{
    Q_OBJECT

public:
    LightSimScene(QWidget* parent = nullptr);
    ~LightSimScene();

    

private:
    void setupScene(); 
    void closeEvent(QCloseEvent* event) override;

    Ui::LightSimScene* ui;

    QSFML::Scene* m_Scene;

    std::vector<Mirror*> m_mirrors;
    std::vector<Laser*> m_lasers;
    std::vector<ConvexLens*> m_convexLenss;

};
