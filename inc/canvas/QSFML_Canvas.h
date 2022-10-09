#pragma once

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include "CanvasSettings.h"
#include "CameraController.h"
#include "CanvasObject.h"
#include "CanvasObjectContainer.h"


class QSFML_Canvas :
        public QWidget,
        public CanvasObjectContainer
{
    public :

        QSFML_Canvas(QWidget* parent, const CanvasSettings &settings = CanvasSettings());

        virtual ~QSFML_Canvas();


        void setSettings(const CanvasSettings &settings);
        const CanvasSettings &getSettings() const;
        void setLayout(const CanvasSettings::Layout &layout);
        const CanvasSettings::Layout getLayout() const;
        void setTiming(const CanvasSettings::Timing &timing);
        const CanvasSettings::Timing &getTiming() const;
        void setContextSettings(const sf::ContextSettings &contextSettings);
        const sf::ContextSettings &getContextSettings() const;

        void setCameraView(const sf::View &view);
        const sf::View &getCameraView() const;
        const sf::View &getDefaultCameraView() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i &pixelSpace);
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace);


        void sfEvent(const std::vector<sf::Event> &events);

    protected:
        QPaintEngine* paintEngine() const override;
        void showEvent(QShowEvent*) override;
        void paintEvent(QPaintEvent*) override;

        void resizeEvent(QResizeEvent *event) override;


    private:

        virtual void OnInit();
        virtual void OnUpdate();

        void internal_event(const std::vector<sf::Event> &events);



    QTimer m_frameTimer;

    CanvasSettings m_settings;
    sf::RenderWindow *m_window;
};
