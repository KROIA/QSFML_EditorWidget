#include "CollisionChecker.h"
#include "MouseCollider.h"
#include "CollisionObject.h"

CLONE_FUNC_IMPL(CollisionChecker);
CollisionChecker::CollisionChecker(const std::string& name, CanvasObject* parent)
	: CanvasObject(name)
{
	MouseCollider* mouse = new MouseCollider();
	CollisionObject* obj = new CollisionObject();

	m_objs.push_back(mouse);
	m_objs.push_back(obj);
	addChilds(m_objs);
}
CollisionChecker::~CollisionChecker()
{

}
void CollisionChecker::update()
{
	for (size_t i = 0; i < m_objs.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objs.size(); ++j)
		{
			if (m_objs[i]->checkCollision(m_objs[j]))
			{
				qDebug()  << getTick() << " Collision";
			}
		}
	}
		
}