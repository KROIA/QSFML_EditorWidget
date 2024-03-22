#pragma once
#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"
#include "LightRay.h"

class OpticalElement;
class Laser: public QSFML::Objects::CanvasObject
{
	class LaserPainter;
	
	friend LaserPainter;
public:
	Laser(const std::string& name = "Laser",
		CanvasObject* parent = nullptr);
	Laser(const Laser& other);
	~Laser();

	

	CLONE_FUNC_DEC(Laser);

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;
	void setDirection(const sf::Vector2f& dir);
	const sf::Vector2f& getDirection() const;

	void setColor(const sf::Color& col);
	const sf::Color& getColor() const;

	void setMaxBounces(unsigned int max);
	unsigned int getMaxBounces() const;


private:
	

	void update() override;
	void updateRay(LightRay& r, LaserInfo &info, unsigned int iteration);

	const std::vector<OpticalElement*> const& m_opticalElements;
	LightRay m_rootRay;
	LaserInfo m_rootLaserInfo;
	unsigned int m_maxBounces;

	std::vector<LightRay> m_childRays;

	
	
	std::vector<LaserInfo> m_lasers;

	class LaserPainter : public QSFML::Components::Drawable
	{
	public:
		LaserPainter(std::vector<LaserInfo>& l, const std::string& name = "LaserPainter");
		LaserPainter(const LaserPainter& other);
		COMPONENT_DECL(LaserPainter);


		sf::Color m_color;
		std::vector<LaserInfo>& m_lasers;
	private:
		void drawComponent(sf::RenderTarget& target,
			sf::RenderStates states) const override;

		
	};
	LaserPainter* m_painter;
};
