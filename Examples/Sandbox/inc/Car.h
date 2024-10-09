#pragma once

#include "QSFML_EditorWidget.h"
#include <QObject>
using namespace QSFML;

class Car : public QObject, public QSFML::Objects::GameObject
{
	Q_OBJECT
public:
	Car(const sf::ContextSettings& settings,
		QWidget* qparent,
		const std::string& name = "Car",
		Objects::GameObjectPtr parent = nullptr);

	void update() override;


private slots:
	void onKeyPressUp();
	void onKeyPressDown();
	void onKeyPressLeft();
	void onKeyPressRight();
private:

	void onAwake() override;

	class Painter : public QSFML::Components::Drawable
	{
	public:
		Painter();
		~Painter();

		void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;
	
		float m_stearingAngle;
	};

	Painter* m_painter;
	Components::Image* m_image;
	Components::KeyPressEvent* m_keyEventUp;
	Components::KeyPressEvent* m_keyEventDown;
	Components::KeyPressEvent* m_keyEventLeft;
	Components::KeyPressEvent* m_keyEventRight;

	sf::Vector2f m_velocity;
	std::vector<sf::Image*> m_images;
	//sf::Vector2f m_acceleration;

	float m_stearingAngle;
	float m_maxStearingAngle;
	float m_maxSpeed;
	float m_maxAcceleration;
	float m_acceleration;
	float m_accelerationSpeed;
	float m_stearingSpeed;

	Objects::CameraWindow *m_camera = nullptr;
	//Objects::CameraController *m_cameraController = nullptr;

};