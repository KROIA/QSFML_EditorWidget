#include "objects/DefaultEditor.h"
#include "Components/KeyPressEvent.h"
#include "Scene/Scene.h"

namespace QSFML
{
    namespace Objects
    {
        OBJECT_IMPL(DefaultEditor)
        DefaultEditor::DefaultEditor(const std::string &name,
                                     const sf::Vector2f &size)
            : GameObject(name)
            , m_cam(new CameraController("Camera"))
            , m_grid(new BackgroundGrid("Grid"))
        {
			m_lightColor = sf::Color(130, 130, 130);
			m_darkColor = sf::Color(100, 100, 100);
            setup(size);
        }
        DefaultEditor::DefaultEditor(const DefaultEditor &other)
            : GameObject(other)
            , m_cam(new CameraController("Camera"))
            , m_grid(new BackgroundGrid("Grid"))
        {
			m_darkColor = other.m_darkColor;
			m_lightColor = other.m_lightColor;
            setup(sf::Vector2f(other.m_grid->getSize().width, other.m_grid->getSize().height));
        }
        DefaultEditor::~DefaultEditor()
        {

        }
        void DefaultEditor::setup(const sf::Vector2f& size)
        {
            m_grid->setSize(sf::IntRect(0, 0, size.x, size.y));
            m_grid->setLineColor({ m_lightColor,
                                m_darkColor,
                                m_darkColor,
                                m_darkColor,
                                m_darkColor });
            m_cam->setMaxMovingBounds(sf::FloatRect(m_grid->getSize()));

            m_runtimeInfo = new RuntimeInfo();
            m_runtimeInfo->setEnabled(false);

            m_toggleRuntimeInfoEvent = new Components::KeyPressEvent("RuntimeInfoToggler", sf::Keyboard::F3);
            connect(m_toggleRuntimeInfoEvent, &Components::KeyPressEvent::fallingEdge, this, &DefaultEditor::onToggleRuntimeInfo);
            addComponent(m_toggleRuntimeInfoEvent);


            addChild(m_grid);
            addChild(m_cam);
            m_runtimeInfo->setRenderLayer(RenderLayer::layer_5);
            //addChild(m_runtimeInfo);


        }
        void DefaultEditor::onSceneParentChange(Scene* oldParent, Scene* newParent)
        {            
            //QSFML_UNUSED(oldParent);
            //QSFML_UNUSED(newParent);
            if (oldParent)
				oldParent->removeObject(m_runtimeInfo);
            if (newParent)
                newParent->addObject(m_runtimeInfo);
        }
        CameraController* DefaultEditor::getCamera() const
        {
            return m_cam;
        }
        BackgroundGrid* DefaultEditor::getGrid() const
        {
            return m_grid;
        }
        RuntimeInfo* DefaultEditor::getRuntimeInfo() const
        {
            return m_runtimeInfo;
        }

        void DefaultEditor::setToggleRuntimeInfoKey(const sf::Keyboard::Key& key)
        {
            m_toggleRuntimeInfoEvent->setTriggerKey(key);
        }

        void DefaultEditor::setColors(const sf::Color& darkColor, const sf::Color& lightColor)
        {
			m_darkColor = darkColor;
			m_lightColor = lightColor;
            setup({ (float)m_grid->getSize().width, (float)m_grid->getSize().height });
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

