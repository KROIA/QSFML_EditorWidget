#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace QSFML
{
	namespace Color
	{
		/// <summary>
		/// Interpolates linearly between color1 and color2 with the factor t.
		/// </summary>
		/// <param name="color1">Color for t=0</param>
		/// <param name="color2">Color for t=1</param>
		/// <param name="t">Interpolation parameter. Range: [0 : 1]</param>
		/// <returns>The the interpolated color</returns>
		QSFML_EDITOR_WIDGET_EXPORT sf::Color lerpLinear(const sf::Color& color1, const sf::Color& color2, float t);

		/// <summary>
		/// Interpolates linearly between between the colors in the list using the factor t.
		/// </summary>
		/// <param name="colors">List of colors which are equaly spaced in the t domain</param>
		/// <param name="t">Interpolation parameter. Range: [0 : 1]</param>
		/// <returns>The the interpolated color</returns>
		QSFML_EDITOR_WIDGET_EXPORT sf::Color lerpLinear(const QSFML::vector<sf::Color>& colors, float t);


		/// <summary>
		/// Interpolates cubic between color1 and color2 with the factor t.
		/// </summary>
		/// <param name="color1">Color for t=0</param>
		/// <param name="color2">Color for t=1</param>
		/// <param name="t">Interpolation parameter. Range: [0 : 1]</param>
		/// <returns>The the interpolated color</returns>
		QSFML_EDITOR_WIDGET_EXPORT sf::Color lerpCubic(const sf::Color& color1, const sf::Color& color2, float t);

		/// <summary>
		/// Interpolates cubic between between the colors in the list using the factor t.
		/// </summary>
		/// <param name="colors">List of colors which are equaly spaced in the t domain</param>
		/// <param name="t">Interpolation parameter. Range: [0 : 1]</param>
		/// <returns>The the interpolated color</returns>
		QSFML_EDITOR_WIDGET_EXPORT sf::Color lerpCubic(const QSFML::vector<sf::Color>& colors, float t);
	
	}
}