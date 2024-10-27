#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

namespace QSFML
{
	namespace Objects
	{

#ifdef QSFML_DEBUG

/*
    If this erro is triggered, it means that the GameObject is not attached to a Scene.
    To call such functions, the GameObject must be attached to a Scene.
*/
#define QSFML_ERROR_HAS_SCENE_PARENT_LOG GameObject::logError(std::string(__PRETTY_FUNCTION__) + " This function can only be called when the GameObject is in a scene"); 
#define QSFML_ERROR_HAS_SCENE_PARENT_INTERNAL(SUB)  if (!m_sceneParent) { SUB; }
#define QSFML_ERROR_HAS_SCENE_PARENT QSFML_ERROR_HAS_SCENE_PARENT_INTERNAL(QSFML_ERROR_HAS_SCENE_PARENT_LOG; return;)
#define QSFML_ERROR_HAS_SCENE_PARENT_R(ret) QSFML_ERROR_HAS_SCENE_PARENT_INTERNAL(QSFML_ERROR_HAS_SCENE_PARENT_LOG; return ret;)
#define QSFML_ERROR_HAS_SCENE_PARENT_RS(construct) QSFML_ERROR_HAS_SCENE_PARENT_INTERNAL(QSFML_ERROR_HAS_SCENE_PARENT_LOG; static auto dummy = construct; return dummy;)
#else
#define QSFML_ERROR_HAS_SCENE_PARENT
#define QSFML_ERROR_HAS_SCENE_PARENT_R(ret)
#define QSFML_ERROR_HAS_SCENE_PARENT_RS(type)
#endif
        sf::Vector2i GameObject::getMousePosition() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2i(0,0));
            return m_sceneParent->getMousePosition();
        }
        sf::Vector2f GameObject::getMouseWorldPosition() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2f(0.f, 0.f));
            return m_sceneParent->getMouseWorldPosition();
        }
        sf::Vector2f GameObject::getMouseObjectPosition() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2f(0.f, 0.f));
            return getMouseWorldPosition() - getGlobalPosition();
        }

        sf::Vector2f GameObject::getInWorldSpace(const sf::Vector2i& pixelSpace) const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2f(0.f, 0.f));
            return m_sceneParent->getInWorldSpace(pixelSpace);
        }
        sf::Vector2i GameObject::getInScreenSpace(const sf::Vector2f& worldSpace) const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2i(0, 0));
            return m_sceneParent->getInScreenSpace(worldSpace);
        }
        const sf::View GameObject::getCameraView() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::View());
            return m_sceneParent->getCameraView();
        }
        const sf::View& GameObject::getDefaultCameraView() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_RS(sf::View());
            return m_sceneParent->getDefaultCameraView();
        }
        Utilities::AABB GameObject::getCameraViewRect() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(Utilities::AABB());
            return m_sceneParent->getViewRect();
        }
        sf::Vector2u GameObject::getCameraSize() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2u(0, 0));
            return m_sceneParent->getCameraSize();
        }
        sf::Vector2u GameObject::getOldCameraSize() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(sf::Vector2u(0, 0));
            return m_sceneParent->getOldCameraSize();
        }
        CameraWindow* GameObject::getDefaultCamera() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(nullptr);
            return m_sceneParent->getDefaultCamera();
        }
        CameraWindow* GameObject::getCurrentRenderCamera() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(nullptr);
			return m_sceneParent->getCurrentRenderCamera();
        }

        const sf::Font& GameObject::getDefaultTextFont() const
        {
            return Scene::getDefaultTextFont();
        }
        size_t GameObject::getTick() const
        {
            QSFML_ERROR_HAS_SCENE_PARENT_R(0);
            return m_sceneParent->getTick();
        }
        double GameObject::getDeltaT() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(0.0);
            return m_sceneParent->getDeltaT();
        }
        double GameObject::getFixedDeltaT() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(0.0);
            return m_sceneParent->getFixedDeltaT();
        }
        double GameObject::getElapsedTime() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(0.0);
            return m_sceneParent->getElapsedTime();
        }
        double GameObject::getFixedElapsedTime() const
        {
			QSFML_ERROR_HAS_SCENE_PARENT_R(0.0);
            return m_sceneParent->getFixedElapsedTime();
        }


	}
}