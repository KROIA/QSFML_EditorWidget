#include "utilities/Color.h"


namespace QSFML
{
	namespace Color
	{
		sf::Color lerpLinear(const sf::Color& color1, const sf::Color& color2, float t)
		{
			return sf::Color(
				static_cast<sf::Uint8>(color1.r + (color2.r - color1.r) * t),
				static_cast<sf::Uint8>(color1.g + (color2.g - color1.g) * t),
				static_cast<sf::Uint8>(color1.b + (color2.b - color1.b) * t),
				static_cast<sf::Uint8>(color1.a + (color2.a - color1.a) * t)
			);
		}
		sf::Color lerpLinear(const std::vector<sf::Color>& colors, float t)
		{
			if (colors.size() == 0)
				return sf::Color::Black;
			if (colors.size() == 1)
				return colors[0];
			if (t <= 0)
				return colors[0];
			if (t >= 1)
				return colors[colors.size() - 1];
			float step = 1.f / (colors.size() - 1);
			int index = static_cast<int>(t / step);
			float localT = (t - index * step) / step;
			return lerpLinear(colors[index], colors[index + 1], localT);
		}

		sf::Color lerpCubic(const sf::Color& color1, const sf::Color& color2, float t)
		{
			float t2 = t * t;
			float t3 = t2 * t;
			float a0 = 2 * t3 - 3 * t2 + 1;
			//float a1 = t3 - 2 * t2 + t;
			//float a2 = t3 - t2;
			float a3 = -2 * t3 + 3 * t2;
			return sf::Color(
				static_cast<sf::Uint8>(color1.r * a0 + color2.r * a3),
				static_cast<sf::Uint8>(color1.g * a0 + color2.g * a3),
				static_cast<sf::Uint8>(color1.b * a0 + color2.b * a3),
				static_cast<sf::Uint8>(color1.a * a0 + color2.a * a3)
			);
		}

		sf::Color lerpCubic(const std::vector<sf::Color>& colors, float t)
		{
			if (colors.size() == 0)
				return sf::Color::Black;
			if (colors.size() == 1)
				return colors[0];
			if (t <= 0)
				return colors[0];
			if (t >= 1)
				return colors[colors.size() - 1];
			float step = 1.f / (colors.size() - 1);
			int index = static_cast<int>(t / step);
			float localT = (t - index * step) / step;
			return lerpCubic(colors[index], colors[index + 1], localT);
		}
	}
}
