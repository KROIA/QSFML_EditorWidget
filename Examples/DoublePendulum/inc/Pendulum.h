#pragma once

#include "QSFML_EditorWidget.h"

class Pendulum : public QSFML::Objects::CanvasObject
{
	struct PendulumData
	{
		double angle = M_PI_2;
		double angleVelocity = 0;
		double angleAcceleration = 0;
		double length = 100;
		double damping = 0.001;

		sf::Vector2f startPos;
		sf::Vector2f endPos;
		QSFML::Components::Line* line = nullptr;
	};
public:
    Pendulum(const std::string& name = "Pendulum",
             CanvasObject* parent = nullptr);
    void update() override;

private:
	void updatePendulum(PendulumData& pendulumData, PendulumData *prev, double dt);
    QSFML::Components::PointPainter *m_pointPainter;
    //QSFML::Components::Line *m_linePainters[2];

	static constexpr size_t m_count = 2;
	PendulumData m_pendulumData[m_count];
	const double m_gravity = 9.81f;
	sf::Vector2f m_origin;
};
