#pragma once

#include "OpticalElement.h"

class ConvexLens : public OpticalElement
{
	class LensPainter;
	friend LensPainter;
public:
	ConvexLens(const std::string& name = "ConvexLens");
	ConvexLens(const ConvexLens& other);
	~ConvexLens();
	OBJECT_DECL(ConvexLens);


	void setColor(const sf::Color& c);
	const sf::Color& getColor() const;

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;

	void setRotation(float angle);
	float getRotation() const;

	void setDiameter(float w);
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

	
	class LensShape : public Shape
	{
	public:
		bool getCollisionData(const LightRay& ray,
			float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const override;

		void updateShape();

		void setThickness(float t);
		float getThickness() const;

		float getFocusLength() const;
	
		bool getLensCollisionFactor(const LightRay& ray, bool outgoingRay, float& smalestFactor, float& collisionNormal, bool &rayInsideShape) const;

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
		float m_lensThickness;
		float m_parableParamA;
		float m_parableParamB;
		
		float m_angle;
		float m_lensRadius;
		float m_openingAngle;
		sf::Vector2f m_pos;
		sf::Vector2f m_circlePos1; // Pos of first circle element
		sf::Vector2f m_circlePos2;

		class LensPainter : public QSFML::Components::Drawable
		{
		public:
			LensPainter(LensShape* lense, const std::string& name = "LensPainter");
			LensPainter(const LensPainter& other);
			COMPONENT_DECL(LensPainter);

			
			void draw(sf::RenderTarget& target,
				sf::RenderStates states) const override;

			sf::Color m_color;
			LensShape* m_lens;
			unsigned int m_resolution;
			
		private:

		};

		mutable std::vector<sf::Drawable*> m_tmpDraw;
		LensPainter* m_painter;
		ConvexLens* m_lens;
	};

	LensShape m_shape;

	

	

	//float m_n1;
	//float m_n2;

	

};
