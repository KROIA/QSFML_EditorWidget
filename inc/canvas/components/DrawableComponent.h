#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include "Component.h"
//#include "DrawInterface.h"

class DrawableComponent : public Component, public sf::Drawable//, protected DrawInterface
{
    public:
        DrawableComponent(const std::string &name = "DrawableComponent");
        ~DrawableComponent();




      //  virtual void draw(const sf::Vector2f &drawPos = sf::Vector2f(0,0)) = 0;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    protected:
        //void setParent(CanvasObject *parent) override;

        /*
        //Draw Interface
        void draw(const sf::Drawable &drawable,
                  const sf::RenderStates &states=sf::RenderStates::Default) final;

        void draw(const sf::Vertex *vertices,
                  std::size_t vertexCount,
                  sf::PrimitiveType type,
                  const sf::RenderStates &states=sf::RenderStates::Default) final;

        void draw(const sf::VertexBuffer &vertexBuffer,
                  const sf::RenderStates &states=sf::RenderStates::Default) final;

        void draw(const sf::VertexBuffer &vertexBuffer,
                  std::size_t firstVertex,
                  std::size_t vertexCount,
                  const sf::RenderStates &states=sf::RenderStates::Default) final;*/

       // void internalOnCanvasParentChange(QSFML_Canvas *newParent) override;
       // void internalOnParentChange(CanvasObject *newParent) override;
    private:
        //QSFML_Canvas *m_canvasParent;

};
