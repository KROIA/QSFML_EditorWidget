#pragma once

#include "QSFML_EditorWidget.h"

class VectorDisplayer: public QSFML::Objects::CanvasObject
{
        class MouseFollower;
    public:
        VectorDisplayer(const std::string &name = "VectorDisplayer",
                        CanvasObject *parent = nullptr)
            : CanvasObject(name, parent)
        {
            m_vec1 = new QSFML::Components::DrawableVector("vec1");
            m_vec2 = new QSFML::Components::DrawableVector("vec2");

            m_vec1->setStart(sf::Vector2f(0,0));
            m_vec1->setEnd(sf::Vector2f(10,30));
            m_vec1->setColor(sf::Color::Blue);

            m_vec2->setStart(m_vec1->getEnd());
            m_vec2->setDirection(sf::Vector2f(10,10));
            m_vec2->setColor(sf::Color::Red);

            addComponent(m_vec1);
            addComponent(m_vec2);


            MouseFollower *follower = new MouseFollower("MouseFollower");
            follower->m_vec = this;
            addComponent(follower);
        }
        void update() override
        {}

        friend MouseFollower;

    private:
        QSFML::Components::DrawableVector *m_vec1, *m_vec2;

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
                            sf::Vector2f pos = m_vec->getInWorldSpace(sf::Vector2i(e.mouseMove.x,e.mouseMove.y));
                            m_vec->m_vec1->setEnd(pos);
                            m_vec->m_vec2->setStart(pos);
                        }
                    }
                }

                VectorDisplayer *m_vec;
        };
};
