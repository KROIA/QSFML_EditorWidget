#pragma once

#include "QSFML_EditorWidget.h"
using namespace QSFML;

class AABBDisplayer: public QSFML::Objects::GameObject
{
        class MouseFollower;
    public:
        AABBDisplayer(float length, const std::string &name = "AABBDisplayer",
                      Objects::GameObjectPtr parent = nullptr)
            : GameObject(name, parent)
        {
			Components::VectorPainter* painter = new Components::VectorPainter();
			painter->setStart(sf::Vector2f(0,0));
			painter->setDirection(sf::Vector2f(length, 0));
			addComponent(painter);

			Components::RectPainter* rectPainter = new RectPainterClone();
			rectPainter->setOutlineColor(sf::Color::Red);
            rectPainter->setOutlineThickness(1);
			addComponent(rectPainter);
        }

        void update() override
        {
			Components::RectPainter* p = getComponent<RectPainterClone>();
			if (!p)
				return;
            rotate(200 * getDeltaT());
            //updateBoundingBox();
            p->setRect(getBoundingBox());
            
        }

        Utilities::AABB getCustomBoundingBox() const override
        {
            Components::VectorPainter* p = getComponent<Components::VectorPainter>();
            if (!p)
                return Utilities::AABB();
			sf::Transform transform = getGlobalTransform();
            sf::Vector2f start = transform.transformPoint(p->getStart());
			sf::Vector2f end = transform.transformPoint(p->getEnd());

			


			return Utilities::AABB(start, end-start);
        }
    private:

        


    class QSFML_EDITOR_WIDGET_EXPORT RectPainterClone : public Components::RectPainter
    {
        public:
        RectPainterClone(const std::string& name = "RectPainterClone")
			: Components::RectPainter(name)
		{
		}
        RectPainterClone(const RectPainterClone& other)
			: Components::RectPainter(other)
		{
		}

        void drawComponent(sf::RenderTarget& target,
                           sf::RenderStates states) const override
        {
            target.draw(m_rectShape);
        }

     
    };
       
};
