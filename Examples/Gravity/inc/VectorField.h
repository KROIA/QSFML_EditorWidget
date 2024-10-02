#pragma once

#include "QSFML_EditorWidget.h"
#include <unordered_map>
using namespace QSFML;

class VectorField : public Components::Drawable
{
public:
	//COMPONENT_DECL(VectorField);
	struct FieldElement
	{
		sf::Vector2f position;
		sf::Vector2f direction;
		sf::Color color;
	};

	VectorField(const std::string& name = "VectorField")
		: Drawable(name)
	{
		
	}

	void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for(auto& el : m_field)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(el.position, el.color),
				sf::Vertex(el.position + el.direction, el.color)
			};
			target.draw(line, 2, sf::Lines);
		}
	}

	std::vector<FieldElement> m_field;
};