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
			
			addComponent(m_text);
		}
		RuntimeInfo::RuntimeInfo(const RuntimeInfo& other)
			: CanvasObject(other)
		{
			m_text = new Components::Text("InfoText");
			m_text->setFont("C:\\Windows\\Fonts\\courbd.ttf");

			addComponent(m_text);
		}
		RuntimeInfo::~RuntimeInfo()
		{

		}
		OBJECT_IMPL(RuntimeInfo);

		void RuntimeInfo::update()
		{
			QSFML::Stats stats = QSFML::StatsManager::getLastStats();
			QSFML::Canvas* canvas = getCanvasParent();

			std::string statsStr = stats.toString();

			std::string performanceText =
				std::string("Performance:\n")+
				std::string("  Tick:                ") + std::to_string(canvas->getTick()) + "\n" +
				std::string("  FPS:                 ") + std::to_string(canvas->getFPS()) + "\n" + 
				std::string("  Frametime:           ") + std::to_string(canvas->getFrametime() * 1000.f) + " ms\n";

	
			m_text->setText(performanceText + statsStr);
			Utilities::AABB viewBox = canvas->getCameraViewRect();
			sf::Vector2f pos = viewBox.TL();
			float width = viewBox.getSize().x;
			m_text->setScale(width / 2000);
			m_text->setPosition(pos);

		}
	}
}
