#include "Scene/Scene.h"
#include "utilities/Stats.h"
#include "utilities/AABB.h"
#include "utilities/RandomEngine.h"
#include <QResizeEvent>
#include <QHBoxLayout>
#include <qapplication.h>
#include <thread>
#include <iostream>

#include "objects/CameraWindow.h"

#include <QDebug>
#include <SFML/OpenGL.hpp>

namespace QSFML {
    const static size_t defaultFontPathsSize = 7;
    const static std::string defaultFontPaths[defaultFontPathsSize]{
        "C:\\Windows\\Fonts\\Arial.ttf",
        "C:\\Program Files\\Common Files\\Microsoft Shared\\Fonts\\arial.ttf",
        "/usr/share/fonts/truetype/msttcorefonts/arial.ttf",
        "~/.fonts/arial.ttf",
        "/usr/share/fonts/truetype/msttcorefonts/arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "~/Library/Fonts/Arial.ttf"
    };

    std::vector<Scene*> Scene::s_instances;
    std::string Scene::m_profilerOutputFile = "profiler_output.prof";

    Scene::Scene(QWidget* parent, const SceneSettings &settings) 
        : QObject(parent)
        , StatsManager::StatsManager()
        , GameObjectContainer(this, m_settings)
    {
        s_instances.push_back(this);
        if (s_instances.size() == 1)
        {
            Utilities::RandomEngine::init();
#ifdef QSFML_PROFILING
            EASY_PROFILER_ENABLE;
#endif
        }
        //m_window = nullptr;
        // Setup layout of this widget
        /*if (parentWidget())
        {
            if (!parentWidget()->layout())
            {
                QHBoxLayout* layout = new QHBoxLayout(parentWidget());
                parentWidget()->setLayout(layout);
            }
            parentWidget()->layout()->addWidget(this);
        }

        // Setup some states to allow direct rendering into the widget
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);

        // Set strong focus to enable keyboard events to be received
        setFocusPolicy(Qt::StrongFocus);
        */
        //m_updateTimer.onFinished(std::bind(&Scene::update, this));
        connect(&m_frameTimer, &QTimer::timeout, this, &Scene::firstUpdate);
        

		m_cameras.defaultCamera = new Objects::CameraWindow(settings.contextSettings, "DefaultCamera", parent);
        m_cameras.defaultCamera->enableFrameTimer(false);
		addObject(m_cameras.defaultCamera);

		const auto geometry = m_cameras.defaultCamera->geometry();
        m_cameras.defaultCamera->setGeometry(geometry.x(), geometry.y(), settings.layout.fixedSize.x, settings.layout.fixedSize.y);
        setSettings(settings);

    }
    Scene::~Scene()
    {
        stop();

		// Crashes sometimes when the window is closed
        //if (parentWidget())
        //    parentWidget()->layout()->removeWidget(this);
        //m_window->close();
        //delete m_window;
        //m_window = nullptr;
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
        GameObjectContainer::cleanup();
    }


    void Scene::setSettings(const SceneSettings& settings)
    {
        setLayout(settings.layout);
        setTiming(settings.timing);
        setContextSettings(settings.contextSettings);
        setUpdateControlls(settings.updateControlls);
        setColorSettings(settings.colors);

        /*if (!m_textfont.loadFromFile(settings.fontPath))
        {
            qDebug() << "Can't load Font: " << settings.fontPath.c_str();
        }*/
    }
    const SceneSettings& Scene::getSettings() const
    {
        return m_settings;
    }
    void Scene::setLayout(const SceneSettings::Layout& layout)
    {
        m_settings.layout = layout;
        if (!m_cameras.defaultCamera)
            return;
        if (m_cameras.defaultCamera->parentWidget())
            m_cameras.defaultCamera->parentWidget()->layout()->setContentsMargins(m_settings.layout.margin.left,
                m_settings.layout.margin.top,
                m_settings.layout.margin.right,
                m_settings.layout.margin.bottom);
        if (!m_settings.layout.autoAjustSize)
        {
            m_cameras.defaultCamera->setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
        }
    }
    const SceneSettings::Layout &Scene::getLayout() const
	{
		return m_settings.layout;
	}
    

