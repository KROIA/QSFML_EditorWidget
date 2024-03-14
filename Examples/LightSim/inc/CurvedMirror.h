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
	//bool processLaser(const LightRay& ray,
	//	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	//	std::vector< LaserInfo>& additionalLightPathsOut) const override;
	//bool getRaycastDistance(const LightRay& ray, float& distanceOut) const override;

	void update() override;

	class MirrorShape : public Shape
	{
	public:
		bool getCollisionData(const LightRay& ray,
			float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const override;

		bool getShortestDistanceAndIndex(const LightRay& ray, float& distanceA, float& distanceB, size_t& index) const;

		float m_angle;
		float m_radius;
		float m_openingAngle;
		unsigned int m_resolution;
		sf::Vector2f m_pos;
		std::vector<QSFML::Utilities::Ray> m_mirrorRays;

	};

	MirrorShape m_shape;


	std::vector<QSFML::Components::LinePainter*> m_mirrorLines;	 
	float m_rotationSpeed;
};
