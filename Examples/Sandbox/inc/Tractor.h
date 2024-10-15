#pragma once

#include "QSFML_EditorWidget.h"
#include <QObject>
using namespace QSFML;

class Tractor : public QObject, public QSFML::Objects::GameObject
{
	Q_OBJECT
	public:
	Tractor(const sf::ContextSettings& settings,
		QWidget* qparent,
		const std::string& name = "Tractor",
		Objects::GameObjectPtr parent = nullptr);

	void onAwake() override;
	void update() override;


	private slots:
	void onKeyPressUp();
	void onKeyPressDown();
	void onKeyPressLeft();
	void onKeyPressRight();

	private:

	float m_d1 = 200;
	float m_d2 = 100;

	float m_u = 0;
	float m_alpha = 5;

	float m_velocity = 0;
	float m_d = 0;

	Components::KeyPressEvent* m_keyEventUp;
	Components::KeyPressEvent* m_keyEventDown;
	Components::KeyPressEvent* m_keyEventLeft;
	Components::KeyPressEvent* m_keyEventRight;

	Components::Shape* m_frontWheels = nullptr;
	Components::Shape* m_body = nullptr;
	Components::Shape* m_back = nullptr;
	Components::LinePainter* m_line = nullptr;
	Objects::CameraWindow* m_camera = nullptr;
};