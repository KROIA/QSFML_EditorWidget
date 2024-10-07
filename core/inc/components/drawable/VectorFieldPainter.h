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

			void setField(const std::vector<Element>& field)
			{
				m_field = field;
			}
			void reserve(size_t size)
			{
				m_field.reserve(size);
			}
			std::vector<Element>& getField()
			{
				return m_field;
			}
			void clear()
			{
				m_field.clear();
			}

			const std::vector<Element>& getField() const
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
			std::vector<Element>::iterator begin()
			{
				return m_field.begin();
			}
			std::vector<Element>::iterator end()
			{
				return m_field.end();
			}

			// Const Iterator
			std::vector<Element>::const_iterator begin() const
			{
				return m_field.begin();
			}
			std::vector<Element>::const_iterator end() const
			{
				return m_field.end();
			}


			void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;

		private:
			std::vector<Element> m_field;
		};
	}
}
