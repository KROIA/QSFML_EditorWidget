#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"

#include <SFML/Graphics.hpp>

namespace QSFML
{
namespace Components
{

/**
 * \brief SfEventHandle handle sf::Events
 * \details This is one of the predefined Components.
 *          The SfEventHandle has a event function
 *          which will provide the sf::Events.
 *          This function will be called on every frame (if there are events)
 *          If multiple events are detected, this function gets called multiple times.
 *
 *          You must inherit from this class and then override the sfEvent(...) function
 * \code
 * class MyHandler : public QSFML::Components::SfEventHandle
 * {
 *     public:
 *     MyHandler(const std::string &name = "MyHandler")
 *         : SfEventHandle(name)
 *     {
 *
 *     }
 *
 *     void sfEvent(const sf::Event &e) override
 *     {
 *          switch(e.type)
 *          {
 *              case sf::Event::MouseMoved:
 *              {
 *                  sf::Vector2i mousePos = sf::Vector2i(e.mouseMove.x,e.mouseMove.y);
 *                  // Do something with the moved mouse pos
 *              }
 *          }
 *          default:
 *              break;
 *     }
 * };
 * \endcode
 */
class QSFML_EDITOR_WIDGET_EXPORT SfEventHandle :   public Component
{
    friend class Objects::GameObject;
    public:
        /**
         * \brief SfEventHandle constructor
         * \param name, the name of this Component
         * \see Component::setName()
         */
        SfEventHandle(const std::string &name = "SfEventHandle");
        SfEventHandle(const SfEventHandle &other);
        virtual ~SfEventHandle();


        /**
         * \brief sfEvent
         * \details This function will be automaticlly called from the
         *          Scene if the Component is enabled
         * \param events, The sf::Event events from the scene, grouped by CameraWindow which detected the event
         */
        virtual void sfEvent(const std::pair<Objects::CameraWindow*, std::vector<sf::Event>> &events) = 0;


        void setSceneParent(Scene* parent) override;

        void addCameraFilter(Objects::CameraWindow* cam)
		{
            if(!cam)
				return;
			for (size_t i = 0; i < m_cameraFilter.size(); ++i)
			{
				if (m_cameraFilter[i] == cam)
					return;
			}
			m_cameraFilter.push_back(cam);
		}
        void removeCameraFilter(Objects::CameraWindow* cam)
        {
            if(!cam)
                return;
            for (size_t i = 0; i < m_cameraFilter.size(); ++i)
		 	{
		 		if (m_cameraFilter[i] == cam)
		 		{
		 			m_cameraFilter.erase(m_cameraFilter.begin() + i);
		 			return;
		 		}
		 	}
		}
        const std::vector<Objects::CameraWindow*> &getCameraFilter() const
		{
			return m_cameraFilter;
		}
        void clearCameraFilter()
        {
            m_cameraFilter.clear();
        }

    private:
        void sfEvent_internal(const std::unordered_map<Objects::CameraWindow*, std::vector<sf::Event>>& events)
        {
            for (auto& filter : m_cameraFilter)
            {
                const auto &it = events.find(filter);
                if (it != events.end())
				{
					sfEvent(*it);
				}
            }
        }

        // Only propagate events that are from a camera in this list
        std::vector<Objects::CameraWindow*> m_cameraFilter;
};
}
}
