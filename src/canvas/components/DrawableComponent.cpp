#include "DrawableComponent.h"
#include "QSFML_Canvas.h"

DrawableComponent::DrawableComponent(const std::string &name)
    :   Component(name)
{
   // m_canvasParent = nullptr;
}
DrawableComponent::~DrawableComponent()
{

}
/*
void DrawableComponent::setParent(CanvasObject *parent)
{
    Component::setParent(parent);
    m_canvasParent = Component::getParent()->getCanvasParent();
}*/
/*
void DrawableComponent::draw(const sf::Drawable &drawable,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(drawable,states);
}


void DrawableComponent::draw(const sf::Vertex *vertices,
                        std::size_t vertexCount,
                        sf::PrimitiveType type,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertices,vertexCount,type,states);
}


void DrawableComponent::draw(const sf::VertexBuffer &vertexBuffer,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertexBuffer,states);
}


void DrawableComponent::draw(const sf::VertexBuffer &vertexBuffer,
                        std::size_t firstVertex,
                        std::size_t vertexCount,
                        const sf::RenderStates &states)
{
    if(!m_canvasParent) return;
    m_canvasParent->draw(vertexBuffer,firstVertex,vertexCount,states);
}*/
/*void DrawableComponent::internalOnCanvasParentChange(QSFML_Canvas *newParent)
{

}
void DrawableComponent::internalOnParentChange(CanvasObject *newParent)
{

}*/
