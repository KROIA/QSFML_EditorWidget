#include "components/Text.h"
#include "objects/base/CanvasObject.h"

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(Text)
Text::Text(const std::string &name)
    : Drawable(name)
    , m_pos(0,0)
{
    m_text.setScale(0.1f,0.1f);
    setText(name);
}
Text::Text(const Text &other)
    : Drawable(other)
    , m_pos(other.m_pos)
{
    m_text = other.m_text;
    updateCenter(other.m_pos);
}
void Text::setText(const std::string &text)
{
    m_text.setString(text);
    updateCenter(m_pos);
}
std::string Text::getText() const
{
    return m_text.getString();
}

void Text::setCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    updateCenter(m_pos);
}
unsigned int Text::getCharacterSize() const
{
    return m_text.getCharacterSize();
}
void Text::setPosition(const sf::Vector2f &pos)
{
    m_pos = pos;
    updateCenter(m_pos);
}
const sf::Vector2f &Text::getPosition() const
{
    return m_pos;
}

void Text::setParent(Objects::CanvasObject *parent)
{
    Component::setParent(parent);
    if(getParent())
    {
         m_text.setFont(getParent()->getTextFont());
    }
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    target.draw(m_text);
}

void Text::updateCenter(const sf::Vector2f &pos)
{
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(sf::Vector2f(bounds.left + bounds.width/2.f,bounds.top + bounds.height/2.f));

    m_text.setPosition(pos);
}

}
}