    void Scene::setTiming(const SceneSettings::Timing& timing)
    {
        m_settings.timing = timing;

        // Setup the timer
        m_frameTimer.setInterval(m_settings.timing.frameTime * 1000);
        StatsManager::setFixedDeltaT(m_settings.timing.physicsFixedDeltaT);
        //m_updateTimer.setInterval(m_settings.timing.frameTime);
    }
    const SceneSettings::Timing& Scene::getTiming() const
    {
        return m_settings.timing;
    }
    void Scene::setContextSettings(const sf::ContextSettings& contextSettings)
    {
        m_settings.contextSettings = contextSettings;
    }
    const sf::ContextSettings& Scene::getContextSettings() const
    {
        return m_settings.contextSettings;
    }
    void Scene::setUpdateControlls(const SceneSettings::UpdateControlls& controlls)
    {
        m_settings.updateControlls = controlls;
        if (m_settings.updateControlls.enableMultithreading)
            initializeThreads(m_settings.updateControlls.threadSettings.threadCount);
        else
            deinitializeThreads();
    }
    const SceneSettings::UpdateControlls& Scene::getUpdateControlls() const
    {
        return m_settings.updateControlls;
    }
    void Scene::setColorSettings(const SceneSettings::Colors& colors)
    {
        m_settings.colors = colors;
    }
    const SceneSettings::Colors& Scene::getColorSettings() const
    {
        return m_settings.colors;
    }

    void Scene::start()
    {
        m_frameTimer.start();
    }
    void Scene::stop()
    {
        m_frameTimer.stop();
        connect(&m_frameTimer, &QTimer::timeout, this, &Scene::firstUpdate);
        disconnect(&m_frameTimer, &QTimer::timeout, this, &Scene::update);
    }

    void Scene::setCameraView(const sf::View& view)
    {
        if (!m_cameras.defaultCamera) return;
        m_cameras.defaultCamera->setThisCameraView(view);
    }
    const sf::View& Scene::getCameraView() const
    {
        static sf::View dummy;
        if (!m_cameras.defaultCamera) return dummy;
        return m_cameras.defaultCamera->getThisCameraView();
    }
    const sf::View& Scene::getDefaultCameraView() const
    {
        static sf::View dummy;
        if (!m_cameras.defaultCamera) return dummy;
        return m_cameras.defaultCamera->getThisCameraDefaultView();
    }
    Utilities::AABB Scene::getViewRect() const
    {
        if(m_cameras.defaultCamera)
			return m_cameras.defaultCamera->getThisCameraViewRect();
		return Utilities::AABB();
    }
    sf::Vector2u Scene::getCameraSize() const
    {
		if (!m_cameras.defaultCamera) return sf::Vector2u(0, 0);
        return m_cameras.defaultCamera->getThisCameraSize();
    }
    sf::Vector2u Scene::getOldCameraSize() const
    {
		if (!m_cameras.defaultCamera) return sf::Vector2u(0, 0);
        return m_cameras.defaultCamera->getThisCameraOldSize();
    }
    sf::Vector2f Scene::getViewCenterPosition() const
    {
		if (!m_cameras.defaultCamera) return sf::Vector2f(0, 0);
        return m_cameras.defaultCamera->getThisCameraViewCenterPosition();
	}
    const std::vector<sf::Event>& Scene::getEvents() const
    {
        if (m_cameras.defaultCamera)
            return m_cameras.defaultCamera->getThisCameraEvents();

        static std::vector<sf::Event> dummy;
        return dummy;
    }
    const std::vector<sf::Event>& Scene::getEvents(Objects::CameraWindow* camera) const
    {
		if (m_cameras.hasCamera(camera))
			return camera->getThisCameraEvents();

		static std::vector<sf::Event> dummy;
        return dummy;
    }


