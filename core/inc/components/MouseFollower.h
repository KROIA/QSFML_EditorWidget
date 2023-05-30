#pragma once

#include "QSFML_base.h"
#include "components/base/SfEventHandle.h"

#include <QObject>

namespace QSFML
{
namespace Components
{

class QSFML_EDITOR_WIDGET_EXPORT MouseFollower : public QObject, public SfEventHandle
{
        Q_OBJECT
    public:
    MouseFollower(const std::string &name = "MouseFollower");
    MouseFollower(const MouseFollower &other);
    COMPONENT_DECL(MouseFollower);

    void sfEvent(const sf::Event &e) override;


    signals:
    void mousePosChanged(const sf::Vector2f &worldPos,
                         const sf::Vector2i &pixelPos);
};
}
}
