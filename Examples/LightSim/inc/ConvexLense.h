#pragma once

#include "OpticalElement.h"

class ConvexLense : public OpticalElement
{
	class LensePainter;
	friend LensePainter;
public:
	ConvexLense(const std::string& name = "ConvexLense");
	ConvexLense(const ConvexLense& other);
	~ConvexLense();
	OBJECT_DECL(ConvexLense);


	void setColor(const sf::Color& c);
	const sf::Color& getColor() const;

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;

	void setRotation(float angle);
	float getRotation() const;

	void serDiameter(float w);
	float getDiameter() const;

	void setThickness(float t);
	float getThickness() const;

	void setFocusLength(float r);
	float getFocusLength() const;

private:
	
	//bool processLaser(const LightRay& ray,
	//	std::vector< LightRay>& reflectedOut,
	//	std::vector< LaserInfo>& additionalLightPathsOut) const override;
	//bool getRaycastDistance(const LightRay& ray, float& distanceOut) const override;


	//bool processLaser_intern(const LightRay& ray,
	//	std::vector< LightRay>& reflectedOut,
	//	std::vector< LaserInfo>& additionalLightPathsOut, bool outgoingRay,
	//	sf::Vector2f & collisionPointOut) const;
	
	void update() override;

	
	class LenseShape : public Shape
	{
	public:
		bool getCollisionData(const LightRay& ray,
			float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const override;

		void updateShape();

		void setThickness(float t);
		float getThickness() const;

		float getFocusLength() const;
	
		bool getLenseCollisionFactor(const LightRay& ray, bool outgoingRay, float& smalestFactor, float& collisionNormal, bool &rayInsideShape) const;

		// calculates the malest factor in ray which collides on the edge of one part of the lense
		bool getCircleElementCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
			float radius, float minAngle, float maxAngle, float& smalestFactor, bool &rayInsideSegment) const;

		// calculates the factor in ray which collides on the edge of the circle
		bool getCircleCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
			float radius, float& fac1, float& fac2) const;


		bool getRaycastData(const LightRay& ray,
			const sf::Vector2f& circleOrigin,
			float& alpha1, float& alpha2,
			float& distance1, float& distance2,
			sf::Vector2f& point1,
			sf::Vector2f& point2) const;

		//void getCollisionData(float alpha, float rayAngle, float& entryAngle, sf::Vector2f& point);

		bool getParableData(const LightRay& ray, float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const;
		
		// Parabel in der Form: ax^2 +b
		bool getParableCollisionFactor(const QSFML::Utilities::Ray& ray, double a, double b, double parableRotation,
			const sf::Vector2f &parablePos, double minMaxInputRange, 
			double& outFac1, double& outFac2, 
			double& outNormal1, double &outNormal2, 
			bool &fac1InRange, bool &vac2InRange) const;

		void getPainterVertecies(std::vector < sf::Vertex>& left, std::vector < sf::Vertex>& right, size_t resolution);
		
		

		
		float m_diameter;
		float m_lenseThickness;
		float m_parableParamA;
		float m_parableParamB;
		
		float m_angle;
		float m_lenseRadius;
		float m_openingAngle;
		sf::Vector2f m_pos;
		sf::Vector2f m_circlePos1; // Pos of first circle element
		sf::Vector2f m_circlePos2;

		class LensePainter : public QSFML::Components::Drawable
		{
		public:
			LensePainter(LenseShape* lense, const std::string& name = "LensePainter");
			LensePainter(const LensePainter& other);
			COMPONENT_DECL(LensePainter);

			
			void draw(sf::RenderTarget& target,
				sf::RenderStates states) const override;

			sf::Color m_color;
			LenseShape* m_lense;
			unsigned int m_resolution;
			
		private:

		};

		mutable std::vector<sf::Drawable*> m_tmpDraw;
		LensePainter* m_painter;
		ConvexLense* m_lense;
	};

	LenseShape m_shape;

	

	

	//float m_n1;
	//float m_n2;

	

};