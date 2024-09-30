#include "objects/CameraController.h"
#include "Scene/Scene.h"
#include <QDebug>

using namespace QSFML::Objects;
namespace QSFML
{
    namespace Objects
    {
        COMPONENT_IMPL(CameraController)
            CameraController::CameraController(const std::string& name,
                GameObjectPtr parent)
            : GameObject(name, parent)
        {
            // getSceneParent() = nullptr;
            m_currentZoom = 1;
            setMinZoom(0.1);
            setMaxZoom(3);
            setMaxMovingBounds(sf::FloatRect(-200, -200, 900, 900));

            m_eventHandleComponent = new SfEventComponent();
            m_eventHandleComponent->setController(this);
            m_dragButton = sf::Mouse::Button::Left;
            addComponent(m_eventHandleComponent);
        }
        CameraController::CameraController(const CameraController& other)
            : GameObject(other)
        {
            m_currentZoom = other.m_currentZoom;
            m_minZoom = other.m_minZoom;
            m_maxZoom = other.m_maxZoom;
            m_maxMovingBounds = other.m_maxMovingBounds;
            m_dragButton = other.m_dragButton;
            m_eventHandleComponent = new SfEventComponent(*other.m_eventHandleComponent);
            m_eventHandleComponent->setController(this);

        }
        CameraController::~CameraController()
        {}
        void CameraController::setDragButton(sf::Mouse::Button button)
        {
            m_dragButton = button;
        }
        sf::Mouse::Button CameraController::getDragButton() const
        {
            return m_dragButton;
        }
        void CameraController::setMinZoom(float min)
        {
            m_minZoom = min;
        }

        void CameraController::setMaxZoom(float max)
        {
            m_maxZoom = max;
        }

        float CameraController::getMinZoom()const
        {
            return m_minZoom;
        }

        float CameraController::getMaxZoom()const
        {
            return m_maxZoom;
        }

        void CameraController::setMaxMovingBounds(const sf::FloatRect& rect)
        {
            m_maxMovingBounds = rect;
        }
        const sf::FloatRect& CameraController::getMaxMovingBounds() const
        {
            return m_maxMovingBounds;
        }

        void CameraController::moveLeft(float amount)
        {
            movePosition(sf::Vector2f(-amount, 0));
        }
        void CameraController::moveRight(float amount)
        {
            movePosition(sf::Vector2f(amount, 0));
        }
        void CameraController::moveUp(float amount)
        {
            movePosition(sf::Vector2f(0, -amount));
        }
        void CameraController::moveDown(float amount)
        {
            movePosition(sf::Vector2f(0, amount));
        }
        void CameraController::movePosition(const sf::Vector2f& delta)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            view.move(delta);
            positionCheck(view);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::setPosition(const sf::Vector2f& pos)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            view.setCenter(pos);
            positionCheck(view);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::rotate(float angle)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            view.rotate(angle);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::setRotation(float angle)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            view.setRotation(angle);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::zoom(float amount)
        {
            if (!getSceneParent()) return;
            float newZoom = m_currentZoom * amount;
            if (newZoom < m_minZoom || newZoom > m_maxZoom)
                return;
            m_currentZoom = newZoom;

            sf::View view = getSceneParent()->getCameraView();
            view.zoom(amount);
            positionCheck(view);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::zoom(float amount, const sf::Vector2i& pixel)
        {
            if (!getSceneParent()) return;
            float newZoom = m_currentZoom * amount;
            if (newZoom < m_minZoom || newZoom > m_maxZoom)
                return;
            sf::View view = getSceneParent()->getCameraView();
            m_currentZoom = newZoom;

            const sf::Vector2f beforeCoord = getInWorldSpace(pixel);

            view.zoom(amount);
            positionCheck(view);
            getSceneParent()->setCameraView(view);
            const sf::Vector2f afterCoord{ getInWorldSpace(pixel) };
            const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
            view.move(offsetCoords);
            positionCheck(view);
            getSceneParent()->setCameraView(view);
        }
        void CameraController::setZoom(float amount)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            sf::View defaultView = getSceneParent()->getDefaultCameraView();

            defaultView.setCenter(view.getCenter());
            getSceneParent()->setCameraView(defaultView);
            zoom(amount);
        }
        void CameraController::setZoom(float amount, const sf::Vector2i& pixel)
        {
            if (!getSceneParent()) return;
            sf::View view = getSceneParent()->getCameraView();
            sf::View defaultView = getSceneParent()->getDefaultCameraView();

            defaultView.setCenter(view.getCenter());
            getSceneParent()->setCameraView(defaultView);
            zoom(amount, pixel);
        }
        void CameraController::setCameraView(const sf::View& view)
        {
            if (!getSceneParent()) return;
            getSceneParent()->setCameraView(view);
        }
        void CameraController::update()
        {

        }

