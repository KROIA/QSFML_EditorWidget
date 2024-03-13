#pragma once

#include "OpticalElement.h"

class SimpleLens : public OpticalElement
{
	class LensPainter;
	friend LensPainter;
public:
	SimpleLens(const std::string& name = "SimpleLens");
	SimpleLens(const SimpleLens& other);
	~SimpleLens();
	OBJECT_DECL(SimpleLens);


	void setColor(const sf::Color& c);
	const sf::Color& getColor() const;

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;

	void setDiameter(float w);
	float getDiameter() const;

	void setRotation(float angle);
	float getRotation() const;

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

	void reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
		ReflectionAndRefractionData& outData) const override;

	class LensShape : public Shape
	{
	public:
		bool getCollisionData(const LightRay& ray,
			float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const override;

		float getFocusLength() const;

		void getPainterVertecies(std::vector < sf::Vertex>& points);



		float m_focusLength;
		float m_diameter;
		sf::Vector2f m_pos;
		float m_angle;


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

		private:

		};

		mutable std::vector<sf::Drawable*> m_tmpDraw;
		LensPainter* m_painter;
		SimpleLens* m_lens;
	};

	LensShape m_shape;





	//float m_n1;
	//float m_n2;



};
