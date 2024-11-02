#include "Car.h"


Car::Car(const sf::ContextSettings& settings,
		 QWidget* qparent, 
		 const std::string& name, 
		 Objects::GameObjectPtr parent)
	: QObject()
	, GameObject(name)
	, m_painter(new Painter())
	, m_image(new Components::Image("CarImage", "Resources/Car.png"))
	, m_keyEventUp(new Components::KeyPressEvent("Forward", sf::Keyboard::Key::W))
	, m_keyEventDown(new Components::KeyPressEvent("Backward", sf::Keyboard::Key::S))
	, m_keyEventLeft(new Components::KeyPressEvent("Left", sf::Keyboard::Key::A))
	, m_keyEventRight(new Components::KeyPressEvent("Right", sf::Keyboard::Key::D))
{
	add(m_image);
	add(m_painter);
	addComponent(m_painter);
	add(m_keyEventUp);
	add(m_keyEventDown);
	add(m_keyEventLeft);
	add(m_keyEventRight);
	


	m_camera = new Objects::CameraWindow(settings, "CustomCamera2", qparent);
	sf::View view(sf::FloatRect(0, 0, 1000, 1000));
	m_camera->setThisCameraView(view);
	m_camera->setPosition(0, 0);
	add(m_camera);
	

	m_image->setScale(0.5, 0.5);

	connect(m_keyEventUp, &Components::KeyPressEvent::down, this, &Car::onKeyPressUp);
	connect(m_keyEventDown, &Components::KeyPressEvent::down, this, &Car::onKeyPressDown);
	connect(m_keyEventLeft, &Components::KeyPressEvent::down, this, &Car::onKeyPressLeft);
	connect(m_keyEventRight, &Components::KeyPressEvent::down, this, &Car::onKeyPressRight);

	

	m_stearingAngle = 0;
	m_maxStearingAngle = 1000;
	m_maxSpeed = 5000;
	m_maxAcceleration = 5000;
	m_velocity = sf::Vector2f(0, 0);
	m_acceleration = 0;
	m_accelerationSpeed = 100;
	m_stearingSpeed = 500;
	//m_acceleration = sf::Vector2f(0, 0);

	enableDrawGizmosRecursive(true);

	setRenderLayer(RenderLayer::layer_0);
}

void Car::onAwake()
{
	//m_camera = findFirstObjectGlobalRecursive<Objects::CameraWindow>("CustomCamera2");
}


void Car::update()
{
	float deltaT = getDeltaT();

	m_acceleration = std::min(m_acceleration, m_maxAcceleration);
	m_acceleration = std::max(m_acceleration, -m_maxAcceleration);
	m_stearingAngle *= std::min(std::abs(m_acceleration * deltaT*0.01f), 1.f);
	m_stearingAngle = std::max(m_stearingAngle, -m_maxStearingAngle);
	m_stearingAngle = std::min(m_stearingAngle, m_maxStearingAngle);

	GameObject::rotate(m_stearingAngle * deltaT*0.1);
	m_painter->m_stearingAngle = m_stearingAngle * deltaT * 0.5;
	if(m_acceleration < 0)
		m_painter->m_stearingAngle = -m_painter->m_stearingAngle;
	
	sf::Vector2f deltaPos = QSFML::VectorMath::getRotatedRAD(sf::Vector2f(0, m_acceleration * deltaT*0.1), getRotation()*M_PI/180.f);
	GameObject::move(deltaPos);
	sf::Vector2f pos = getPosition();
	//logInfo("Position: " + std::to_string(pos.x) + " " + std::to_string(pos.y));
	//logInfo("Rotation: " + std::to_string(getRotation()));
	//logInfo("Stearing: " + std::to_string(m_stearingAngle));

	if (pos.x > 1000)
	{
		pos.x = 999;
		m_acceleration = 0;
		m_stearingAngle = 0;
	}
	if (pos.x < -1000)
	{
		pos.x = -999;
		m_acceleration = 0;
		m_stearingAngle = 0;
	}
	if (pos.y > 1000)
	{
		pos.y = 999;
		m_acceleration = 0;
		m_stearingAngle = 0;
	}
	if (pos.y < -1000)
	{
		pos.y = -999;
		m_acceleration = 0;
		m_stearingAngle = 0;
	}
	setPosition(pos);

	m_stearingAngle = m_stearingAngle *0.9;
	m_acceleration = m_acceleration *0.98;



	if (m_camera)
	{
		//m_camera->setPosition(getPosition());
		//m_camera->setRotation(getRotation());
		m_images.push_back(new sf::Image(m_camera->captureThisCameraScreen()));
		if (m_images.size() > 1)
		{
			delete m_images[0];
			m_images.erase(m_images.begin());
		}
		m_image->loadFromImage(*m_images.back());
		m_image->setPosition(-m_image->getLocalBounds().width * m_image->getScale().x / 2.f,
			-m_image->getLocalBounds().height * m_image->getScale().y / 2.f);
	}

	
}

