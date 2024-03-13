#include "SandboxObject.h"

#include <QDebug>
using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

OBJECT_IMPL(SandboxObject)
SandboxObject::SandboxObject(const std::string &name)
    : CanvasObject(name)
{
    m_vec = new VectorPainter();
    addComponent(m_vec);
    m_vec->setPoints(sf::Vector2f(0,0),VectorMath::getUnitVector()*20.f);

    Components::Button *button = new Components::Button();
    button->setSize(sf::Vector2f(50,50));
    addComponent(button);
    connect(button, &Components::Button::fallingEdge, this, &SandboxObject::onButtonPress);


}
SandboxObject::SandboxObject(const SandboxObject &other)
    : CanvasObject(other)
{

}
SandboxObject::~SandboxObject()
{

}

void SandboxObject::update()
{
    m_vec->setDirection(VectorMath::getRotated(m_vec->getDirection(), 0.1));
}

void SandboxObject::onButtonPress()
{
    qDebug() << "Press";
}
