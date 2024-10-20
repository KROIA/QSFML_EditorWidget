#pragma once
#include "QSFML_EditorWidget.h"
#include <QObject>

class CustomObject : public QObject, public QSFML::Objects::GameObject
{
	Q_OBJECT
	public:
	OBJECT_DECL(CustomObject);
	CustomObject();
	CustomObject(const CustomObject &other);

	~CustomObject() override;

	private slots:
	void mousePosChanged(const sf::Vector2f& worldPos,
						 const sf::Vector2i& pixelPos);

	private:
	QSFML::Components::Shape *m_shape;
	QSFML::Components::MouseFollower *m_mouseFollower;
};