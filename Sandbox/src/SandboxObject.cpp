#include "SandboxObject.h"
using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

SandboxObject::SandboxObject(const std::string &name)
    : CanvasObject(name)
{
    m_vec = new DrawableVector();
    addComponent(m_vec);
    m_vec->setPoints(sf::Vector2f(0,0),VectorMath::getUnitVector()*20.f);
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
