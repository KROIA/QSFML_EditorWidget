#include "CustomObject.h"

OBJECT_IMPL(CustomObject);

CustomObject::CustomObject()
	: QObject()
	, GameObject("CustomObjectName")
{
	// Create a square shape
	m_shape = new QSFML::Components::Shape();
	m_shape->setPoints({
		{-50, -50},
		{50, -50},
		{50, 50},
		{-50, 50},
		{-50, -50}
					  });
	m_shape->setFillColor(sf::Color::Green);
	m_shape->setFill(true);

	// Create a mouse follower component
	m_mouseFollower = new QSFML::Components::MouseFollower();
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
			this, &CustomObject::mousePosChanged);

	// Add the components to this object.
	addComponent(m_shape);
	addComponent(m_mouseFollower);
}
CustomObject::CustomObject(const CustomObject& other)
	: QObject()
	, GameObject("CustomObjectName")
{
	// Create a square shape
	m_shape = new QSFML::Components::Shape(*other.m_shape);

	// Create a mouse follower component
	m_mouseFollower = new QSFML::Components::MouseFollower();
	connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
			this, &CustomObject::mousePosChanged);

	// Add the components to this object.
	addComponent(m_shape);
	addComponent(m_mouseFollower);
}

CustomObject::~CustomObject()
{
	// Do not delete components which are added to this object.
	// They will be deleted automatically.
}

void CustomObject::mousePosChanged(const sf::Vector2f& worldPos,
					 const sf::Vector2i& pixelPos)
{
	// Set the position of this object to the mouse position
	this->setPosition(worldPos);
}