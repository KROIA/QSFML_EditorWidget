#include "components/drawable/Text.h"
#include "objects/base/GameObject.h"
#include "utilities/AABB.h"
#include "utilities/Origin.h"
#include "Scene/Scene.h"

namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(Text);
        Text::Text(const std::string& name)
            : Drawable(name)
            , m_origin(Utilities::Origin::Type::TopLeft)
        {
            setFont(Scene::getDefaultTextFont());
            setText(name);
        }
        Text::Text(const Text &other)
            : Drawable(other)
            , m_origin(other.m_origin)
        {
            m_text = other.m_text;
            updateCenter(other.getPosition());
        }
        Text::~Text()
        {
            delete m_selfOwnedFont;
        }
        void Text::setText(const std::string &text)
        {
            m_text.setString(text);
            m_needsGeometryUpdate = true;
            //updateCenter(getPosition());
        }
        std::string Text::getText() const
        {
            return m_text.getString();
        }

        void Text::setCharacterSize(unsigned int size)
        {
            m_text.setCharacterSize(size);
            m_needsGeometryUpdate = true;
            //updateCenter(getPosition());
        }
        unsigned int Text::getCharacterSize() const
        {
            return m_text.getCharacterSize();
        }

        void Text::setScale(float scale)
        {
            m_text.setScale(sf::Vector2f(scale, scale));
            m_needsGeometryUpdate = true;
           // updateCenter(getPosition());
        }
        float Text::getScale() const
        {
            return m_text.getScale().x;
        }       
       

        void Text::setFont(const sf::Font& font)
        {
            if (m_selfOwnedFont != &font)
            {
                delete m_selfOwnedFont;
                m_selfOwnedFont = nullptr;
            }
            m_text.setFont(font);
            m_needsGeometryUpdate = true;
           // updateCenter(getPosition());
        }
        bool Text::setFont(const std::string& path)
        {
            sf::Font *f = new sf::Font();
            if (f->loadFromFile(path))
            {
                setFont(*f);
                m_selfOwnedFont = f;
                return true;
            }
            delete f;
            return false;
        }
        const sf::Font& Text::getFont() const
        {
            return *m_text.getFont();
        }

        void Text::setParent(Objects::GameObjectPtr parent)
        {
            Component::setParent(parent);
            /*if (!m_font)
            if(getParent())
            {
                setFont(getParent()->getTextFont());
            }*/
        }

        void Text::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
        {
            updateGeometry();
            states.transform *= getTransform();
            target.draw(m_text, states);
        }

        void Text::updateCenter(const sf::Vector2f &pos) const
        {
            QSFML_UNUSED(pos);
            
            sf::FloatRect bounds = m_text.getLocalBounds();
            Utilities::AABB box(bounds.left, bounds.top, bounds.width, bounds.height);
    
            sf::Vector2f originPos = m_origin.getOrigin(box);
            m_text.setOrigin(originPos);
        }
        void Text::updateGeometry() const
        {
            updateCenter(getPosition());
            float angle = getRotation() * 180.f / M_PI;
            m_text.setRotation(angle);
            m_needsGeometryUpdate = false;
        }

    }
}