    sf::Vector2i Scene::getMousePosition() const
    {
        if (!m_cameras.defaultCamera) return sf::Vector2i(0, 0);
        return m_cameras.defaultCamera->getThisCameraMousePosition();
    }
    sf::Vector2f Scene::getMouseWorldPosition() const
    {
        if (!m_cameras.defaultCamera) return sf::Vector2f(0, 0);
        return m_cameras.defaultCamera->getThisCameraMouseWorldPosition();
    }
    sf::Vector2f Scene::getInWorldSpace(sf::Vector2i pixelSpace)
    {
        if (!m_cameras.defaultCamera) return sf::Vector2f(0, 0);
        return m_cameras.defaultCamera->getInThisCameraWorldSpace(pixelSpace);
    }
    sf::Vector2i Scene::getInScreenSpace(const sf::Vector2f& worldSpace)
    {
        if (!m_cameras.defaultCamera) return sf::Vector2i(0, 0);
        return m_cameras.defaultCamera->getInThisCameraScreenSpace(worldSpace);
    }

    sf::Image Scene::captureScreen()
    {
        if (!m_cameras.defaultCamera) return sf::Image();
        return m_cameras.defaultCamera->captureThisCameraScreen();
    }
    void Scene::captureScreen(sf::Image& image)
    {
		if (!m_cameras.defaultCamera) return;
		m_cameras.defaultCamera->captureThisCameraScreen(image);
    }


    void Scene::OnInit()
    {

    }

    void Scene::OnUpdate()
    {

    }
    /*
    QPaintEngine* Scene::paintEngine() const
    {
        return nullptr;
    }

    void Scene::showEvent(QShowEvent*)
    {
        if (!m_window)
        {
            // Under X11, we need to flush the commands sent to the server to ensure that
            // SFML will get an updated view of the windows
#ifdef Q_WS_X11
            XFlush(QX11Info::display());
#endif

            // Create the SFML window with the widget handle
            m_window = new sf::RenderWindow((sf::WindowHandle)QWidget::winId(), m_settings.contextSettings);

            m_view = m_window->getView();
            // Let the derived class do its specific stuff
            OnInit();

            // Setup the timer to trigger a refresh at specified framerate
            start();
            //m_updateTimer.autoRestart(true);
            //m_updateTimer.start();


            m_syncedUpdateT_t1 = std::chrono::high_resolution_clock::now();
            m_update_t1 = m_syncedUpdateT_t1;
            m_paint_t1 = m_syncedUpdateT_t1;
            m_oldSceneSize = getSceneSize();

            // Calculate the dpi scale
            QRect geometry = QWidget::geometry();
            m_dpiScale.x = (float)m_oldSceneSize.x / geometry.width();
            m_dpiScale.y = (float)m_oldSceneSize.y / geometry.height();
        }
    }
    void Scene::closeEvent(QCloseEvent*)
    {
        stop();
    }
    void Scene::paintEvent(QPaintEvent*)
    {

    }

    void Scene::resizeEvent(QResizeEvent* event)
    {
        if (!m_window) return;
        if (m_settings.layout.autoAjustSize)
        {
            QSize size = event->size();
            m_oldSceneSize = getSceneSize();
        }
        else
        {
            m_window->setSize(m_settings.layout.fixedSize);
            QWidget::setFixedSize(m_settings.layout.fixedSize.x, m_settings.layout.fixedSize.y);
        }
    }*/

