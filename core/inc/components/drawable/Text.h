#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include "SFML/Graphics/Text.hpp"

#include "utilities/Origin.h"

namespace QSFML
{
namespace Components
{

class QSFML_EDITOR_WIDGET_EXPORT Text : public Drawable
{
    public:
        /**
         * \brief Text constructor
         * \param name of this Component
         * \see Component::setName()
         */
        Text(const std::string &name = "Text");
        Text(const Text &other);
        COMPONENT_DECL(Text);
        ~Text();

        void setText(const std::string &text);
        std::string getText() const;

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;

        void setScale(float scale);
        float getScale() const;

        void setOrigin(const Utilities::Origin& origin)
        {
            m_origin = origin;
            updateCenter(getPosition());
        }
        const Utilities::Origin& getOrigin() const
        {
            return m_origin;
        }

        void setFont(const sf::Font& font);
        bool setFont(const std::string& path);
        const sf::Font& getFont() const;

       



        void setParent(Objects::GameObjectPtr parent) override;

        /**
         * \brief draw
         * \details This function will be automaticlly called from the
         *          Scene once per frame if the Component is enabled
         * \param target to draw on
         * \param states
         */
        void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;

    protected:
        void updateCenter(const sf::Vector2f &pos) const;
        void updateGeometry() const;

        mutable bool m_needsGeometryUpdate = false;
        mutable sf::Text m_text;
        //sf::Vector2f m_pos;
        Utilities::Origin m_origin;
        const sf::Font* m_selfOwnedFont = nullptr;
};
}
}
