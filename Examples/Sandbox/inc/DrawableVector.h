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
            m_vec1 = new QSFML::Components::VectorPainter("vec1");
            m_vec2 = new QSFML::Components::VectorPainter("vec2");
            m_vec3 = new QSFML::Components::VectorPainter("vec3");

            m_vec1->setStart(sf::Vector2f(0,0));
            m_vec1->setEnd(sf::Vector2f(0,0));
            m_vec1->setColor(sf::Color::Blue);

            m_vec2->setStart(m_vec1->getEnd());
            m_vec2->setDirection(sf::Vector2f(10,10));
            m_vec2->setColor(sf::Color::Red);

            m_vec3->setStart(sf::Vector2f(0, 0));
            m_vec3->setDirection(sf::Vector2f(20, 0));
            m_vec3->setColor(sf::Color::Yellow);

            addComponent(m_vec1);
            addComponent(m_vec2);
            addComponent(m_vec3);


            MouseFollower *follower = new MouseFollower("MouseFollower");
            follower->m_vec = this;

            m_image = new QSFML::Components::Image("hund.png", "Image");
            m_image->setScale(0.1,0.1);
            m_image->setOrigin(256, 256);
            
            
            addComponent(follower);
           // addComponent(m_image);
        }
        void update() override
        {}

        friend MouseFollower;

    private:
        QSFML::Components::VectorPainter *m_vec1, *m_vec2, *m_vec3;
        QSFML::Components::Image* m_image;

        QSFML::Utilities::Ray m_ray1;
        QSFML::Utilities::Ray m_ray2;

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
                            m_vec->m_vec1->setStart(pos);
                            m_vec->m_vec2->setStart(pos);

                            m_vec->m_image->setPosition(pos);

                            m_vec->m_ray1.setPos(m_vec->m_vec1->getStart());
                            m_vec->m_ray1.setDirection(m_vec->m_vec1->getDirection());

                            m_vec->m_ray2.setPos(m_vec->m_vec2->getStart());
                            m_vec->m_ray2.setDirection(m_vec->m_vec2->getDirection());
                            m_vec->m_vec3->setAngle(m_vec->m_ray1.getAngle(m_vec->m_ray2));
          
                        }
                    }
                }

                VectorDisplayer *m_vec;
        };
};
