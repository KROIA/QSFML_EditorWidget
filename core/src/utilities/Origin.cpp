#include "utilities/Origin.h"
#include "utilities/AABB.h"

namespace QSFML
{
	namespace Utilities
	{
		Origin::Origin(Type type)
			: m_type(type)
		{

		}
		Origin::Origin(const Origin &other)
			: m_type(other.m_type)
		{

		}
		Origin::~Origin()
		{

		}

		void Origin::setType(Type type)
		{
			m_type = type;
		}
		Origin::Type Origin::getType() const
		{
			return m_type;
		}

		sf::Vector2f Origin::getOrigin(const Utilities::AABB& rect) const
		{
			return getOrigin(rect, m_type);
		}
		sf::Vector2f Origin::getOrigin(const Utilities::AABB& rect, Type type)
		{
			switch (type)
			{
				case Type::TopLeft:		return rect.TL();
				case Type::TopRight:	return rect.TR();
				case Type::BottomLeft:	return rect.BL();
				case Type::BottomRight:	return rect.BR();
				case Type::Center:		return (rect.TL() + rect.TR() + rect.BL() + rect.BR()) * 0.25f;					
			}
			return sf::Vector2f(0, 0);
		}
	}
}