#include "sandbox.h"
#include "ui_sandbox.h"
#include "VisibleCamera.h"
#include "BackgroundGrid.h"

SandBox::SandBox(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SandBox)
{
    ui->setupUi(this);

    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    m_canvas = new QSFML_Canvas(ui->canvasWidget,settings);

    VisibleCamera *cam = new VisibleCamera("Camera");
    BackgroundGrid *grid = new BackgroundGrid("Grid");
    CanvasObject *obj = new CanvasObject("Test");
    grid->addChild(cam);
    obj->addChild(grid);
    obj->addChild(cam);

    m_canvas->addObject(obj);
    //m_canvas->addObject(grid);

    //m_canvas->instantiateAdd<BackgroundGrid>();

    qDebug() << obj->toString().c_str();
    //qDebug() << grid->toString().c_str();
}

SandBox::~SandBox()
{
    delete ui;
}