void Car::onKeyPressUp()
{
	m_acceleration -= m_accelerationSpeed;
	
}
void Car::onKeyPressDown()
{
	m_acceleration += m_accelerationSpeed;
	
}
void Car::onKeyPressLeft()
{
	float dir = 1;
	if (m_acceleration < 0)
		dir = -1;
	m_stearingAngle += m_stearingSpeed * dir;
	
}
void Car::onKeyPressRight()
{
	float dir = 1;
	if (m_acceleration < 0)
		dir = -1;
	m_stearingAngle -= m_stearingSpeed * dir;
	
}



Car::Painter::Painter()
{

}
Car::Painter::~Painter()
{

}

void Car::Painter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Define the main body of the car
	sf::RectangleShape carBody(sf::Vector2f(50, 100));
	carBody.setFillColor(sf::Color::Blue);
	carBody.setOutlineColor(sf::Color::Black);
	carBody.setOutlineThickness(2.f);
	float centerOffset = carBody.getSize().y / 2;
	carBody.setOrigin(carBody.getSize() / 2.f-sf::Vector2f(0, centerOffset));
	carBody.setPosition(0, -centerOffset); // Set the position where the car will be drawn

	// Define the wheels
	int wheelWidth = 5;
	sf::RectangleShape frontWheel(sf::Vector2f(wheelWidth, 30.f));
	frontWheel.setFillColor(sf::Color::Black);
	frontWheel.setOrigin(frontWheel.getSize() / 2.f);
	

	sf::RectangleShape rearWheel(sf::Vector2f(wheelWidth, 30.f));
	rearWheel.setFillColor(sf::Color::Black);
	rearWheel.setOrigin(rearWheel.getSize() / 2.f);
	

	// Set the positions of the wheels relative to the car body
	frontWheel.setPosition(carBody.getPosition().x - wheelWidth - carBody.getSize().x / 2, carBody.getPosition().y - 30.f+ centerOffset);
	rearWheel.setPosition(carBody.getPosition().x + wheelWidth + carBody.getSize().x / 2, carBody.getPosition().y - 30.f+ centerOffset);

	// Rear wheels
	sf::RectangleShape frontRearWheel(frontWheel);
	frontRearWheel.setPosition(carBody.getPosition().x - wheelWidth - carBody.getSize().x/2, carBody.getPosition().y + 30.f+ centerOffset);

	
	sf::RectangleShape backRearWheel(rearWheel);
	backRearWheel.setPosition(carBody.getPosition().x + wheelWidth + carBody.getSize().x / 2, carBody.getPosition().y + 30.f+centerOffset);

	frontWheel.setRotation(-m_stearingAngle);
	rearWheel.setRotation(-m_stearingAngle);
	// Front window
	int windowWidth = 20;
	sf::ConvexShape frontWindow;
	frontWindow.setPointCount(4);
	frontWindow.setPoint(0, sf::Vector2f(-windowWidth, -50));
	frontWindow.setPoint(1, sf::Vector2f(windowWidth, -50));
	frontWindow.setPoint(2, sf::Vector2f(15, -30));
	frontWindow.setPoint(3, sf::Vector2f(-15, -30));
	frontWindow.setFillColor(sf::Color(100, 100, 100, 200));
	frontWindow.setOutlineColor(sf::Color::Black);
	frontWindow.setOutlineThickness(2.f);
	frontWindow.setOrigin(0, 0);
	frontWindow.setPosition(carBody.getPosition().x, carBody.getPosition().y  + carBody.getSize().y / 4+ centerOffset);

	// Rear window
	sf::ConvexShape rearWindow;
	rearWindow.setPointCount(4);
	rearWindow.setPoint(0, sf::Vector2f(-windowWidth, 50));
	rearWindow.setPoint(1, sf::Vector2f(windowWidth, 50));
	rearWindow.setPoint(2, sf::Vector2f(15, 30));
	rearWindow.setPoint(3, sf::Vector2f(-15, 30));
	rearWindow.setFillColor(sf::Color(100, 100, 100, 200));
	rearWindow.setOutlineColor(sf::Color::Black);
	rearWindow.setOutlineThickness(2.f);
	rearWindow.setOrigin(0, 0);
	rearWindow.setPosition(carBody.getPosition().x, carBody.getPosition().y - carBody.getSize().y / 10+ centerOffset);



	// Draw all components
	target.draw(frontWheel, states);
	target.draw(rearWheel, states);
	target.draw(frontRearWheel, states);
	target.draw(backRearWheel, states);
	target.draw(carBody, states);
	target.draw(frontWindow, states);
	target.draw(rearWindow, states);
}