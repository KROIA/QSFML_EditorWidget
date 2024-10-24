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

#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
#include <imgui-SFML.h>
#include <imgui.h>
#endif


namespace QSFML
{

class QSFML_EDITOR_WIDGET_EXPORT Scene :
        public QObject,
        public Utilities::StatsManager,
        public GameObjectContainer
    {
    Q_OBJECT
        friend GameObjectContainer;
        friend Objects::CameraWindow;
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
        Utilities::AABB getViewRect() const;
        sf::Vector2u getCameraSize() const;
        sf::Vector2u getOldCameraSize() const;
        sf::Vector2f getViewCenterPosition() const;
		const QSFML::vector<sf::Event>& getEvents() const;
		const QSFML::vector<sf::Event>& getEvents(Objects::CameraWindow *camera) const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(sf::Vector2i pixelSpace);
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace);

        
		sf::Image captureScreen();
        void captureScreen(sf::Image& image);
        //void sfEvent(const QSFML::vector<sf::Event> &events);

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

        Objects::CameraWindow* getDefaultCamera() { return m_cameras.defaultCamera; }
        Objects::CameraWindow* getCurrentRenderCamera() { return m_cameras.currentRenderingCamera; }
    
        void paint(Objects::CameraWindow* currentCamera);
    protected:
        //QPaintEngine* paintEngine() const override;
        //void showEvent(QShowEvent*) override;
        //void closeEvent(QCloseEvent*) override;
        //void paintEvent(QPaintEvent*) override;
        //
        //void resizeEvent(QResizeEvent *event) override;



        virtual void OnInit();
        virtual void OnUpdate();

    //private slots:
        
        //void firstUpdate();
        void update();
        void checkEvents();
        void updateObjects();
        void paint();
		


    private:
        void onCameraWindowClose(Objects::CameraWindow* window);

        void internal_event(const QSFML::unordered_map<Objects::CameraWindow*, QSFML::vector<sf::Event>> &events);



        QTimer m_frameTimer;
        TimePoint m_syncedUpdateT_t1;
        TimePoint m_update_t1;
        TimePoint m_paint_t1;

#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
		sf::Clock m_imguiClock;
        ImGuiContext* m_imGuiContext = nullptr;
        void applyImGuiEventFilter(QSFML::vector<sf::Event>& events);
#endif


        SceneSettings m_settings;
        //sf::RenderWindow *m_window;
        //sf::Vector2u m_oldSceneSize;
        //sf::View m_view;
        //sf::Vector2f m_dpiScale;

        Internal::SceneLogger m_logger;


		 
        struct Cameras
		{
            Objects::CameraWindow* defaultCamera = nullptr;
            Objects::CameraWindow* currentRenderingCamera = nullptr;
			QSFML::vector<Objects::CameraWindow*> cameras;
			QSFML::unordered_map<Objects::CameraWindow*, Objects::CameraWindow*> cameraMap;
		
            bool hasCamera(Objects::CameraWindow* camera) const
            {
				return cameraMap.find(camera) != cameraMap.end();
            }
            void addCamera(Objects::CameraWindow* camera)
            {
                if (!hasCamera(camera))
                {
                    cameras.push_back(camera);
                    cameraMap[camera] = camera;
                }
            }
			void removeCamera(Objects::CameraWindow* camera)
			{
				if (hasCamera(camera))
				{
					cameras.erase(std::remove(cameras.begin(), cameras.end(), camera), cameras.end());
					cameraMap.erase(camera);
				}
				if (camera == defaultCamera)
				{
					defaultCamera = nullptr;
				}
			}
			void setDefaultCamera(Objects::CameraWindow* camera)
			{
                defaultCamera = camera;
                if (!hasCamera(camera))
                {
					addCamera(camera);
                }
			}
        };
        Cameras m_cameras;

        static std::string m_profilerOutputFile;
        static QSFML::vector<Scene*> s_instances;

    };
}
