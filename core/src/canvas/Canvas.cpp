#include "canvas/Canvas.h"
#include "utilities/Stats.h"
#include "utilities/AABB.h"
#include <QResizeEvent>
#include <QHBoxLayout>
#include <qapplication.h>

/*
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif*/

using namespace QSFML;


std::vector<Canvas*> Canvas::s_instances;
//bool Canvas::s_execEventLoop = false;
std::string Canvas::m_profilerOutputFile = "profile.prof";

Canvas::Canvas(QWidget* parent, const CanvasSettings &settings) :
  QWidget(parent),
  CanvasObjectContainer(this, settings)
{
    s_instances.push_back(this);
    StatsManager::createStats(this);
#ifdef QSFML_PROFILING
    if(s_instances.size() == 1)
    {
        EASY_PROFILER_ENABLE;
    }
#endif
    m_window = nullptr;
    m_deltaT = 0;
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

    //m_updateTimer.onFinished(std::bind(&Canvas::update, this));
    connect(&m_frameTimer, &QTimer::timeout, this, &Canvas::update);
    setSettings(settings);

    
}
Canvas::~Canvas()
{
    delete m_window;
#ifdef QSFML_PROFILING
    if (s_instances.size() == 1)
    {
        saveProfilerFile();
    }
#endif
    for (size_t i = 0; i < s_instances.size(); ++i)
    {
        if (s_instances[i] == this)
        {
            s_instances.erase(s_instances.begin() + i);
        }
    }
   // if (s_instances.size() == 0)
   //     stopEventLoop();
}



void Canvas::setSettings(const CanvasSettings &settings)
{
    setLayout(settings.layout);
    setTiming(settings.timing);
    setContextSettings(settings.contextSettings);
    setUpdateControlls(settings.updateControlls);

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
    //m_updateTimer.setInterval(m_settings.timing.frameTime);
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
void Canvas::setUpdateControlls(const CanvasSettings::UpdateControlls &controlls)
{
    m_settings.updateControlls = controlls;
}
const CanvasSettings::UpdateControlls &Canvas::getUpdateControlls() const
{
    return m_settings.updateControlls;
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
}
const sf::View &Canvas::getDefaultCameraView() const
{
    static sf::View dummy;
    if(!m_window) return dummy;
    return m_window->getDefaultView();
}
Utilities::AABB Canvas::getCameraViewRect() const
{
    return Utilities::AABB(m_view.getCenter() - m_view.getSize() * 0.5f, m_view.getSize());
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
float Canvas::getDeltaT() const
{
    return m_deltaT;
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
        
        m_view = m_window->getView();
        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        m_frameTimer.start(0);
        //m_updateTimer.autoRestart(true);
        //m_updateTimer.start();
        
        
        m_deltaT_t1 = std::chrono::high_resolution_clock::now();
        m_oldCanvasSize = getCanvasSize();
    }
}
void Canvas::paintEvent(QPaintEvent*)
{
   // update();
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    if(!m_window) return;
    if(m_settings.layout.autoAjustSize)
    {
       QSize size = event->size();
       m_oldCanvasSize = getCanvasSize();
    }
    else
    {
        m_window->setSize(m_settings.layout.fixedSize);
        QWidget::setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
    }
}

void Canvas::update()
{
    if(!m_window)
        return;
    QSFMLP_FUNCTION(QSFMLP_CANVAS_COLOR_1); // Magenta block with name "foo"


    TimePoint t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = t2 - m_deltaT_t1;
    m_deltaT_t1 = t2;
    m_deltaT = elapsed.count();
    StatsManager::resetCollisionStats(this);

    QSFMLP_BLOCK("Delete unused objects", QSFMLP_CANVAS_COLOR_2);
    CanvasObjectContainer::updateNewElements();
    QSFMLP_END_BLOCK;

    sf::Event event;
    if(m_settings.updateControlls.enableEventLoop)
    {
        QSFMLP_BLOCK("Process sf::Events", QSFMLP_CANVAS_COLOR_3);

        std::vector<sf::Event> events;
        events.reserve(20);
        while (m_window->pollEvent(event))
        {
            events.push_back(event);
        }
        sfEvent(events);
        internal_event(events);
        QSFMLP_END_BLOCK;
    }

    if(m_settings.updateControlls.enableUpdateLoop)
    {
        QSFMLP_BLOCK("Process update", QSFMLP_CANVAS_COLOR_4);
        // Let the derived class do its specific stuff
        OnUpdate();

        CanvasObjectContainer::update();
        QSFMLP_END_BLOCK;
    }

    if(m_settings.updateControlls.enablePaintLoop)
    {
        QSFMLP_BLOCK("Clear Display", QSFMLP_CANVAS_COLOR_5);
        m_window->clear(m_settings.colors.defaultBackground);
        QSFMLP_END_BLOCK;

        QSFMLP_BLOCK("Process draw", QSFMLP_CANVAS_COLOR_6);
        CanvasObjectContainer::draw(*m_window);
        QSFMLP_END_BLOCK;

        QSFMLP_BLOCK("Process Display", QSFMLP_CANVAS_COLOR_7);
        // Display on screen
        m_window->display();
        QSFMLP_END_BLOCK;
    }
}
/*void Canvas::timedUpdate()
{
    m_updateTimer.update();
}*/


void Canvas::sfEvent(const std::vector<sf::Event> &events)
{

}
void Canvas::internal_event(const std::vector<sf::Event> &events)
{
    CanvasObjectContainer::sfEvent(events);
}
const sf::Font &Canvas::getTextFont() const
{
    return m_textfont;
}
void Canvas::setProfilerOutputFileName(const std::string& fileName)
{
    m_profilerOutputFile = fileName;
}
const std::string& Canvas::getProfilerOutputFileName()
{
    return m_profilerOutputFile;
}
void Canvas::saveProfilerFile()
{
#ifdef QSFML_PROFILING
    profiler::dumpBlocksToFile(m_profilerOutputFile.c_str());
#endif
}
void Canvas::saveProfilerFile(const std::string& fileName)
{
    setProfilerOutputFileName(fileName);
    saveProfilerFile();
}

/*void Canvas::startEventLoop()
{
    s_execEventLoop = true;
    while (s_execEventLoop)
    {
        QSFMLP_BLOCK("QApplication::processEvents", QSFMLP_CANVAS_COLOR_1);
        QApplication::processEvents();
        QSFMLP_END_BLOCK;
        QSFMLP_BLOCK("Canvas::timedUpdate", QSFMLP_CANVAS_COLOR_1);
        for (auto canvas : s_instances)
            canvas->timedUpdate();
        QSFMLP_END_BLOCK;
        
    }
}
void Canvas::stopEventLoop()
{
    s_execEventLoop = false;
}*/
