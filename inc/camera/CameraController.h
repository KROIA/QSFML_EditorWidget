#pragma once

#include <SFML/Graphics.hpp>
#include "CanvasObject.h"
#include "CameraInterface.h"
#include <QDebug>

class CameraController: virtual public CanvasObject
{
    public:
        CameraController(CanvasObject *parent = nullptr);
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

        void setView(const sf::View &view);
        const sf::View getView();

        void sfEvent(const sf::Event &e) override;

        void internalOnCanvasParentChange(QSFML_Canvas *newParent) override;
        void internalOnParentChange(CanvasObject *newParent) override;
    private:
        void positionCheck(sf::View &view);


        //CameraInterface *m_interface;
        float m_currentZoom;
        float m_minZoom;
        float m_maxZoom;
        sf::FloatRect m_maxMovingBounds;
};
