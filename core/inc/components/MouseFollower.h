#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/SfEventHandle.h"

#include <QObject>

namespace QSFML
{
namespace Components
{
/**
 * @class MouseFollower
 * @brief The MouseFollower is a Component that will emit a signal when the mouse moves
 */
class QSFML_EDITOR_WIDGET_API MouseFollower : public QObject, public SfEventHandle
{
        Q_OBJECT
    public:
    MouseFollower(const std::string &name = "MouseFollower");
    MouseFollower(const MouseFollower &other);
    COMPONENT_DECL(MouseFollower);

    /**
     * @brief 
     * Gets called by the Scene to process the events for a specific camera.
     * @param events 
     */
    void sfEvent(const QSFML::pair<Objects::CameraWindow*, QSFML::vector<sf::Event>> &events) override;


    signals:

	 /**
     * @brief
     * This signal is emitted when the mouse position changes
     * @param worldPos
     * @param pixelPos
     */
    void mousePosChanged(const sf::Vector2f &worldPos,
                         const sf::Vector2i &pixelPos);
};
}
}
