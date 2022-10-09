#include "Drawable.h"
#include "QSFML_Canvas.h"

Drawable::Drawable(CanvasObject *parent)
    :   CanvasObject(parent)
{
    m_canvasParent = nullptr;
}
Drawable::~Drawable()
{

}

void Drawable::draw(const sf::Drawable &drawable,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(drawable,states);
}


void Drawable::draw(const sf::Vertex *vertices,
                        std::size_t vertexCount,
                        sf::PrimitiveType type,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertices,vertexCount,type,states);
}


void Drawable::draw(const sf::VertexBuffer &vertexBuffer,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertexBuffer,states);
}


void Drawable::draw(const sf::VertexBuffer &vertexBuffer,
                        std::size_t firstVertex,
                        std::size_t vertexCount,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertexBuffer,firstVertex,vertexCount,states);
}
void Drawable::internalOnCanvasParentChange(QSFML_Canvas *newParent)
{

}
void Drawable::internalOnParentChange(CanvasObject *newParent)
{

}
