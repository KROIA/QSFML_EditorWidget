#include "objects/DefaultEditor.h"

namespace QSFML
{
namespace Objects
{
DefaultEditor::DefaultEditor(const std::string &name,
                             const sf::Vector2f &size)
    : CanvasObject(name)
{
    m_cam     = new VisibleCamera("Camera");
    m_grid    = new BackgroundGrid("Grid");

    m_grid->setSize(sf::IntRect(0,0,size.x,size.y));
    m_grid->setLineColor({sf::Color(130,130,130),
                        sf::Color(100,100,100),
                        sf::Color(100,100,100),
                        sf::Color(100,100,100),
                        sf::Color(100,100,100)});
    m_cam->setMaxMovingBounds(sf::FloatRect(m_grid->getSize()));

    addChild(m_grid);
    addChild(m_cam);
}
DefaultEditor::DefaultEditor(const DefaultEditor &other)
    : CanvasObject(other)
{

}
DefaultEditor::~DefaultEditor()
{

}

VisibleCamera *DefaultEditor::getCamera() const
{
    return m_cam;
}
BackgroundGrid *DefaultEditor::getGrid() const
{
    return m_grid;
}
}
}
