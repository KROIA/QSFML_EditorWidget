#pragma once

#include "QSFML_EditorWidget_base.h"
#include "objects/base/GameObject.h"
#include "components/base/SfEventHandle.h"

#include <SFML/Graphics.hpp>

namespace QSFML
{
namespace Objects
{

class QSFML_EDITOR_WIDGET_EXPORT CameraController: public GameObject
{
        class SfEventComponent;
    public:
        CameraController(const std::string &name = "",
                         GameObjectPtr parent = nullptr);
        CameraController(const CameraController &other);
        ~CameraController();
        COMPONENT_DECL(CameraController);

        void setDragButton(sf::Mouse::Button button);
        sf::Mouse::Button getDragButton() const;

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

        void update() override;

        Objects::CameraWindow* getCamera() const;

    private:
        sf::Vector2i getThisCameraMousePosition() const;
        sf::Vector2f getInThisCameraWorldSpace(const sf::Vector2i& pixelSpace) const;
        sf::Vector2u getThisCameraOldSize() const;
        sf::Vector2u getThisCameraSize() const;
		sf::View getThisCameraView() const;

        void onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent) override;
        //void internalOnSceneParentChange(Scene *oldParent, Scene *newParent) override;
        //void internalOnParentChange(GameObjectPtr oldParent, GameObjectPtr newParent) override;
        void positionCheck(sf::View &view);

        float m_currentZoom;
        float m_minZoom;
        float m_maxZoom;
        sf::FloatRect m_maxMovingBounds;

        SfEventComponent* m_eventHandleComponent;
        sf::Mouse::Button m_dragButton;
        Objects::CameraWindow* m_customCam = nullptr;

    // Defining component
    class SfEventComponent : public Components::SfEventHandle
    {
        public:
            SfEventComponent()
            {
                m_controller = nullptr;
            }
            SfEventComponent(const SfEventComponent &other)
                : SfEventHandle(other)
            {
                m_controller = nullptr;
            }
            ~SfEventComponent(){}

            void setController(CameraController *controller);
            void sfEvent(const sf::Event &e) override;

        private:
            CameraController *m_controller;
    };
};
}
}
