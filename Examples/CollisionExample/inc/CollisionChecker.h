#pragma once
#include "QSFML_EditorWidget.h"
#include "MouseCollider.h"
#include "CollisionObject.h"
#include "PerformanceObject.h"

class CollisionChecker : public QObject, public QSFML::Objects::GameObject
{
	Q_OBJECT
	//class Painter;
	//friend Painter;
public:
	enum Mode
	{
		intersecting,
		contains,
		performanceTest
	};

	CollisionChecker(const std::string& name = "CollisionChecker",
		QSFML::Objects::GameObjectPtr parent = nullptr);
	CollisionChecker(const CollisionChecker &other);
    ~CollisionChecker();

	CLONE_FUNC_DEC(CollisionChecker);

	void setMode(Mode mode);

private slots:
	void onMousePosChanged(const sf::Vector2f& worldPos,
						   const sf::Vector2i& pixelPos);

private:
	void onSceneParentChange(QSFML::Scene* oldParent, QSFML::Scene* newParent) override;
	void onAwake() override;
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
	QSFML::Objects::GameObjectPtr m_colliderContainer;
	std::vector<QSFML::Objects::GameObjectPtr> m_objs;
	std::vector<QSFML::Objects::GameObjectPtr> m_collisionObjs;

	QSFML::Objects::GameObjectPtr m_performanceContainer;
	std::vector<QSFML::Objects::GameObjectPtr> m_performanceObjs;
	QSFML::Utilities::ObjectQuadTree m_tree;
	
/*
	class QSFML_EDITOR_WIDGET_EXPORT Painter : public QSFML::Components::Drawable
	{
	public:
		Painter(CollisionChecker* checker, const std::string& name = "Painter");
		Painter(const Painter& other);
		COMPONENT_DECL(Painter);


		~Painter();


		void drawComponent(sf::RenderTarget& target,
			sf::RenderStates states) const override;

		
	private:
		CollisionChecker* m_ckecker;
	};*/
};
