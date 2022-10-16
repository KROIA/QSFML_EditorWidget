#pragma once

#include <QObject>
#include "SfEventHandle.h"

namespace QSFML
{
namespace Components
{

class MouseFollower : public QObject, public SfEventHandle
{
        Q_OBJECT
    public:
    MouseFollower(const std::string &name = "MouseFollower");

    void sfEvent(const sf::Event &e) override;


    signals:
    void mousePosChanged(const sf::Vector2f &worldPos,
                         const sf::Vector2i &pixelPos);
};
}
}
