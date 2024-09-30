#pragma once
#include "QSFML_EditorWidget.h"


class CollisionObject : public QSFML::Objects::GameObject
{
public:
	CollisionObject(const std::string& name = "MyObject",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	CollisionObject(const std::vector<sf::Vector2f>& vertecies,
					const std::string& name = "MyObject",
		QSFML::Objects::GameObjectPtr parent = nullptr);
    ~CollisionObject();

	virtual CLONE_FUNC_DEC(CollisionObject);

	//QSFML::Components::Collider* getCollider() const;

private:
	QSFML::Components::Collider* m_collider;
};
