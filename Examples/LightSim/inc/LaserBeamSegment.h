#pragma once
#include "QSFML_EditorWidget.h"
#include <QObject>

class LaserBeamSegment: public QSFML::Objects::CanvasObject
{
public:
	LaserBeamSegment(const std::string& name = "LaserBeamSegment",
		CanvasObject* parent = nullptr);
	LaserBeamSegment(const LaserBeamSegment& other);
	~LaserBeamSegment();

	CLONE_FUNC_DEC(LaserBeamSegment);


private:
	QSFML::Utilities::Ray m_ray;
	QSFML::Components::Line* m_line;
};