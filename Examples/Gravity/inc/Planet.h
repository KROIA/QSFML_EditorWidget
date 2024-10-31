#pragma once

#include "QSFML_EditorWidget.h"
using namespace QSFML;

class Planet : public Objects::GameObject
{
	friend class Painter;
public:
	static constexpr float G = 6.67430f; // scaled G constant

	OBJECT_DECL(Planet);
	Planet(const std::string& name = "Planet", Objects::GameObjectPtr parent = nullptr);
	~Planet();

	void update() override;
	void applyVelocity();

	void setVelocity(const sf::Vector2f &velocity) { m_velocity = velocity; }
	void setMass(float mass);
	float getMass() const { return m_mass; }
	const sf::Vector2f &getVelocity() const { return m_velocity; }

	static const QSFML::vector<Planet*> &getPlanets() { return m_planets; }

	static sf::Vector2f calculateForce(const sf::Vector2f &position);
	static void setEnableCollision(bool enable) { m_enableCollision = enable; }
	static void setWorldBounds(const sf::FloatRect &bounds) 
	{ 
		m_worldBounds = bounds; 
		m_enableWorldBounds = true;
	}
	static void enableWorldBounds(bool enable) { m_enableWorldBounds = enable; }

	float getEnergy() const
	{
		return getKineticEnergy() + getPotentialEnergy();
	}
	float getKineticEnergy() const
	{
		return 0.5f * m_mass * (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
	}
	float getPotentialEnergy() const
	{
		float potentialEnergy = 0;
		for (auto planet : m_planets)
		{
			if (planet != this)
			{
				float distance = VectorMath::getLength(getPosition() - planet->getPosition());
				potentialEnergy += -G * m_mass * planet->getMass() / (distance);
			}
		}
		return potentialEnergy;
	}
	void showPlots(bool show) 
	{ 
		m_showPlots = show;
	}
	bool isShowingPlots() const { return m_showPlots; }
	void drawPlots() const
	{
		m_plotCounter++;

		float energy = getEnergy();
		float kineticEnergy = getKineticEnergy();
		float potentialEnergy = getPotentialEnergy();
		float speed = VectorMath::getLength(m_velocity);
		
		ImGui::Begin(("Planet Info: " + getName()).c_str());
		ImGui::Text("Speed: %f", speed);
		ImGui::Text("Kinetic Energy: %f", kineticEnergy);
		ImGui::Text("Potential Energy: %f", potentialEnergy);
		ImGui::Text("Total Energy: %f", energy);

		if (m_plotCounter % 4 == 0)
		{
			// Shift the data
			memmove(m_speed, &m_speed[1], (plotSize - 1) * sizeof(float));
			memmove(m_kineticEnergy, &m_kineticEnergy[1], (plotSize - 1) * sizeof(float));
			memmove(m_potentialEnergy, &m_potentialEnergy[1], (plotSize - 1) * sizeof(float));
			memmove(m_totalEnergy, &m_totalEnergy[1], (plotSize - 1) * sizeof(float));
		
			// Add the new data
			m_speed[plotSize - 1] = speed;
			m_kineticEnergy[plotSize - 1] = kineticEnergy;
			m_potentialEnergy[plotSize - 1] = potentialEnergy;
			m_totalEnergy[plotSize - 1] = energy;
		}

		// Plot using ImPlot and auto ajust the axis scaling

		ImVec2 plotDimensions = ImGui::GetContentRegionAvail();
		plotDimensions.y *= 0.5f;
		if (ImPlot::BeginPlot("Energy Plot", "Time", "Energy", plotDimensions))
		{
			// Set the y-axis to automatically fit the data
			ImPlot::SetupAxis(ImAxis_Y1, NULL, ImPlotAxisFlags_AutoFit);

			//ImPlot::PlotLine("Kinetic Energy", m_kineticEnergy, plotSize);
			//ImPlot::PlotLine("Potential Energy", m_potentialEnergy, plotSize);
			ImPlot::PlotLine("Total Energy", m_totalEnergy, plotSize);
			ImPlot::EndPlot();
		}

		if (ImPlot::BeginPlot("Speed Plot", "Time", "Speed", plotDimensions))
		{
			// Set the y-axis to automatically fit the data
			ImPlot::SetupAxis(ImAxis_Y1, NULL, ImPlotAxisFlags_AutoFit);

			ImPlot::PlotLine("Speed", m_speed, plotSize);
		
			ImPlot::EndPlot();
		}
		ImGui::End();
	}

private:
	sf::Vector2f calculateForce();
	sf::Vector2f calculateForce(Planet *other);
	


	class Painter : public Components::Drawable
	{
	public:
		//COMPONENT_DECL(Painter);

		Painter(const std::string &name, Planet* planet)
			: Drawable(name)
		{
			m_planet = planet;
		}

		void drawComponent(sf::RenderTarget &target, sf::RenderStates states) const override
		{
			sf::CircleShape shape(m_planet->m_radius);
			shape.setFillColor(sf::Color::Green);
			shape.setOutlineColor(sf::Color(0,200,0));
			shape.setOutlineThickness(3);
			shape.setOrigin(m_planet->m_radius, m_planet->m_radius);
			//shape.setPosition(m_planet->getPosition());
			target.draw(shape, states);
		}
		Planet* m_planet = nullptr;
	};


	Painter* m_painter = nullptr;
	Components::VectorPainter* m_accelerationPainter = nullptr;
	Components::PathPainter* m_pathPainter = nullptr;

	sf::Vector2f m_nextPosition;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_force;
	float m_mass = 1;
	float m_radius = 50;
	bool m_showPlots = false;

	static QSFML::vector<Planet*> m_planets;
	static bool m_enableCollision;
	static sf::FloatRect m_worldBounds;
	static bool m_enableWorldBounds;

	static constexpr size_t plotSize = 1000;

	mutable float m_kineticEnergy[plotSize] = { 0 };
	mutable float m_potentialEnergy[plotSize] = { 0 };
	mutable float m_totalEnergy[plotSize] = { 0 };
	mutable float m_speed[plotSize] = { 0 };
	mutable unsigned int m_plotCounter = 0;

};