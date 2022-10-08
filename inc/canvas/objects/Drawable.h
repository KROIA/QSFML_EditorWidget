#pragma once

#include "CanvasObject.h"
#include "DrawInterface.h"

class Drawable : virtual public CanvasObject, protected DrawInterface
{
    public:
        Drawable(CanvasObject *parent = nullptr);
        ~Drawable();

        virtual void draw(const sf::Vector2f &drawPos = sf::Vector2f(0,0)) = 0;

    protected:


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
                  const sf::RenderStates &states=sf::RenderStates::Default) final;

        void internalOnCanvasParentChange(QSFML_Canvas *newParent) override;
        void internalOnParentChange(CanvasObject *newParent) override;

        DrawInterface *m_drawInterface;
    private:


};
