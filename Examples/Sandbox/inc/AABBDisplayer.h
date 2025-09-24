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

            Components::PathPainter *pathPainter = new Components::PathPainter();
            pathPainter->setColor(sf::Color::Green);
            pathPainter->ignoreTransform(true);
            addComponent(pathPainter);

            setCustomBoundingBoxFunction([this]() {
                Components::VectorPainter* p = getFirstComponent<Components::VectorPainter>();
                if (!p)
                    return Utilities::AABB();
                sf::Transform transform = getGlobalTransform();
                sf::Vector2f start = transform.transformPoint(p->getStart());
                sf::Vector2f end = transform.transformPoint(p->getEnd());

                return Utilities::AABB(start, end - start);
                });
        }

        void update() override
        {
            rotate(200 * getDeltaT());

            Components::RectPainter* rectPainter = getFirstComponent<RectPainterClone>();
            rectPainter->setRect(getBoundingBox());
            

            Components::PathPainter *pathPainter = getFirstComponent<Components::PathPainter>();
            Components::VectorPainter *vectorPainter = getFirstComponent<Components::VectorPainter>();
            sf::Transform transform = getGlobalTransform();
            pathPainter->appenPoint(transform.transformPoint(vectorPainter->getEnd()));
            if (pathPainter->getPointCount() > 1000)
            {
                pathPainter->popPointAtStart();
            }

        }

    private:

        


    class QSFML_EDITOR_WIDGET_API RectPainterClone : public Components::RectPainter
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
