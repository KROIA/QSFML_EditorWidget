#pragma once

#include <SFML/Graphics.hpp>

class CameraInterface
{
    public:
        virtual void zoomCamera(float zoomValue) = 0;
        virtual void setCameraZoom(float zoomValue) = 0;
        virtual float getCameraZoom() const = 0;

        virtual void moveCamera(const sf::Vector2f &worldCoords) = 0;
        virtual void setCameraPosition(const sf::Vector2f &worldCoords) = 0;
        virtual const sf::Vector2f &getCameraPosition() const = 0;

        virtual void setCameraView(const sf::View &view) = 0;
        virtual const sf::View &getCameraView() const = 0;

};
