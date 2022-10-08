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

    VisibleCamera *cam = new VisibleCamera();
    BackgroundGrid *grid = new BackgroundGrid();

    m_canvas->addObject(cam);
    m_canvas->addObject(grid);
}

SandBox::~SandBox()
{
    delete ui;
}

