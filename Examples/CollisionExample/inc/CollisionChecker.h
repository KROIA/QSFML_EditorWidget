#pragma once
#include "QSFML_EditorWidget.h"


class CollisionChecker : public QSFML::Objects::CanvasObject
{
public:
	CollisionChecker(const std::string& name = "CollisionChecker",
					CanvasObject* parent = nullptr);
    ~CollisionChecker();

	CLONE_FUNC_DEC(CollisionChecker);

private:
	void update() override;


	QSFML::Components::Collider* m_collider;
	std::vector<QSFML::Objects::CanvasObject*> m_objs;
};