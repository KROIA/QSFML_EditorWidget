#include "Drawable.h"
#include "QSFML_Canvas.h"

Drawable::Drawable(CanvasObject *parent)
    :   CanvasObject(parent)
{
    m_drawInterface = nullptr;
}
Drawable::~Drawable()
{

}

void Drawable::draw(const sf::Drawable &drawable,
                        const sf::RenderStates &states)
{
    if(!m_drawInterface) return;
    m_drawInterface->draw(drawable,states);
}


void Drawable::draw(const sf::Vertex *vertices,
                        std::size_t vertexCount,
                        sf::PrimitiveType type,
                        const sf::RenderStates &states)
{
    if(!m_drawInterface) return;
    m_drawInterface->draw(vertices,vertexCount,type,states);
}


void Drawable::draw(const sf::VertexBuffer &vertexBuffer,
                        const sf::RenderStates &states)
{
    if(!m_drawInterface) return;
    m_drawInterface->draw(vertexBuffer,states);
}


void Drawable::draw(const sf::VertexBuffer &vertexBuffer,
                        std::size_t firstVertex,
                        std::size_t vertexCount,
                        const sf::RenderStates &states)
{
    if(!m_drawInterface) return;
    m_drawInterface->draw(vertexBuffer,firstVertex,vertexCount,states);
}
void Drawable::internalOnCanvasParentChange(QSFML_Canvas *newParent)
{
    if(newParent)
    {
        m_drawInterface = newParent->getDrawInterface();
    }
}
void Drawable::internalOnParentChange(CanvasObject *newParent)
{

}
