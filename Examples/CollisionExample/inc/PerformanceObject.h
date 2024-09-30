#pragma once
#include "QSFML_EditorWidget.h"


class PerformanceObject : public QSFML::Objects::GameObject
{
public:
	PerformanceObject(const std::string& name = "PerformanceObject",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	PerformanceObject(const std::vector<sf::Vector2f>& vertecies,
		const std::string& name = "PerformanceObject",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	~PerformanceObject();

	CLONE_FUNC_DEC(PerformanceObject);

	void setRange(const QSFML::Utilities::AABB& range);
	void setDynamic(bool isDynamic);
private:
	void update() override; 
	QSFML::Components::Collider* m_collider;
	sf::Vector2f m_velocity;
	float m_currentDirection;
	bool m_dynamic;
	//sf::Vector2f m_acceletation;

	QSFML::Utilities::AABB m_range;
};
