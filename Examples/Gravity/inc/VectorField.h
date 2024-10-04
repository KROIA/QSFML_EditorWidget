#pragma once

#include "QSFML_EditorWidget.h"
#include <SFML/OpenGL.hpp>
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

	void setField(const std::vector<FieldElement>& field)
	{
		m_field = field;
	}

	const std::vector<FieldElement>& getField() const
	{
		return m_field;
	}
	FieldElement& getFieldElement(size_t index)
	{
		return m_field[index];
	}
	size_t getFieldSize() const
	{
		return m_field.size();
	}

	// Iterator
	std::vector<FieldElement>::iterator begin()
	{
		return m_field.begin();
	}
	std::vector<FieldElement>::iterator end()
	{
		return m_field.end();
	}

	// Const Iterator
	std::vector<FieldElement>::const_iterator begin() const
	{
		return m_field.begin();
	}
	std::vector<FieldElement>::const_iterator end() const
	{
		return m_field.end();
	}


	void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		// Apply SFML transform
		glLoadMatrixf(states.transform.getMatrix());
		glBegin(GL_LINES);
		for (const auto& el : m_field)
		{
			if (isVisible(el.position, target) || isVisible(el.position + el.direction, target))
			{
				glColor4ub(el.color.r, el.color.g, el.color.b, el.color.a);
				glVertex2f(el.position.x, el.position.y);
				glVertex2f(el.position.x + el.direction.x, el.position.y + el.direction.y);
			}
		}
		glEnd();
	}

	private:


	std::vector<FieldElement> m_field;
};