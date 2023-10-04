#pragma once

#include "QSFML_EditorWidget.h"

struct LightRay
{
	QSFML::Utilities::Ray ray;


	LightRay& operator=(const LightRay& other)
	{
		ray = other.ray;

		return *this;
	}
};