        void CameraController::SfEventComponent::setController(CameraController* controller)
        {
            m_controller = controller;
        }
        void CameraController::SfEventComponent::sfEvent(const sf::Event& e)
        {
            if (!m_controller) return;

            static bool mousePressed = false;
            static sf::Vector2f startPos;
            switch (e.type)
            {
            case sf::Event::MouseWheelScrolled:
            {
                float zoomAmount = 1.1;
                sf::Vector2i mousePos = m_controller->getMousePosition();
                if (e.mouseWheelScroll.delta > 0)
                    m_controller->zoom(1 / zoomAmount, mousePos);
                else if (e.mouseWheelScroll.delta < 0)
                    m_controller->zoom(zoomAmount, mousePos);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (e.mouseButton.button == m_controller->m_dragButton)
                {
                    startPos = sf::Vector2f(m_controller->getMousePosition());
                    mousePressed = true;
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (e.mouseButton.button == m_controller->m_dragButton)
                    mousePressed = false;
                break;
            }
            case sf::Event::MouseMoved:
            {
                 // Ignore mouse movement unless a button is pressed (see above)
                if (!mousePressed)
                    break;
                
                sf::Vector2f newPos = sf::Vector2f(m_controller->getMousePosition());
                sf::Vector2f deltaPos = m_controller->getInWorldSpace(sf::Vector2i(startPos)) - m_controller->getInWorldSpace(sf::Vector2i(newPos));

                m_controller->movePosition(deltaPos);
                startPos = newPos;
                break;
            }
            case sf::Event::Resized:
            {
                sf::View view = m_controller->getCameraView();

                sf::Vector2u oldWindowSize = m_controller->getOldSceneSize();
                sf::Vector2u newWindowSize = m_controller->getSceneSize();
                sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

                viewRect.width = viewRect.width / oldWindowSize.x * newWindowSize.x;
                viewRect.height = viewRect.height / oldWindowSize.y * newWindowSize.y;

                view.setSize(viewRect.width, viewRect.height);
                view.setCenter(viewRect.left + viewRect.width / 2.f, viewRect.top + viewRect.height / 2.f);
                m_controller->positionCheck(view);
                m_controller->setCameraView(view);
                break;
            }
            }

        }

        void CameraController::positionCheck(sf::View& view)
        {
            sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
            sf::Vector2f cameraPos = view.getCenter();
            sf::Vector2u windowSize = getSceneSize();
            float aspectRatio = (float)windowSize.x / (float)windowSize.y;

            if (viewRect.width / viewRect.height > aspectRatio)
            {
                viewRect.width = viewRect.height * aspectRatio;
            }
            else
            {
                viewRect.height = viewRect.width / aspectRatio;
            }
            if (cameraPos.x < m_maxMovingBounds.left)
                cameraPos.x = m_maxMovingBounds.left;
            else if (cameraPos.x > m_maxMovingBounds.left + m_maxMovingBounds.width)
                cameraPos.x = m_maxMovingBounds.left + m_maxMovingBounds.width;

            if (cameraPos.y < m_maxMovingBounds.top)
                cameraPos.y = m_maxMovingBounds.top;
            else if (cameraPos.y > m_maxMovingBounds.top + m_maxMovingBounds.height)
                cameraPos.y = m_maxMovingBounds.top + m_maxMovingBounds.height;

            view.setSize(viewRect.width, viewRect.height);
            view.setCenter(cameraPos);
        }
    }
}
