#include "objects/RuntimeInfo.h"
#include "utilities/Stats.h"
#include "Scene/Scene.h"




namespace QSFML
{
	namespace Objects
	{
		RuntimeInfo::RuntimeInfo(const std::string& name)
			: GameObject(name)
		{
#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
			GameObject::addDrawFunction([](const GameObject& obj, sf::RenderTarget&, sf::RenderStates)
				{
					QSFML::Scene* Scene = obj.getSceneParent();
					if (Scene && obj.isCurrentRenderCameraTheDefaultCamera())
					{
						const QSFML::Utilities::Stats &stats = Scene->getLastStats();
						stats.drawImGui();
					}
				});
#else
			m_text = new Components::Text("InfoText");
			addComponent(m_text);
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");
#endif
			
			m_smoothStats = true;

			
		}
		RuntimeInfo::RuntimeInfo(const RuntimeInfo& other)
			: GameObject(other)
		{
#if IMGUI_SFML_LIBRARY_AVAILABLE != 1
			m_text = new Components::Text("InfoText");
			addComponent(m_text);
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");
#endif

			m_smoothStats = other.m_smoothStats;
		}
		RuntimeInfo::~RuntimeInfo()
		{

		}
		OBJECT_IMPL(RuntimeInfo);

		void RuntimeInfo::enableSmoothStats(bool enable)
		{
			m_smoothStats = enable;
		}
		bool RuntimeInfo::isSmoothStatsEnabled() const
		{
			return m_smoothStats;
		}

		void RuntimeInfo::update()
		{
			QSFML::Scene* Scene = getSceneParent();
			QSFML::Utilities::Stats stats = Scene->getLastStats();
			if (m_smoothStats)
				stats = stats.getSmothed(m_oldStats, 0.9);

			m_oldStats = stats;

			


#if IMGUI_SFML_LIBRARY_AVAILABLE != 1
			std::string statsStr = stats.toString();
			m_text->setText(statsStr);
			Utilities::AABB viewBox = Scene->getViewRect();
			sf::View view = Scene->getCameraView();
			sf::Vector2f pos = -view.getSize() * 0.5f;
			float width = viewBox.getSize().x;
			m_text->setScale(width / 2000);
			m_text->setPosition(pos);
			float rotation = Scene->getCameraView().getRotation();
			setRotation(rotation);
			setPosition(view.getCenter());
#endif
			
			

		}
	}
}
