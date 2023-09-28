#pragma once
#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"

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
	void updateRay(QSFML::Utilities::Ray& r, LaserInfo &info, unsigned int iteration);

	const std::vector<OpticalElement*> const& m_opticalElements;
	QSFML::Utilities::Ray m_rootRay;
	LaserInfo m_rootLaserInfo;
	unsigned int m_maxBounces;

	std::vector<QSFML::Utilities::Ray> m_childRays;

	
	
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
		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override;

		
	};
	LaserPainter* m_painter;
};