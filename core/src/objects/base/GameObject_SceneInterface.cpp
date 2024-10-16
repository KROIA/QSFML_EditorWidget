#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

namespace QSFML
{
	namespace Objects
	{

        sf::Vector2i GameObject::getMousePosition() const
        {
            if (!m_sceneParent) return sf::Vector2i(0, 0);
            return m_sceneParent->getMousePosition();
        }
        sf::Vector2f GameObject::getMouseWorldPosition() const
        {
            if (!m_sceneParent) return sf::Vector2f(0, 0);
            return m_sceneParent->getMouseWorldPosition();
        }
        sf::Vector2f GameObject::getMouseObjectPosition() const
        {
            return getMouseWorldPosition() - getGlobalPosition();
        }

        sf::Vector2f GameObject::getInWorldSpace(const sf::Vector2i& pixelSpace) const
        {
            if (!m_sceneParent) return sf::Vector2f(0, 0);
            return m_sceneParent->getInWorldSpace(pixelSpace);
        }
        sf::Vector2i GameObject::getInScreenSpace(const sf::Vector2f& worldSpace) const
        {
            if (!m_sceneParent) return sf::Vector2i(0, 0);
            return m_sceneParent->getInScreenSpace(worldSpace);
        }
        const sf::View GameObject::getCameraView() const
        {
            static const sf::View dummy;
            if (!m_sceneParent) return dummy;
            return m_sceneParent->getCameraView();
        }
        const sf::View& GameObject::getDefaultCameraView() const
        {
            static const sf::View dummy;
            if (!m_sceneParent) return dummy;
            return m_sceneParent->getDefaultCameraView();
        }
        Utilities::AABB GameObject::getCameraViewRect() const
        {
            if (!m_sceneParent) return Utilities::AABB();
            return m_sceneParent->getViewRect();
        }
        sf::Vector2u GameObject::getCameraSize() const
        {
            if (!m_sceneParent) return sf::Vector2u(0, 0);
            return m_sceneParent->getCameraSize();
        }
        sf::Vector2u GameObject::getOldCameraSize() const
        {
            if (!m_sceneParent) return sf::Vector2u(0, 0);
            return m_sceneParent->getOldCameraSize();
        }
        CameraWindow* GameObject::getDefaultCamera() const
        {
            if (!m_sceneParent) return nullptr;
            return m_sceneParent->getDefaultCamera();
        }
        CameraWindow* GameObject::getCurrentRenderCamera() const
        {
			if (!m_sceneParent) return nullptr;
			return m_sceneParent->getCurrentRenderCamera();
        }

        const sf::Font& GameObject::getDefaultTextFont() const
        {
            return Scene::getDefaultTextFont();
        }
        size_t GameObject::getTick() const
        {
            if (!m_sceneParent) return 0;
            return m_sceneParent->getTick();
        }
        double GameObject::getDeltaT() const
        {
            if (!m_sceneParent) return 0;
            return m_sceneParent->getDeltaT();
        }
        double GameObject::getFixedDeltaT() const
        {
            if (!m_sceneParent) return 0;
            return m_sceneParent->getFixedDeltaT();
        }
        double GameObject::getElapsedTime() const
        {
            if (!m_sceneParent) return 0;
            return m_sceneParent->getElapsedTime();
        }
        double GameObject::getFixedElapsedTime() const
        {
            if (!m_sceneParent) return 0;
            return m_sceneParent->getFixedElapsedTime();
        }


	}
}