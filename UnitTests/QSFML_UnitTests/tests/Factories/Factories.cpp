#include "Factories.h"

namespace Factories
{

	QSFML::vector<sf::Vector2f> randomShapePoints(
		float radius,
		size_t vertecies)
	{
		QSFML::vector<sf::Vector2f> v;
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


	QSFML::Objects::GameObjectPtr randomShapeObject(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color,
		size_t vertecies)
	{
		// Create compontents
		QSFML::vector<sf::Vector2f> points = randomShapePoints(radius, vertecies);
		QSFML::Components::Collider* collider = new QSFML::Components::Collider();
		collider->setVertecies(points);

		// Create object
		QSFML::Objects::GameObjectPtr obj = new QSFML::Objects::GameObject("RandomShape");

		// Add components
		obj->addComponent(collider);

		obj->setPosition(position);
		return obj;
	}

	QSFML::Objects::GameObjectPtr randomLinesObject(
		size_t lines)
	{
		QSFML::Objects::GameObjectPtr obj = new QSFML::Objects::GameObject("RandomLineObj");
		QSFML::Components::LinePainter* linePainter = new QSFML::Components::LinePainter();
		obj->addComponent(linePainter);
		for (size_t i = 0; i < lines; ++i)
		{
			sf::Vector2f p1 = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
			sf::Vector2f p2 = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
			linePainter->addLine(p1, p2, sf::Color(rand() % 255, rand() % 255, rand() % 255), rand() % 20);
		}
		return obj;
	}

	QSFML::Objects::GameObjectPtr randomPathObject(
		size_t length)
	{
		QSFML::Objects::GameObjectPtr obj = new QSFML::Objects::GameObject("RandomPathObj");
		QSFML::Components::PathPainter* painter = new QSFML::Components::PathPainter();
		obj->addComponent(painter);
	
		float angle = QSFML::Utilities::RandomEngine::getFloat(0, 2*M_PI);
		QSFML::vector<sf::Vector2f> path;
		path.reserve(length);
		sf::Vector2f lastPos = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
		for (size_t j = 0; j < length; ++j)
		{
			sf::Vector2f dir = sf::Vector2f(cos(angle), sin(angle));
			float length = 2;
			lastPos += dir * length;
			path.push_back(lastPos);
			angle += QSFML::Utilities::RandomEngine::getFloat(-1, 1);
		}
		painter->setColor(QSFML::Utilities::RandomEngine::getColor());
		painter->setPath(path);	
		return obj;
	}

	QSFML::Objects::GameObjectPtr randomPointsObject(
		size_t points)
	{
		QSFML::Objects::GameObjectPtr obj = new QSFML::Objects::GameObject("RandomPointObj");
		QSFML::Components::PointPainter* painter = new QSFML::Components::PointPainter();
		painter->setVerteciesCount(20);
		obj->addComponent(painter);
		
		for (size_t i = 0; i < points; ++i)
		{
			sf::Vector2f p1 = QSFML::Utilities::RandomEngine::getVector() * 1000.f;
			float radius = QSFML::Utilities::RandomEngine::getFloat(1, 10);
			sf::Color color = QSFML::Utilities::RandomEngine::getColor();
			painter->addPoint(p1, radius, color);
		}
		return obj;
	}
}