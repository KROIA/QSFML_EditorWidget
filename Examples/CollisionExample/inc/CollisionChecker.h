#pragma once
#include "QSFML_EditorWidget.h"
#include "MouseCollider.h"
#include "CollisionObject.h"
#include "PerformanceObject.h"

class CollisionChecker : public QObject, public QSFML::Objects::CanvasObject
{
	Q_OBJECT
public:
	enum Mode
	{
		intersecting,
		contains,
		performanceTest
	};

	CollisionChecker(const std::string& name = "CollisionChecker",
					CanvasObject* parent = nullptr);
	CollisionChecker(const CollisionChecker &other);
    ~CollisionChecker();

	CLONE_FUNC_DEC(CollisionChecker);

	void setMode(Mode mode);

private slots:
	void onMousePosChanged(const sf::Vector2f& worldPos,
						   const sf::Vector2i& pixelPos);

private:
	void update() override;
	void update_intersecting();
	void update_contains();
	void update_performanceTest();

	Mode m_mode;

	QSFML::Components::PointPainter* m_pointPainter;
	MouseCollider* m_mouseCollider;

	//CollisionObject* m_collisionObject;
	QSFML::Components::MouseFollower* m_mouseFollower;
	sf::Vector2f m_mousePos;
	QSFML::Objects::CanvasObject* m_colliderContainer;
	std::vector<QSFML::Objects::CanvasObject*> m_objs;
	std::vector<QSFML::Objects::CanvasObject*> m_collisionObjs;

	QSFML::Objects::CanvasObject* m_performanceContainer;
	std::vector<QSFML::Objects::CanvasObject*> m_performanceObjs;
};
