#include "objects/CameraController.h"
#include "Scene/Scene.h"
#include "objects/CameraWindow.h"
#include <QDebug>
#include "utilities/VectorOperations.h"

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
            setMaxMovingBounds(sf::FloatRect(0, 0, 1000, 1000));

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
            move(sf::Vector2f(-amount, 0));
        }
        void CameraController::moveRight(float amount)
        {
            move(sf::Vector2f(amount, 0));
        }
        void CameraController::moveUp(float amount)
        {
            move(sf::Vector2f(0, -amount));
        }
        void CameraController::moveDown(float amount)
        {
            move(sf::Vector2f(0, amount));
        }
        /*void CameraController::movePosition(const sf::Vector2f& delta)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getThisCameraView();
            view.move(delta);
            positionCheck(view);
            cam->setThisCameraView(view);
        }
        void CameraController::setPosition(const sf::Vector2f& pos)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getThisCameraView();
            view.setCenter(pos);
            positionCheck(view);
            cam->setThisCameraView(view);
        }
        void CameraController::rotate(float angle)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getThisCameraView();
            view.rotate(angle);
            cam->setThisCameraView(view);
        }
        void CameraController::setRotation(float angle)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getCameraView();
            view.setRotation(angle);
            cam->setThisCameraView(view);
        }*/
        void CameraController::zoom(float amount)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            float newZoom = m_currentZoom * amount;
            if (newZoom < m_minZoom || newZoom > m_maxZoom)
                return;
            m_currentZoom = newZoom;

            sf::View view = cam->getThisCameraView();
            sf::Vector2f orgPos = view.getCenter();
            view.zoom(amount);
            positionCheck(view);
            sf::Vector2f newPos = view.getCenter();
            view.setCenter(orgPos);
            cam->setPosition(newPos);
            cam->setThisCameraView(view);
        }
        void CameraController::zoom(float amount, const sf::Vector2i& pixel)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            float newZoom = m_currentZoom * amount;
            if (newZoom < m_minZoom || newZoom > m_maxZoom)
                return;
            sf::View view = cam->getThisCameraView();
			sf::Vector2f orgPos = view.getCenter();
            m_currentZoom = newZoom;

            const sf::Vector2f beforeCoord = cam->getInThisCameraWorldSpace(pixel);

            view.zoom(amount);
            positionCheck(view);
            cam->setThisCameraView(view);
            const sf::Vector2f afterCoord{ cam->getInThisCameraWorldSpace(pixel) };
            const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
            view.move(offsetCoords);
            positionCheck(view);
			sf::Vector2f newPos = view.getCenter();
			view.setCenter(orgPos);
			cam->setPosition(newPos);
            cam->setThisCameraView(view);
        }
        void CameraController::setZoom(float amount)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getThisCameraView();
            sf::View defaultView = cam->getThisCameraDefaultView();

            defaultView.setCenter(view.getCenter());
            cam->setThisCameraView(defaultView);
            zoom(amount);
        }
        void CameraController::setZoom(float amount, const sf::Vector2i& pixel)
        {
            Objects::CameraWindow* cam = getCamera();
            if (!cam) return;
            sf::View view = cam->getThisCameraView();
            sf::View defaultView = cam->getThisCameraDefaultView();

            defaultView.setCenter(view.getCenter());
            cam->setThisCameraView(defaultView);
            zoom(amount, pixel);
        }
        void CameraController::setCameraView(const sf::View& view)
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
				cam->setThisCameraView(view);
        }
        void CameraController::setCameraView(const sf::FloatRect& area)
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
            {
                sf::View v(area);
                cam->setThisCameraView(v);
            }
        }
        void CameraController::update()
        {
           // Objects::CameraWindow* cam = getCamera();
            //if (!cam) 
            //    return;
            //sf::Transform globalTransform = getGlobalTransform();
            //sf::Vector2f pos = globalTransform.transformPoint({ 0.0, 0.0 });
            //float rot = VectorMath::getRotation(globalTransform);

            
            //sf::View view = cam->getThisCameraView();
			//view.setCenter(pos);
			//view.setRotation(rot);
            //positionCheck(view);
           // cam->setThisCameraView(view);

        }
        Objects::CameraWindow* CameraController::getCamera() const
        {
            if (m_customCam)
                return m_customCam;
            Scene* sc = getSceneParent();
            if (sc)
                return sc->getDefaultCamera();
            return nullptr;
        }


        sf::Vector2i CameraController::getThisCameraMousePosition() const
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
                return cam->getThisCameraMousePosition();
            return sf::Vector2i(0, 0);
        }
        sf::Vector2f CameraController::getInThisCameraWorldSpace(const sf::Vector2i& pixelSpace) const
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
                return cam->getInThisCameraWorldSpace(pixelSpace);
            return sf::Vector2f(0, 0);
        }
        sf::Vector2u CameraController::getThisCameraOldSize() const
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
                return cam->getThisCameraOldSize();
            return sf::Vector2u(0, 0);
        }
        sf::Vector2u CameraController::getThisCameraSize() const
        {
            Objects::CameraWindow* cam = getCamera();
            if (cam)
                return cam->getThisCameraSize();
            return sf::Vector2u(0, 0);
        }
        sf::View CameraController::getThisCameraView() const
        {
			Objects::CameraWindow* cam = getCamera();
			if (cam)
				return cam->getThisCameraView();
			static sf::View dummy;
			return dummy;
        }

        void CameraController::SfEventComponent::setController(CameraController* controller)
        {
            m_controller = controller;
        }
        void CameraController::SfEventComponent::sfEvent(const QSFML::pair<Objects::CameraWindow*, QSFML::vector<sf::Event>>& events)
        {
            if (!m_controller) return;
            
            static bool mousePressed = false;
            static sf::Vector2f startPos;
            for (auto& e : events.second)
            {
                switch (e.type)
                {
                case sf::Event::MouseWheelScrolled:
                {
                    float zoomAmount = 1.1;
                    sf::Vector2i mousePos = m_controller->getThisCameraMousePosition();
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
                        startPos = sf::Vector2f(m_controller->getThisCameraMousePosition());
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
                    auto cam = m_controller->getCamera();
                    sf::Vector2f newPos = sf::Vector2f(cam->getThisCameraMousePosition());
                    sf::Vector2f deltaPos = cam->getInThisCameraWorldSpace(sf::Vector2i(startPos)) -
                                            cam->getInThisCameraWorldSpace(sf::Vector2i(newPos));

                    cam->GameObject::move(deltaPos);
                    //m_controller->move(deltaPos);
                    startPos = newPos;
                    break;
                }
                case sf::Event::Resized:
                {
                    /*
                    sf::View view = m_controller->getThisCameraView();

                    sf::Vector2u oldWindowSize = m_controller->getThisCameraOldSize();
                    sf::Vector2u newWindowSize = m_controller->getThisCameraSize();
                    sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

                    viewRect.width = viewRect.width / oldWindowSize.x * newWindowSize.x;
                    viewRect.height = viewRect.height / oldWindowSize.y * newWindowSize.y;

                    view.setSize(viewRect.width, viewRect.height);
                    view.setCenter(viewRect.left + viewRect.width / 2.f, viewRect.top + viewRect.height / 2.f);
                    m_controller->positionCheck(view);
                    m_controller->setCameraView(view);
                    break;*/
                }
                }
            }

        }

        void CameraController::onParentChange(GameObjectPtr oldParent, GameObjectPtr newParent)
        {
            GameObject::onParentChange(oldParent, newParent);
            m_customCam = dynamic_cast<Objects::CameraWindow*>(newParent);
            if (m_customCam)
            {
                m_eventHandleComponent->clearCameraFilter();
                m_eventHandleComponent->addCameraFilter(m_customCam);
            }
        }
        void CameraController::positionCheck(sf::View& view)
        {
            sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
            sf::Vector2f cameraPos = view.getCenter();
            sf::Vector2u windowSize = getThisCameraSize();
            float aspectRatio = (float)windowSize.x / (float)windowSize.y;

            if (viewRect.width / viewRect.height > aspectRatio)
            {
                viewRect.width = viewRect.height * aspectRatio;
            }
            else
            {
                viewRect.height = viewRect.width / aspectRatio;
            }
            /*
            if (cameraPos.x < m_maxMovingBounds.left)
                cameraPos.x = m_maxMovingBounds.left;
            else if (cameraPos.x > m_maxMovingBounds.left + m_maxMovingBounds.width)
                cameraPos.x = m_maxMovingBounds.left + m_maxMovingBounds.width;

            if (cameraPos.y < m_maxMovingBounds.top)
                cameraPos.y = m_maxMovingBounds.top;
            else if (cameraPos.y > m_maxMovingBounds.top + m_maxMovingBounds.height)
                cameraPos.y = m_maxMovingBounds.top + m_maxMovingBounds.height;*/

			setPosition(cameraPos);
            view.setSize(viewRect.width, viewRect.height);
            view.setCenter(cameraPos);
        }
    }
}
