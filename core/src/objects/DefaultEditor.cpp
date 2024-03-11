#include "objects/DefaultEditor.h"

namespace QSFML
{
    namespace Objects
    {
        OBJECT_IMPL(DefaultEditor)
        DefaultEditor::DefaultEditor(const std::string &name,
                                     const sf::Vector2f &size)
            : CanvasObject(name)
            , m_cam(new VisibleCamera("Camera"))
            , m_grid(new BackgroundGrid("Grid"))
        {
            setup(size);
        }
        DefaultEditor::DefaultEditor(const DefaultEditor &other)
            : CanvasObject(other)
            , m_cam(new VisibleCamera("Camera"))
            , m_grid(new BackgroundGrid("Grid"))
        {
            setup(sf::Vector2f(other.m_grid->getSize().width, other.m_grid->getSize().height));
        }
        DefaultEditor::~DefaultEditor()
        {

        }
        void DefaultEditor::setup(const sf::Vector2f& size)
        {
            m_grid->setSize(sf::IntRect(0, 0, size.x, size.y));
            m_grid->setLineColor({ sf::Color(130,130,130),
                                sf::Color(100,100,100),
                                sf::Color(100,100,100),
                                sf::Color(100,100,100),
                                sf::Color(100,100,100) });
            m_cam->setMaxMovingBounds(sf::FloatRect(m_grid->getSize()));

            m_runtimeInfo = new RuntimeInfo();
            m_runtimeInfo->setEnabled(false);

            addChild(m_grid);
            addChild(m_cam);
            addChild(m_runtimeInfo);

        }
        VisibleCamera *DefaultEditor::getCamera() const
        {
            return m_cam;
        }
        BackgroundGrid *DefaultEditor::getGrid() const
        {
            return m_grid;
        }
        RuntimeInfo* DefaultEditor::getRuntimeInfo() const
        {
            return m_runtimeInfo;
        }

        void DefaultEditor::onToggleRuntimeInfo()
        {
            m_runtimeInfo->setEnabled(!m_runtimeInfo->isEnabled());
        }
        void DefaultEditor::onEnableRuntimeInfo()
        {
            m_runtimeInfo->setEnabled(true);
        }
        void DefaultEditor::onDisableRuntimeInfo()
        {
            m_runtimeInfo->setEnabled(false);
        }
    }
}
