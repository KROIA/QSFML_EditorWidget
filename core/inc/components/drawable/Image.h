#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_EXPORT Image : public Drawable
        {
        public:
            Image(const std::string& name = "Image");
            Image(const sf::Texture* t, const std::string& name = "Image");
            Image(const std::string &imagePath, const std::string& name = "Image");
            Image(const Image& other);
            ~Image();
            COMPONENT_DECL(Image);

            void setTexture(const sf::Texture* t);
            const sf::Texture* getTexture() const;

            bool loadFromFile(const std::string& imagePath);
			void loadFromImage(const sf::Image& image);

            void drawComponent(sf::RenderTarget& target,
                sf::RenderStates states) const override;

            sf::FloatRect getLocalBounds() const
			{
				return m_sprite->getLocalBounds();
			}
      
        private:
            void createSprite();

            sf::Sprite* m_sprite;
            const sf::Texture* m_texture;
            sf::Texture* m_selfOwnedTexture;
        };
    }
}
