#include "objects/RuntimeInfo.h"
#include "utilities/Stats.h"
#include "canvas/Canvas.h"

namespace QSFML
{
	namespace Objects
	{
		RuntimeInfo::RuntimeInfo(const std::string& name)
			: CanvasObject(name)
		{
			m_text = new Components::Text("InfoText");
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");
			m_smoothStats = true;

			addComponent(m_text);
		}
		RuntimeInfo::RuntimeInfo(const RuntimeInfo& other)
			: CanvasObject(other)
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
			QSFML::Canvas* canvas = getCanvasParent();
			QSFML::Utilities::Stats stats = canvas->getLastStats();
			if (m_smoothStats)
				stats = stats.getSmothed(m_oldStats, 0.9);

			m_oldStats = stats;

			std::string statsStr = stats.toString();


	
			m_text->setText(statsStr);
			Utilities::AABB viewBox = canvas->getCameraViewRect();
			sf::Vector2f pos = viewBox.TL();
			float width = viewBox.getSize().x;
			m_text->setScale(width / 2000);
			m_text->setPosition(pos);

		}
	}
}
