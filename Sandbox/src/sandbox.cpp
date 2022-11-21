#include "sandbox.h"
#include "ui_sandbox.h"

#include "DrawableVector.h"
#include "AABBDisplayer.h"
#include "SandboxObject.h"

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
        //settings.updateControlls.enableMultithreading = false;
        //settings.updateControlls.enablePaintLoop = false;
        //settings.updateControlls.enableEventLoop = false;
        //settings.updateControlls.enableUpdateLoop = false;
        m_canvas_1 = new Canvas(ui->canvasWidget_1,settings);

        DefaultEditor *defaultEditor = new DefaultEditor();

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        m_canvas_1->addObject(defaultEditor);
        m_canvas_1->addObject(m_vecDisplay);
        //m_canvas->addObject(grid);
        qDebug() << defaultEditor->toString().c_str();
        qDebug() << m_vecDisplay->toString().c_str();

        SandboxObject *sbObj = new SandboxObject();
        m_canvas_1->addObject(sbObj);
    }

    {

        CanvasSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
       //settings.updateControlls.enableMultithreading = false;
       //settings.updateControlls.enablePaintLoop = false;
       //settings.updateControlls.enableEventLoop = false;
       //settings.updateControlls.enableUpdateLoop = false;
        m_canvas_2 = new Canvas(ui->canvasWidget_2,settings);

       DefaultEditor *defaultEditor = new DefaultEditor();
       defaultEditor->setRenderLayer(RenderLayer::layer_1);

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        //m_canvas_2->addObject(obj);
        //m_canvas_2->addObject(m_vecDisplay);
        m_canvas_2->addObject(defaultEditor);



        LineChart *m_chart = new LineChart();
        m_chart->setDataPoints({0,1,-1,0.5,-0.5,0});
        m_chart->setOrigin(sf::Vector2f(50,50));
        m_chart->setSize(sf::Vector2f(200,100));
        m_canvas_2->addObject(m_chart);
    }
}

SandBox::~SandBox()
{
    delete ui;
    delete m_canvas_2;
    delete m_canvas_1;
}

