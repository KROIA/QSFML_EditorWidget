#include "LaserBeamSegment.h"

CLONE_FUNC_IMPL(LaserBeamSegment);

LaserBeamSegment::LaserBeamSegment(const std::string& name, CanvasObject* parent)
	: CanvasObject(name)
{
	

}
LaserBeamSegment::LaserBeamSegment(const LaserBeamSegment& other)
	: CanvasObject(other)
{

}
LaserBeamSegment::~LaserBeamSegment()
{

}