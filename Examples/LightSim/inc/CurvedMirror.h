#pragma once

#include "OpticalElement.h"

class CurvedMirror : public OpticalElement
{
public:
	CurvedMirror(unsigned int resolution = 20,const std::string& name = "CurvedMirror");
	CurvedMirror(const CurvedMirror& other);
	~CurvedMirror();
	OBJECT_DECL(CurvedMirror);

	void setColor(const sf::Color& c);
	const sf::Color& getColor() const;

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;

	void setRotation(float angle);
	float getRotation() const;


	void setOpeningAngle(float angle);
	float getOpeningAngle() const;

	void setRadius(float r);
	float getRadius() const;

private:
	void updateLine();
	bool processLaser(const QSFML::Utilities::Ray& ray,
		std::vector< QSFML::Utilities::Ray>& reflectedOut,
		std::vector< LaserInfo>& additionalLightPathsOut) const override;
	bool getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const override;

	bool getShortestDistanceAndIndex(const QSFML::Utilities::Ray& ray, float& distanceA, float& distanceB, size_t& index) const;
	void update() override;

	std::vector<QSFML::Components::Line*> m_mirrorLines;
	std::vector<QSFML::Utilities::Ray> m_mirrorRays;


	float m_angle;
	float m_radius;
	float m_openingAngle;
	unsigned int m_resolution;
	sf::Vector2f m_pos;
	 
	float m_rotationSpeed;
};