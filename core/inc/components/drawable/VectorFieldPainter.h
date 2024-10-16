#pragma once

#include "QSFML_EditorWidget.h"
#include <vector>

namespace QSFML
{
	namespace Components
	{

		class VectorFieldPainter : public Components::Drawable
		{
		public:
			COMPONENT_DECL(VectorFieldPainter);
			struct Element
			{
				sf::Vector2f position;
				sf::Vector2f direction;
				sf::Color color;
			};

			VectorFieldPainter(const std::string& name = "VectorFieldPainter");

			void setField(const QSFML::vector<Element>& field)
			{
				m_field = field;
			}
			void reserve(size_t size)
			{
				m_field.reserve(size);
			}
			QSFML::vector<Element>& getField()
			{
				return m_field;
			}
			void clear()
			{
				m_field.clear();
			}

			const QSFML::vector<Element>& getField() const
			{
				return m_field;
			}
			Element& getFieldElement(size_t index)
			{
				return m_field[index];
			}
			size_t getFieldSize() const
			{
				return m_field.size();
			}

			// Iterator
			QSFML::vector<Element>::iterator begin()
			{
				return m_field.begin();
			}
			QSFML::vector<Element>::iterator end()
			{
				return m_field.end();
			}

			// Const Iterator
			QSFML::vector<Element>::const_iterator begin() const
			{
				return m_field.begin();
			}
			QSFML::vector<Element>::const_iterator end() const
			{
				return m_field.end();
			}


			void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;

		private:
			QSFML::vector<Element> m_field;
		};
	}
}