    void Scene::firstUpdate()
    {
        disconnect(&m_frameTimer, &QTimer::timeout, this, &Scene::firstUpdate);
        connect(&m_frameTimer, &QTimer::timeout, this, &Scene::update);
        m_syncedUpdateT_t1 = std::chrono::high_resolution_clock::now();
        m_update_t1 = m_syncedUpdateT_t1;
        m_paint_t1 = m_syncedUpdateT_t1;
        update();
    }
    void Scene::update()
    {
        if (!m_cameras.defaultCamera)
            return;
        QSFMLP_SCENE_FUNCTION(QSFML_COLOR_STAGE_1);

        TimePoint t2 = std::chrono::high_resolution_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(t2 - m_syncedUpdateT_t1).count();

        m_syncedUpdateT_t1 = t2;
        StatsManager::resetFrame_synced();
        StatsManager::setFrameTime(elapsedSeconds);

        applyObjectChanges();

        if (m_settings.updateControlls.enableEventLoop)
        {
            checkEvents();
        }

        if (m_settings.updateControlls.enableUpdateLoop)
        {
            updateObjects();
        }

        if (m_settings.updateControlls.enablePaintLoop)
        {
            paint();
        }
    }
    void Scene::applyObjectChanges()
    {
        QSFMLP_SCENE_BLOCK("Delete unused objects and add new objects", QSFML_COLOR_STAGE_2);
        GameObjectContainer::applyObjectChanges();
        StatsManager::copyObjectCounts();
        QSFMLP_SCENE_END_BLOCK;
    }
    void Scene::checkEvents()
    {
        QSFMLP_SCENE_BLOCK("Process sf::Events", QSFML_COLOR_STAGE_3);
        TimePoint t1 = std::chrono::high_resolution_clock::now();
        StatsManager::resetFrame_eventloop();
        
        std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>> events;
        for(auto camera : m_cameras.cameras)
		{
            camera->pollEvents();
            const std::vector<sf::Event> &cameraEvents = camera->getThisCameraEvents();
            if (cameraEvents.size() > 0)
			    events[camera] = cameraEvents;
		}
        
        if(events.size() > 0)
            internal_event(events);
        TimePoint t2 = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(t2 - t1).count();
        StatsManager::setEventTime(elapsed);
        QSFMLP_SCENE_END_BLOCK;
    }
    void Scene::updateObjects()
    {
        QSFMLP_SCENE_BLOCK("Process update", QSFML_COLOR_STAGE_4);
        TimePoint t1 = std::chrono::high_resolution_clock::now();
        StatsManager::resetFrame_updateLoop();
        double elapsedSeconds = std::chrono::duration<double>(t1 - m_update_t1).count();
        if (elapsedSeconds > 0)
            StatsManager::setTPS(1.0 / elapsedSeconds);
        else
            StatsManager::setTPS(9999999);
        m_update_t1 = t1;
        StatsManager::setDeltaT(elapsedSeconds * m_settings.timing.physicsDeltaTScale);

        // Let the derived class do its specific stuff
        OnUpdate();

        GameObjectContainer::update();
        TimePoint t2 = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(t2 - t1).count();
        StatsManager::setUpdateTime(elapsed);
        StatsManager::addTick();
        QSFMLP_SCENE_END_BLOCK;
    }
    void Scene::paint()
    {
        QSFMLP_SCENE_BLOCK("paint", QSFML_COLOR_STAGE_5);
        TimePoint t1 = std::chrono::high_resolution_clock::now();
        StatsManager::resetFrame_paintLoop();
        double elapsedSeconds = std::chrono::duration<double>(t1 - m_paint_t1).count();
        if (elapsedSeconds > 0)
            StatsManager::setFPS(1.0 / elapsedSeconds);
        else
            StatsManager::setFPS(9999999);

        m_paint_t1 = t1;

		if (m_cameras.defaultCamera->isOpen())
		    paint(*m_cameras.defaultCamera->getRenderWindow());
		/*for (auto camera : m_cameras)
		{
			QSFMLP_SCENE_BLOCK("Repaint camera", QSFML_COLOR_STAGE_5);
			QSFMLP_SCENE_TEXT("Camera", camera->getName().c_str());
            QSFMLP_SCENE_BLOCK("Clear Display", QSFML_COLOR_STAGE_6);
			camera->getRenderWindow()->clear(m_settings.colors.defaultBackground);
            QSFMLP_SCENE_END_BLOCK;
            QSFMLP_SCENE_BLOCK("Process draw", QSFML_COLOR_STAGE_7);
			GameObjectContainer::draw(*camera->getRenderWindow());
            QSFMLP_SCENE_END_BLOCK;
            QSFMLP_SCENE_BLOCK("Process Display", QSFML_COLOR_STAGE_8);
			camera->getRenderWindow()->display();
            QSFMLP_SCENE_END_BLOCK;
		}*/

        /*

        QSFMLP_SCENE_BLOCK("Process draw", QSFML_COLOR_STAGE_6);
        GameObjectContainer::draw(*m_window);
        QSFMLP_SCENE_END_BLOCK;

        QSFMLP_SCENE_BLOCK("Process Display", QSFML_COLOR_STAGE_7);
        // Display on screen
        m_window->display();*/
        TimePoint t2 = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(t2 - t1).count();
        StatsManager::setDrawTime(elapsed);
        QSFMLP_SCENE_END_BLOCK;
    }
    void Scene::paint(sf::RenderWindow& target)
    {
        QSFMLP_SCENE_BLOCK("Repaint camera", QSFML_COLOR_STAGE_5);

        // Set the viewport
        target.setActive(true);

        // In case a user only draws using gl calls, we need to set the viewport first.
        // SFML would handle this if sfml draw calls are used.
		const sf::View& view = target.getView();
        sf::IntRect viewport = target.getViewport(view);
        int top = static_cast<int>(target.getSize().y) - (viewport.top + viewport.height);
        glViewport(viewport.left, top, viewport.width, viewport.height);

        // Set the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(view.getTransform().getMatrix());

        // Go back to model-view mode
        glMatrixMode(GL_MODELVIEW);

        //QSFMLP_SCENE_TEXT("Camera", camera->getName().c_str());
        QSFMLP_SCENE_BLOCK("Clear Display", QSFML_COLOR_STAGE_6);
        target.clear(m_settings.colors.defaultBackground);
        QSFMLP_SCENE_END_BLOCK;
        QSFMLP_SCENE_BLOCK("Process draw", QSFML_COLOR_STAGE_7);
        GameObjectContainer::draw(target);
        QSFMLP_SCENE_END_BLOCK;
        QSFMLP_SCENE_BLOCK("Process Display", QSFML_COLOR_STAGE_8);
        target.display();
        target.setActive(false);
        QSFMLP_SCENE_END_BLOCK;
    }

