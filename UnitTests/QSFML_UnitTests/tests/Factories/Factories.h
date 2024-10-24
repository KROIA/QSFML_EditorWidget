#pragma once
#include "QSFML_EditorWidget.h"


namespace Factories
{



	QSFML::vector<sf::Vector2f> randomShapePoints(
		float radius,
		size_t vertecies);

	QSFML::Components::Shape* randomShape(
		float radius,
		const sf::Color& color,
		size_t vertecies);

	QSFML::Objects::GameObjectPtr randomShapeObject(
		const sf::Vector2f& position,
		float radius,
		const sf::Color& color,
		size_t vertecies);

	QSFML::Objects::GameObjectPtr randomLinesObject(
		size_t lines);

	QSFML::Objects::GameObjectPtr randomPathObject(
		size_t length);

	QSFML::Objects::GameObjectPtr randomPointsObject(
		size_t points);
}