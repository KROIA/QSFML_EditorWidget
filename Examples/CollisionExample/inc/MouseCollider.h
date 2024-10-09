#pragma once
#include "QSFML_EditorWidget.h"
#include <QObject>

class MouseCollider : public QObject, public QSFML::Objects::GameObject
{
	Q_OBJECT
public:
	MouseCollider(const std::string& name = "MouseCollider",
				  QSFML::Objects::GameObjectPtr parent = nullptr);
	MouseCollider(const MouseCollider& other);
    ~MouseCollider();

	CLONE_FUNC_DEC(MouseCollider);

private slots:
	void onMousePosChanged(const sf::Vector2f& worldPos,
						   const sf::Vector2i& pixelPos);
private:
	QSFML::Components::Collider* m_collider;
	QSFML::Components::MouseFollower* m_mouseFollower;
};
