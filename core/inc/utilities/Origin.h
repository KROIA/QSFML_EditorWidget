#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_API Origin
		{
		public:
			enum Type
			{
				TopLeft,
				TopRight,
				BottomLeft,
				BottomRight,
				Center
			};
			Origin(Type type);
			Origin(const Origin &other);
			~Origin();

			void setType(Type type);
			Type getType() const;

			sf::Vector2f getOrigin(const Utilities::AABB& rect) const;
			static sf::Vector2f getOrigin(const Utilities::AABB& rect, Type type);


		private:
			Type m_type;
		};
	}
}
