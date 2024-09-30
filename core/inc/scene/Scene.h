#pragma once

#include "QSFML_EditorWidget_base.h"
#include "scene/SceneSettings.h"
#include "scene/GameObjectContainer.h"

#include "utilities/Timer.h"
#include "utilities/Stats.h"
#include "SceneLogger.h"

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>


namespace QSFML
{

class QSFML_EDITOR_WIDGET_EXPORT Scene :
        public QWidget,
        public Utilities::StatsManager,
        public GameObjectContainer
    {
        Q_OBJECT
        friend GameObjectContainer;
    public:


        Scene(QWidget* parent, const SceneSettings &settings = SceneSettings());

        virtual ~Scene();

        void applyObjectChanges(); // Will be called automaticly if not manual

        void setSettings(const SceneSettings &settings);
        const SceneSettings &getSettings() const;
        void setLayout(const SceneSettings::Layout &layout);
        const SceneSettings::Layout &getLayout() const;
        void setTiming(const SceneSettings::Timing &timing);
        const SceneSettings::Timing &getTiming() const;
        void setContextSettings(const sf::ContextSettings &contextSettings);
        const sf::ContextSettings &getContextSettings() const;
        void setUpdateControlls(const SceneSettings::UpdateControlls &controlls);
        const SceneSettings::UpdateControlls &getUpdateControlls() const;
        void setColorSettings(const SceneSettings::Colors& colors);
        const SceneSettings::Colors& getColorSettings() const;

        void start();
        void stop();

        void setCameraView(const sf::View &view);
        const sf::View &getCameraView() const;
        const sf::View &getDefaultCameraView() const;
        Utilities::AABB getCameraViewRect() const;
        sf::Vector2u getSceneSize() const;
        sf::Vector2u getOldSceneSize() const;
        sf::Vector2f getViewCenterPosition() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(sf::Vector2i pixelSpace);
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace);

        

        void sfEvent(const std::vector<sf::Event> &events);

        static const sf::Font& getDefaultTextFont();

        static void setProfilerOutputFileName(const std::string& fileName);
        static const std::string& getProfilerOutputFileName();
        static void saveProfilerFile();
        static void saveProfilerFile(const std::string &fileName);

        size_t getTick() const; // returns the count of updates done
        double getDeltaT() const; // Gets delta Time in seconds
        double getElapsedTime() const; // Gets elapsed Time in seconds
        double getFixedDeltaT() const; // Gets fixed delta Time in seconds
        double getFixedElapsedTime() const; // Gets fixed elapsed Time in seconds
        double getFPS() const;
        double getTPS() const;

        Log::LogObject& getSceneLogger() { return m_logger.getSceneLogger(); }
        Log::LogObject& getObjectLogger() { return m_logger.getObjectLogger(); }
        Log::LogObject& getComponentLogger() { return m_logger.getComponentLogger(); }
        Log::LogObject& getPhysicsLogger() { return m_logger.getPhysicsLogger(); }

    protected:
        QPaintEngine* paintEngine() const override;
        void showEvent(QShowEvent*) override;
        void closeEvent(QCloseEvent*) override;
        void paintEvent(QPaintEvent*) override;

        void resizeEvent(QResizeEvent *event) override;



        virtual void OnInit();
        virtual void OnUpdate();

    private slots:
        
        void update();
        void checkEvents();
        void updateObjects();
        void paint();


    private:

        void internal_event(const std::vector<sf::Event> &events);



        QTimer m_frameTimer;
        TimePoint m_syncedUpdateT_t1;
        TimePoint m_update_t1;
        TimePoint m_paint_t1;


        SceneSettings m_settings;
        sf::RenderWindow *m_window;
        sf::Vector2u m_oldSceneSize;
        sf::View m_view;
        sf::Vector2f m_dpiScale;

        Internal::SceneLogger m_logger;


        static std::string m_profilerOutputFile;
        static std::vector<Scene*> s_instances;

    };
}
