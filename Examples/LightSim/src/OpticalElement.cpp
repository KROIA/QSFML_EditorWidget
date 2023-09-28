#include "OpticalElement.h"


std::vector<OpticalElement*> OpticalElement::s_opticalElements;
OpticalElement::OpticalElement(const std::string& name)
	: CanvasObject(name)
{
	s_opticalElements.push_back(this);
}
OpticalElement::OpticalElement(const OpticalElement& other)
	: CanvasObject(other)
{
	s_opticalElements.push_back(this);
}
OpticalElement::~OpticalElement()
{
	for (size_t i = 0; i < s_opticalElements.size(); ++i)
	{
		if (s_opticalElements[i] == this)
		{
			s_opticalElements.erase(s_opticalElements.begin() + i);
			return;
		}
	}
}
OBJECT_IMPL(OpticalElement);

const std::vector<OpticalElement*> const& OpticalElement::getOpticalElements()
{
	return s_opticalElements;
}

bool OpticalElement::processLaser(const QSFML::Utilities::Ray& ray, 
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	return false;
}
bool OpticalElement::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
	return false;
}
/*void OpticalElement::update()
{

}*/