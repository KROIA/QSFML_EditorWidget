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
			m_text = new Components::Text("InfoText");
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");
			m_smoothStats = true;

			addComponent(m_text);
		}
		RuntimeInfo::RuntimeInfo(const RuntimeInfo& other)
			: GameObject(other)
		{
			m_text = new Components::Text("InfoText");
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");

			m_smoothStats = other.m_smoothStats;

			addComponent(m_text);
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

			std::string statsStr = stats.toString();


	
			m_text->setText(statsStr);
			Utilities::AABB viewBox = Scene->getViewRect();
			sf::View view = Scene->getCameraView();
			sf::Vector2f pos = -view.getSize()*0.5f;
			float width = viewBox.getSize().x;
			m_text->setScale(width / 2000);
			m_text->setPosition(pos);
			float rotation = Scene->getCameraView().getRotation();
			setRotation(rotation);
			setPosition(view.getCenter());

		}
	}
}
