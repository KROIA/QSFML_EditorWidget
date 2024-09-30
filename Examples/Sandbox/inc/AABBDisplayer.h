/*#pragma once

#include "QSFML_EditorWidget.h"

class AABBDisplayer: public QSFML::Objects::GameObject
{
        class MouseFollower;
    public:
        AABBDisplayer(const std::string &name = "AABBDisplayer",
                        GameObject *parent = nullptr)
            : GameObject(name, parent)
        {
            m_box = new QSFML::Objects::DrawableBoxCollider("Box1",QSFML::Utilities::AABB(0,0,100,50));
            m_clickable  = new QSFML::Objects::DrawableBoxCollider("Box2",QSFML::Utilities::AABB(200,200,100,200));

            m_clickable->setColor(sf::Color::Red);

            addChild(m_box);
            addChild(m_clickable);

            MouseFollower *follower = new MouseFollower("MouseFollower");
            follower->m_AABB = this;
            addComponent(follower);
        }

        void update() override
        {}

        friend MouseFollower;

    private:
        QSFML::Objects::DrawableBoxCollider *m_box, *m_clickable;

        class MouseFollower : public QSFML::Components::SfEventHandle
        {
            public:
                MouseFollower(const std::string &name = "MouseFollower")
                    : SfEventHandle(name)
                {

                }
                void sfEvent(const sf::Event &e) override
                {
                    switch(e.type)
                    {
                        case sf::Event::MouseMoved:
                        {
                            sf::Vector2f pos = m_AABB->getInWorldSpace(sf::Vector2i(e.mouseMove.x,e.mouseMove.y));
                            QSFML::Components::BoxCollider *box = m_AABB->m_box->getBox();
                            box->setCenterPos(pos);
                            break;
                        }
                        case sf::Event::MouseButtonPressed:
                        {
                            sf::Vector2f pos = m_AABB->getInWorldSpace(sf::Vector2i(e.mouseButton.x,e.mouseButton.y));
                            QSFML::Components::BoxCollider *box = m_AABB->m_clickable->getBox();
                            //if(box->intersects(*(m_AABB->m_box->getBox())))
                            if(box->contains(pos))
                            {
                                m_AABB->m_clickable->setColor(sf::Color::Green);

                            }
                            break;
                        }
                        case sf::Event::MouseButtonReleased:
                        {
                            m_AABB->m_clickable->setColor(sf::Color::Red);
                            break;
                        }
                    }
                }

                AABBDisplayer *m_AABB;
        };
};
*/
