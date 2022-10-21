#include "Canvas.h"
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QDebug>
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

using namespace QSFML;

Canvas::Canvas(QWidget* parent, const CanvasSettings &settings) :
  QWidget(parent),
 // DrawInterface(),
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
Canvas::~Canvas()
{
    delete m_window;
}

void Canvas::setSettings(const CanvasSettings &settings)
{
    setLayout(settings.layout);
    setTiming(settings.timing);
    setContextSettings(settings.contextSettings);

    if(!m_textfont.loadFromFile(settings.fontPath))
    {
        qDebug() << "Can't load Font: "<<settings.fontPath.c_str();
    }
}
const CanvasSettings &Canvas::getSettings() const
{
    return m_settings;
}
void Canvas::setLayout(const CanvasSettings::Layout &layout)
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
const CanvasSettings::Layout Canvas::getLayout() const
{
    return m_settings.layout;
}
void Canvas::setTiming(const CanvasSettings::Timing &timing)
{
    m_settings.timing = timing;

    // Setup the timer
    m_frameTimer.setInterval(m_settings.timing.frameTime);
}
const CanvasSettings::Timing &Canvas::getTiming() const
{
    return m_settings.timing;
}
void Canvas::setContextSettings(const sf::ContextSettings &contextSettings)
{
    m_settings.contextSettings = contextSettings;
}
const sf::ContextSettings &Canvas::getContextSettings() const
{
    return m_settings.contextSettings;
}

void Canvas::setCameraView(const sf::View &view)
{
    if(!m_window) return;
    m_view = view;
    m_window->setView(m_view);
}
const sf::View &Canvas::getCameraView() const
{
    static sf::View dummy;
    if(!m_window) return dummy;
    return m_view;
    //return m_window->getView();
}
const sf::View &Canvas::getDefaultCameraView() const
{
    static sf::View dummy;
    if(!m_window) return dummy;
    return m_window->getDefaultView();
}
sf::Vector2u Canvas::getCanvasSize() const
{
    return m_window->getSize();
}
sf::Vector2u Canvas::getOldCanvasSize() const
{
    return m_oldCanvasSize;
}

sf::Vector2i Canvas::getMousePosition() const
{
    if(m_window)
        return sf::Mouse::getPosition(*m_window);
    return sf::Mouse::getPosition();
}
sf::Vector2f Canvas::getMouseWorldPosition() const
{
    if(!m_window) return sf::Vector2f(0,0);
    sf::Vector2i pixelPos = getMousePosition();
    return m_window->mapPixelToCoords(pixelPos);
}
sf::Vector2f Canvas::getInWorldSpace(const sf::Vector2i &pixelSpace)
{
    if(!m_window) return sf::Vector2f(0,0);
    return m_window->mapPixelToCoords(pixelSpace);
}
sf::Vector2i Canvas::getInScreenSpace(const sf::Vector2f &worldSpace)
{
    if(!m_window) return sf::Vector2i(0,0);
    return m_window->mapCoordsToPixel(worldSpace);

}

void Canvas::OnInit()
{

}

void Canvas::OnUpdate()
{

}

QPaintEngine* Canvas::paintEngine() const
{
    return nullptr;
}
void Canvas::showEvent(QShowEvent*)
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
         //   m_window->setSize(sf::Vector2u(QWidget::geometry().size().width(),QWidget::geometry().size().height()));
        m_view = m_window->getView();
        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&m_frameTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        m_frameTimer.start();
        m_oldCanvasSize = getCanvasSize();
    }
}
void Canvas::paintEvent(QPaintEvent*)
{
    if(!m_window)return;
    EASY_FUNCTION(profiler::colors::Green); // Magenta block with name "foo"
    // Let the derived class do its specific stuff
    OnUpdate();

    EASY_BLOCK("Delete unused objects",profiler::colors::Green200);
    CanvasObjectContainer::updateNewElements();
    EASY_END_BLOCK;

    sf::Event event;

    EASY_BLOCK("Process sf::Events",profiler::colors::Green200);

    std::vector<sf::Event> events;
    events.reserve(20);
    while (m_window->pollEvent(event))
    {
        events.push_back(event);
    }
    sfEvent(events);
    internal_event(events);
    EASY_END_BLOCK;

    EASY_BLOCK("Process update",profiler::colors::Green400);
    CanvasObjectContainer::update();
    EASY_END_BLOCK;

    EASY_BLOCK("Process draw",profiler::colors::Green500);
    m_window->clear(m_settings.colors.defaultBackground);
    CanvasObjectContainer::draw(*m_window);
    EASY_END_BLOCK;

    EASY_BLOCK("Process display",profiler::colors::Green600);
    // Display on screen
    m_window->display();
    EASY_END_BLOCK;
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    if(!m_window) return;
    if(m_settings.layout.autoAjustSize)
    {
       QSize size = event->size();

       m_oldCanvasSize = getCanvasSize();
       //setCameraView(getCameraView());
       //m_window->setSize(sf::Vector2u(QWidget::geometry().size().width(),QWidget::geometry().size().height()));
       //m_window->setSize(sf::Vector2u(size.width(),size.height()));
       //m_window->setSize(sf::Vector2u(m_oldCanvasSize.x,m_oldCanvasSize.y));
      /* sf::View view = getCameraView();

       sf::Vector2u oldWindowSize(event->oldSize().width(),event->oldSize().height());
       sf::Vector2u newWindowSize(size.width(),size.height());
       sf::FloatRect viewRect = sf::FloatRect(view.getCenter()-view.getSize()/2.f,view.getSize());
       //viewPort.width = size.width()
       //sf::View view2(sf::Vector2f(size.width(), size.height())/2.f, sf::Vector2f(size.width(), size.height()));
       //m_window->setView(view2);

       viewRect.width = viewRect.width/oldWindowSize.x * newWindowSize.x;
       viewRect.height = viewRect.height/oldWindowSize.y * newWindowSize.y;

       view.setSize(viewRect.width,viewRect.height);
       view.setCenter(viewRect.left+viewRect.width/2.f,viewRect.top+viewRect.height/2.f);
       m_window->setView(view);*/
    }
    else
    {
        m_window->setSize(m_settings.layout.fixedSize);
        QWidget::setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
    }
}



void Canvas::sfEvent(const std::vector<sf::Event> &events){}
void Canvas::internal_event(const std::vector<sf::Event> &events)
{
    CanvasObjectContainer::sfEvent(events);
}
const sf::Font &Canvas::getTextFont() const
{
    return m_textfont;
}
