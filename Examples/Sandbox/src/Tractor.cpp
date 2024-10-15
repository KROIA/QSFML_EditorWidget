#include "Tractor.h"

Tractor::Tractor(const sf::ContextSettings& settings,
				 QWidget* qparent,
				 const std::string& name,
				 Objects::GameObjectPtr parent)
	: QObject()
	, GameObject(name)
	, m_keyEventUp(new Components::KeyPressEvent("Forward", sf::Keyboard::Key::W))
	, m_keyEventDown(new Components::KeyPressEvent("Backward", sf::Keyboard::Key::S))
	, m_keyEventLeft(new Components::KeyPressEvent("Left", sf::Keyboard::Key::A))
	, m_keyEventRight(new Components::KeyPressEvent("Right", sf::Keyboard::Key::D))
{
	m_camera = new Objects::CameraWindow(settings, "TractorCamera", qparent);
	addChild(m_camera);
	//m_camera->rotate(90);


	m_frontWheels = new Components::Shape("FrontWheels");
	m_frontWheels->setPoints({
		{-20, -50},
		{20, -50},
		{20, 50},
		{-20, 50}
							 });
	//m_frontWheels->setFillColor(sf::Color::Green);
	//m_frontWheels->setFill(true);
	m_frontWheels->setOutlineColor(sf::Color::Black);
	m_frontWheels->setOutlineThickness(2);
	m_frontWheels->setPosition(0, -m_d1);
	addComponent(m_frontWheels);

	m_body = new Components::Shape("Body");
	m_body->setPoints({
		{-20, -50},
		{20, -50},
		{20, 50},
		{-20, 50},
		{-20, 5},
		{-m_d1, 5},
		{-m_d1, -5},
		{-20, -5}
					  });
	//m_body->setFillColor(sf::Color::Red);
	//m_body->setFill(true);
	m_body->setOutlineColor(sf::Color::Black);
	m_body->setOutlineThickness(2);
	m_body->setPosition(0, 0);
	m_body->setRotation(+90);
	addComponent(m_body);

	m_back = new Components::Shape("Back");
	m_back->setPoints({
		{-10, -10},
		{m_d2, -10},
		{m_d2, 0},
		{m_d2, 10},
		{-10, 10}
					  });
	addComponent(m_back);
	m_back->setOutlineColor(sf::Color::Black);
	m_back->setOutlineThickness(2);
	m_back->setRotation(90);

	m_line = new Components::LinePainter("Line");
	addComponent(m_line);
	m_line->ignoreTransform(true);
	m_line->setThickness(2);


	connect(m_keyEventUp, &Components::KeyPressEvent::down, this, &Tractor::onKeyPressUp);
	connect(m_keyEventDown, &Components::KeyPressEvent::down, this, &Tractor::onKeyPressDown);
	connect(m_keyEventLeft, &Components::KeyPressEvent::down, this, &Tractor::onKeyPressLeft);
	connect(m_keyEventRight, &Components::KeyPressEvent::down, this, &Tractor::onKeyPressRight);


	addComponent(m_keyEventUp);
	addComponent(m_keyEventDown);
	addComponent(m_keyEventLeft);
	addComponent(m_keyEventRight);


}

void Tractor::onAwake()
{
	m_camera->setPosition(getPosition());
}

void Tractor::update()
{
	float deltaT = getDeltaT();


	double omega = m_u * m_velocity / 100;
	m_alpha += omega * deltaT;
	double dDt = m_alpha * m_velocity/100 - omega * m_d2;
	m_d += dDt * deltaT;

	setRotation(-m_alpha);
	sf::Vector2f v = QSFML::VectorMath::getRotatedUnitVector((float)((getRotation()-90) * M_PI / 180.f)) * m_velocity;
	sf::Vector2f deltaPos = v * deltaT;
	move(deltaPos);
	logInfo("d=" + std::to_string(m_d));
	
	m_frontWheels->setRotation(-m_u + 90);
	
	m_u *= 0.9;
	m_velocity *= 0.9;


	sf::Vector2f startPos;
	//sf::Vector2f endPos = startPos + sf::Vector2f(m_d/100, 0);
	sf::Vector2f endPos = m_back->getTransformedPoint(2);
	startPos.y = endPos.y;
	startPos.x = -m_d/100;

	//endPos = transform.transformPoint(endPos);
	//startPos.y = endPos.y;
	//endPos.x = -endPos.x;

	m_line->setPoints(startPos, endPos);
}


		
void Tractor::onKeyPressUp()
{
	m_velocity += getDeltaT() * 10000;
}
void Tractor::onKeyPressDown()
{
	m_velocity -= getDeltaT() * 10000;
}
void Tractor::onKeyPressLeft()
{
	if(m_u < 45)
		m_u += getDeltaT() * 1000;
}
void Tractor::onKeyPressRight()
{
	if (m_u > -45)
		m_u -= getDeltaT() * 1000;
}