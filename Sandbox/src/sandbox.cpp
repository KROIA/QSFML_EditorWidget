#include "sandbox.h"
#include "ui_sandbox.h"
#include "VisibleCamera.h"
#include "BackgroundGrid.h"


#include "VectorDisplayer.h"
#include "AABBDisplayer.h"

using namespace QSFML;
using namespace QSFML::Objects;

SandBox::SandBox(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SandBox)
{
    ui->setupUi(this);

    {
        CanvasSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        m_canvas_1 = new Canvas(ui->canvasWidget_1,settings);

        VisibleCamera *cam      = new VisibleCamera("Camera");
        BackgroundGrid *grid    = new BackgroundGrid("Grid");
        CanvasObject *obj       = new CanvasObject("Test");

        grid->setSize(sf::IntRect(0,0,800,600));
        grid->setLineColor({sf::Color(130,130,130),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100)});
        cam->setMaxMovingBounds(sf::FloatRect(grid->getSize()));

        obj->addChild(grid);
        obj->addChild(cam);

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        m_canvas_1->addObject(obj);
        m_canvas_1->addObject(m_vecDisplay);
        //m_canvas->addObject(grid);
        qDebug() << obj->toString().c_str();
        qDebug() << m_vecDisplay->toString().c_str();
    }

    {

        CanvasSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        m_canvas_2 = new Canvas(ui->canvasWidget_2,settings);

        VisibleCamera *cam      = new VisibleCamera("Camera");
        BackgroundGrid *grid    = new BackgroundGrid("Grid");
        AABBDisplayer *aabb     = new AABBDisplayer("AABB");

        grid->setSize(sf::IntRect(0,0,800,600));
        grid->setLineColor({sf::Color(130,130,130),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100),
                            sf::Color(100,100,100)});
        cam->setMaxMovingBounds(sf::FloatRect(grid->getSize()));

       // obj->addChild(grid);
       // obj->addChild(cam);

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        //m_canvas_2->addObject(obj);
        //m_canvas_2->addObject(m_vecDisplay);
        m_canvas_2->addObject(grid);
        m_canvas_2->addObject(aabb);

        m_canvas_2->addObject(cam);
    }
}

SandBox::~SandBox()
{
    delete ui;
}

