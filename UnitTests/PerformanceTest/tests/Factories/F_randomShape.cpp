#include "F_randomShape.h"

namespace Factories
{
	std::vector<sf::Vector2f> randomShapePoints(
		float radius,
		size_t vertecies)
	{
		std::vector<sf::Vector2f> v;
		v.reserve(vertecies);
		float rad = 0;
		float dRad = M_PI * 2 / (float)vertecies;
		for (size_t i = 0; i < vertecies; ++i)
		{
			sf::Vector2f rotated(sin(rad) * radius, cos(rad) * radius);
			sf::Vector2f randomVec = QSFML::Utilities::RandomEngine::getVector() * radius;

			v.push_back(rotated + randomVec);
			rad += dRad;
		}
		return v;
	}


	QSFML::Components::Shape* randomShape(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color,
		size_t vertecies)
	{
		QSFML::Components::Shape *shape = new QSFML::Components::Shape();
		shape->setPoints(randomShapePoints(radius, vertecies));
		return shape;
	}


	QSFML::Objects::CanvasObject* randomShapeObject(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color,
		size_t vertecies)
	{
		// Create compontents
		std::vector<sf::Vector2f> points = randomShapePoints(radius, vertecies);
		QSFML::Components::Collider* collider = new QSFML::Components::Collider();
		collider->setVertecies(points);

		QSFML::Components::Collider::Painter* painter = collider->createPainter();
		painter->setColor(color);

		// Create object
		QSFML::Objects::CanvasObject* obj = new QSFML::Objects::CanvasObject("RandomShape");

		// Add components
		obj->addComponent(collider);
		obj->addComponent(painter);

		obj->setPosition(position);
		return obj;
	}
}