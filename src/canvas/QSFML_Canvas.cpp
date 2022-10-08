#include "QSFML_Canvas.h"
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QDebug>
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif


QSFML_Canvas::QSFML_Canvas(QWidget* parent, const CanvasSettings &settings) :
  QWidget(parent),
 //CameraInterface(),
  DrawInterface(),
  CanvasObjectContainer(this)
  //sf::RenderWindow(),
{
    m_window = nullptr;
    // Setup layout of this widget
    if(!parentWidget()->layout())
    {
        QHBoxLayout *layout = new QHBoxLayout(parentWidget());
        parentWidget()->setLayout(layout);
    }
    parentWidget()->layout()->addWidget(this);

    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);


    setSettings(settings);
}
QSFML_Canvas::~QSFML_Canvas()
{
    delete m_window;
}

void QSFML_Canvas::setSettings(const CanvasSettings &settings)
{
    setLayout(settings.layout);
    setTiming(settings.timing);
}
const CanvasSettings &QSFML_Canvas::getSettings() const
{
    return m_settings;
}
void QSFML_Canvas::setLayout(const CanvasSettings::Layout &layout)
{
    m_settings.layout = layout;
    parentWidget()->layout()->setContentsMargins(m_settings.layout.margin.left,
                                                 m_settings.layout.margin.top,
                                                 m_settings.layout.margin.right,
                                                 m_settings.layout.margin.bottom);
    if(!m_settings.layout.autoAjustSize)
    {
        QWidget::setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
    }
}
const CanvasSettings::Layout QSFML_Canvas::getLayout() const
{
    return m_settings.layout;
}
void QSFML_Canvas::setTiming(const CanvasSettings::Timing &timing)
{
    m_settings.timing = timing;

    // Setup the timer
    m_frameTimer.setInterval(m_settings.timing.frameTime);
}
const CanvasSettings::Timing &QSFML_Canvas::getTiming() const
{
    return m_settings.timing;
}
void QSFML_Canvas::setContextSettings(const sf::ContextSettings &contextSettings)
{
    m_settings.contextSettings = contextSettings;
}
const sf::ContextSettings &QSFML_Canvas::getContextSettings() const
{
    return m_settings.contextSettings;
}
/*
void QSFML_Canvas::zoomCamera(float zoomValue)
{

}
void QSFML_Canvas::setCameraZoom(float zoomValue)
{

}
float QSFML_Canvas::getCameraZoom() const
{

}

void QSFML_Canvas::moveCamera(const sf::Vector2f &worldCoords)
{

}
void QSFML_Canvas::setCameraPosition(const sf::Vector2f &worldCoords)
{

}
const sf::Vector2f &QSFML_Canvas::getCameraPosition() const
{

}*/

void QSFML_Canvas::setCameraView(const sf::View &view)
{
    if(!m_window) return;
    m_window->setView(view);
}
const sf::View &QSFML_Canvas::getCameraView() const
{
    static sf::View dummy;
    if(!m_window) return dummy;
    return m_window->getView();
}
const sf::View &QSFML_Canvas::getDefaultCameraView() const
{
    static sf::View dummy;
    if(!m_window) return dummy;
    return m_window->getDefaultView();
}

sf::Vector2i QSFML_Canvas::getMousePosition() const
{
    if(m_window)
        return sf::Mouse::getPosition(*m_window);
    return sf::Mouse::getPosition();
}
sf::Vector2f QSFML_Canvas::getMouseWorldPosition() const
{
    if(!m_window) return sf::Vector2f(0,0);
    sf::Vector2i pixelPos = getMousePosition();
    return m_window->mapPixelToCoords(pixelPos);
}
sf::Vector2f QSFML_Canvas::getInWorldSpace(const sf::Vector2i &pixelSpace)
{
    if(!m_window) return sf::Vector2f(0,0);
    return m_window->mapPixelToCoords(pixelSpace);
}
sf::Vector2i QSFML_Canvas::getInScreenSpace(const sf::Vector2f &worldSpace)
{
    if(!m_window) return sf::Vector2i(0,0);
    return m_window->mapCoordsToPixel(worldSpace);

}

void QSFML_Canvas::OnInit()
{

}

void QSFML_Canvas::OnUpdate()
{

}

QPaintEngine* QSFML_Canvas::paintEngine() const
{
    return nullptr;
}
void QSFML_Canvas::showEvent(QShowEvent*)
{
    if (!m_window)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        m_window = new sf::RenderWindow((sf::WindowHandle)QWidget::winId(),m_settings.contextSettings);

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&m_frameTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        m_frameTimer.start();
    }
}
void QSFML_Canvas::paintEvent(QPaintEvent*)
{
    if(!m_window)return;
    // Let the derived class do its specific stuff
    OnUpdate();
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        sfEvent(event);
        internal_event(event);
    }
    m_window->clear(sf::Color(100,100,100));

    /*sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0, 0)),
        sf::Vertex(sf::Vector2f(1500, 1500))
    };*/

   // m_window->draw(line,2, sf::Lines);
    CanvasObjectContainer::draw();
    // Display on screen
    m_window->display();
}

void QSFML_Canvas::resizeEvent(QResizeEvent *event)
{
    if(!m_window) return;
    if(m_settings.layout.autoAjustSize)
    {
        QSize size = event->size();
        m_window->setSize(sf::Vector2u(size.width(),size.height()));
        sf::View view2(sf::Vector2f(size.width(), size.height())/2.f, sf::Vector2f(size.width(), size.height()));
        m_window->setView(view2);
    }
    else
    {
        m_window->setSize(m_settings.layout.fixedSize);
        QWidget::setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
    }
}

void QSFML_Canvas::draw(const sf::Drawable &drawable,
                        const sf::RenderStates &states)
{
    if(!m_window) return;
    m_window->draw(drawable,states);
}


void QSFML_Canvas::draw(const sf::Vertex *vertices,
                        std::size_t vertexCount,
                        sf::PrimitiveType type,
                        const sf::RenderStates &states)
{
    if(!m_window) return;
    m_window->draw(vertices,vertexCount,type,states);
}


void QSFML_Canvas::draw(const sf::VertexBuffer &vertexBuffer,
                        const sf::RenderStates &states)
{
    if(!m_window) return;
    m_window->draw(vertexBuffer,states);
}


void QSFML_Canvas::draw(const sf::VertexBuffer &vertexBuffer,
                        std::size_t firstVertex,
                        std::size_t vertexCount,
                        const sf::RenderStates &states)
{
    if(!m_window) return;
    m_window->draw(vertexBuffer,firstVertex,vertexCount,states);
}



void QSFML_Canvas::sfEvent(const sf::Event &){}
void QSFML_Canvas::internal_event(const sf::Event &e)
{
    CanvasObjectContainer::sfEvent(e);
    /*if (e.type == sf::Event::MouseWheelMoved)
    {
        qDebug() << "wheel movement: " << e.mouseWheel.delta;
        qDebug() << "mouse x: " << e.mouseWheel.x;
        qDebug() << "mouse y: " << e.mouseWheel.y;
    }*/
}
