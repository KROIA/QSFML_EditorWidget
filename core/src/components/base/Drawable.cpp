#include "components/base/Drawable.h"
#include "canvas/Canvas.h"

using namespace QSFML::Components;

Drawable::Drawable(const std::string &name)
    : Component(name)
{

}
Drawable::Drawable(const Drawable &other)
    : Component(other)
{

}
Drawable::~Drawable()
{

}