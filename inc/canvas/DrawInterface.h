#pragma once

#include <SFML/Graphics.hpp>

class DrawInterface
{
    public:

        virtual void draw(const sf::Drawable &drawable,
                          const sf::RenderStates &states=sf::RenderStates::Default) = 0;

        virtual void draw(const sf::Vertex *vertices,
                          std::size_t vertexCount,
                          sf::PrimitiveType type,
                          const sf::RenderStates &states=sf::RenderStates::Default) = 0;

        virtual void draw(const sf::VertexBuffer &vertexBuffer,
                          const sf::RenderStates &states=sf::RenderStates::Default) = 0;

        virtual void draw(const sf::VertexBuffer &vertexBuffer,
                          std::size_t firstVertex,
                          std::size_t vertexCount,
                          const sf::RenderStates &states=sf::RenderStates::Default) = 0;



};
