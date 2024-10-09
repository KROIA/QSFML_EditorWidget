/*#include "objects/DrawableBoxCollider.h"
namespace QSFML
{
namespace Objects
{
DrawableBoxCollider::DrawableBoxCollider(const std::string &name)
    : GameObject(name)
{
    setup(Utilities::AABB(0,0,100,50));
}
DrawableBoxCollider::DrawableBoxCollider(const std::string &name,
                                         const Utilities::AABB &box)
    : GameObject(name)
{
    setup(box);
}
DrawableBoxCollider::DrawableBoxCollider(const DrawableBoxCollider &other)
    : GameObject(other)
{
    m_pos    = new QSFML::Components::DrawableVector(*other.m_pos   );
    m_left   = new QSFML::Components::DrawableVector(*other.m_left  );
    m_top    = new QSFML::Components::DrawableVector(*other.m_top   );
    m_right  = new QSFML::Components::DrawableVector(*other.m_right );
    m_bottom = new QSFML::Components::DrawableVector(*other.m_bottom);

    m_box = new Components::BoxCollider(*other.m_box);
    setColor(sf::Color::Green);

    addComponent(m_pos);
    addComponent(m_left);
    addComponent(m_top);
    addComponent(m_right);
    addComponent(m_bottom);
    addComponent(m_box);
}
void DrawableBoxCollider::setup(const Utilities::AABB &box)
{
    m_pos    = new QSFML::Components::DrawableVector("pos");
    m_left   = new QSFML::Components::DrawableVector("left");
    m_top    = new QSFML::Components::DrawableVector("top");
    m_right  = new QSFML::Components::DrawableVector("right");
    m_bottom = new QSFML::Components::DrawableVector("bottom");

    m_box = new Components::BoxCollider("Box",box);
    setColor(sf::Color::Green);

    addComponent(m_pos);
    addComponent(m_left);
    addComponent(m_top);
    addComponent(m_right);
    addComponent(m_bottom);
    addComponent(m_box);
}
DrawableBoxCollider::~DrawableBoxCollider()
{}


void DrawableBoxCollider::setColor(const sf::Color &color)
{
    m_color = color;
    m_bottom->setColor(m_color);
    m_right->setColor(m_color);
    m_top->setColor(m_color);
    m_left->setColor(m_color);
    m_pos->setColor(m_color);
}
const sf::Color &DrawableBoxCollider::getColor() const
{
    return m_color;
}

Components::BoxCollider *DrawableBoxCollider::getBox() const
{
    return m_box;
}
void DrawableBoxCollider::update()
{
    m_pos->setEnd(m_box->TL());
    m_left->setPoints(m_box->TL(),   m_box->BL());
    m_top->setPoints(m_box->TL(),    m_box->TR());
    m_right->setPoints(m_box->TR(),  m_box->BR());
    m_bottom->setPoints(m_box->BL(), m_box->BR());
}

}
}*/
