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
            , m_cam(std::make_shared<CameraController>("Camera"))
            , m_grid(std::make_shared<BackgroundGrid>("Grid"))
        {
            setup(size);
        }
        DefaultEditor::DefaultEditor(const DefaultEditor &other)
            : GameObject(other)
            , m_cam(std::make_shared<CameraController>("Camera"))
            , m_grid(std::make_shared<BackgroundGrid>("Grid"))
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

            m_runtimeInfo = std::make_shared<RuntimeInfo>();
            m_runtimeInfo->setEnabled(false);

            m_toggleRuntimeInfoEvent = std::make_shared<Components::KeyPressEvent>("RuntimeInfoToggler", sf::Keyboard::F3);
            connect(m_toggleRuntimeInfoEvent.get(), &Components::KeyPressEvent::fallingEdge, this, &DefaultEditor::onToggleRuntimeInfo);
            addComponent(m_toggleRuntimeInfoEvent);


            addChild(m_grid);
            addChild(m_cam);
            m_runtimeInfo->setRenderLayer(RenderLayer::layer_5);
            //addChild(m_runtimeInfo);


        }
        void DefaultEditor::onSceneParentChange(Scene* oldParent, Scene* newParent)
        {
            if (oldParent)
				oldParent->removeObject(m_runtimeInfo);
            if (newParent)
                newParent->addObject(m_runtimeInfo);
        }
        std::shared_ptr<CameraController> DefaultEditor::getCamera() const
        {
            return m_cam;
        }
        std::shared_ptr<BackgroundGrid> DefaultEditor::getGrid() const
        {
            return m_grid;
        }
        std::shared_ptr<RuntimeInfo> DefaultEditor::getRuntimeInfo() const
        {
            return m_runtimeInfo;
        }

        void DefaultEditor::setToggleRuntimeInfoKey(const sf::Keyboard::Key& key)
        {
            m_toggleRuntimeInfoEvent->setTriggerKey(key);
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

