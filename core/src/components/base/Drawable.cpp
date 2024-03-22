#include "components/base/Drawable.h"
#include "canvas/Canvas.h"

using namespace QSFML::Components;

Drawable::Drawable(const std::string &name)
    : Component(name)
    , m_position(0, 0)
{

}
Drawable::Drawable(const Drawable &other)
    : Component(other)
    , m_position(other.m_position)
{

}
Drawable::~Drawable()
{

}
