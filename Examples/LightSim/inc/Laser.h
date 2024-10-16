#pragma once
#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"
#include "LightRay.h"

class OpticalElement;
class Laser: public QSFML::Objects::GameObject
{
	class LaserPainter;
	
	friend LaserPainter;
public:
	Laser(const std::string& name = "Laser",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	Laser(const Laser& other);
	~Laser();

	

	CLONE_FUNC_DEC(Laser);

	void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition() const;
	void setDirection(const sf::Vector2f& dir);
	const sf::Vector2f& getDirection() const;

	void setColor(const sf::Color& col);
	const sf::Color& getColor() const;

	void setMaxBounces(unsigned int max);
	unsigned int getMaxBounces() const;


private:
	

	void update() override;
	void updateRay(LightRay& r, LaserInfo &info, unsigned int iteration);

	const QSFML::vector<OpticalElement*> & m_opticalElements;
	LightRay m_rootRay;
	LaserInfo m_rootLaserInfo;
	unsigned int m_maxBounces;

	QSFML::vector<LightRay> m_childRays;

	
	
	QSFML::vector<LaserInfo> m_lasers;

	class LaserPainter : public QSFML::Components::Drawable
	{
	public:
		LaserPainter(QSFML::vector<LaserInfo>& l, const std::string& name = "LaserPainter");
		LaserPainter(const LaserPainter& other);
		COMPONENT_DECL(LaserPainter);


		sf::Color m_color;
		QSFML::vector<LaserInfo>& m_lasers;
	private:
		void drawComponent(sf::RenderTarget& target,
			sf::RenderStates states) const override;

		
	};
	LaserPainter* m_painter;
};
