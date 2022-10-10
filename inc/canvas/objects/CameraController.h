#pragma once

#include <SFML/Graphics.hpp>
#include "CanvasObject.h"
#include "SfEventHandle.h"
#include <QDebug>

namespace QSFML
{
namespace Objects
{

class CameraController: public CanvasObject
{
        class SfEventComponent;
    public:
        CameraController(const std::string &name = "",
                         CanvasObject *parent = nullptr);
        ~CameraController();

        void setMinZoom(float min);
        void setMaxZoom(float max);
        float getMinZoom()const;
        float getMaxZoom()const;

        void setMaxMovingBounds(const sf::FloatRect &rect);
        const sf::FloatRect &getMaxMovingBounds() const;

        void moveLeft(float amount);
        void moveRight(float amount);
        void moveUp(float amount);
        void moveDown(float amount);
        void movePosition(const sf::Vector2f &delta);
        void setPosition(const sf::Vector2f &pos);

        void rotate(float angle);
        void setRotation(float angle);

        void zoom(float amount);
        void zoom(float amount, const sf::Vector2i &pixel);
        void setZoom(float amount);
        void setZoom(float amount, const sf::Vector2i &pixel);

        void setCameraView(const sf::View &view);





    private:
        void internalOnCanvasParentChange(Canvas *newParent) override;
        void internalOnParentChange(CanvasObject *newParent) override;
        void positionCheck(sf::View &view);

        float m_currentZoom;
        float m_minZoom;
        float m_maxZoom;
        sf::FloatRect m_maxMovingBounds;

        SfEventComponent *m_eventHandleComponent;

    // Defining component
    class SfEventComponent : public Components::SfEventHandle
    {
        public:
            SfEventComponent(){}
            ~SfEventComponent(){}

            void setController(CameraController *controller);
            void sfEvent(const sf::Event &e) override;

        private:
            CameraController *m_controller;
    };
};
}
}