    //void Scene::sfEvent(const std::vector<sf::Event>& events)
    //{
    //    QSFML_UNUSED(events);
    //}
    const sf::Font& Scene::getDefaultTextFont()
    {
        static sf::Font textfont;
        static bool isLoaded = false;
        while (!isLoaded)
        {
            static size_t counter = 0;
            isLoaded = textfont.loadFromFile(defaultFontPaths[counter]);
            if (!isLoaded)
            {
                qDebug() << "Can't load Font: " << defaultFontPaths[counter].c_str();
            }
            counter++;
            if (counter >= defaultFontPathsSize)
            {
                counter = 0;
                break;
            }
        }
        return textfont;
    }
    void Scene::internal_event(const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>& events)
    {
        GameObjectContainer::sfEvent(events);
    }
    void Scene::setProfilerOutputFileName(const std::string& fileName)
    {
        m_profilerOutputFile = fileName;
    }
    const std::string& Scene::getProfilerOutputFileName()
    {
        return m_profilerOutputFile;
    }
    void Scene::saveProfilerFile()
    {
#ifdef QSFML_PROFILING
        profiler::dumpBlocksToFile(m_profilerOutputFile.c_str());
#endif
    }
    void Scene::saveProfilerFile(const std::string& fileName)
    {
        setProfilerOutputFileName(fileName);
        saveProfilerFile();
    }


    size_t Scene::getTick() const
    {
        return m_currentStats.getTick();
    }
    double Scene::getDeltaT() const
    {
        return m_currentStats.getDeltaT();
    }
    double Scene::getElapsedTime() const
    {
        return m_currentStats.getElapsedTime();
    }
    double Scene::getFixedElapsedTime() const
    {
        return m_currentStats.getFiexedElapsedTime();
    }
    double Scene::getFixedDeltaT() const
    {
        return m_settings.timing.physicsFixedDeltaT;
    }
    double Scene::getFPS() const
    {
        return m_currentStats.getFPS();
    }
    double Scene::getTPS() const
    {
        return m_currentStats.getTPS();
    }
}