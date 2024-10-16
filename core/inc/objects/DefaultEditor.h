#pragma once

#include "objects/base/GameObject.h"
#include "objects/CameraController.h"
#include "objects/BackgroundGrid.h"
#include "objects/RuntimeInfo.h"
#include "QSFML_EditorWidget_base.h"

#include <QObject>

namespace QSFML
{
namespace Objects
{
class QSFML_EDITOR_WIDGET_EXPORT DefaultEditor: public QObject, public GameObject
{
    Q_OBJECT
    public:
        DefaultEditor(const std::string &name = "DefaultEditor",
                      const sf::Vector2f &size = sf::Vector2f(800,600));
        DefaultEditor(const std::string& name,
                      const sf::FloatRect& area);
        DefaultEditor(const DefaultEditor &other);
        ~DefaultEditor();
        OBJECT_DECL(DefaultEditor);

        CameraController* getCamera() const;
        BackgroundGrid* getGrid() const;
        RuntimeInfo* getRuntimeInfo() const;

        void setToggleRuntimeInfoKey(const sf::Keyboard::Key& key);

        void setColors(const sf::Color& darkColor, const sf::Color& lightColor);
		void setLineThickness(float thickness);

    public slots:
        void onToggleRuntimeInfo();
        void onEnableRuntimeInfo();
        void onDisableRuntimeInfo();

    protected:
        void onAwake() override;

    private:
        
        void setup(const sf::IntRect& area);
        void onSceneParentChange(Scene* oldParent, Scene* newParent) override;

        CameraController* m_cam;
        BackgroundGrid* m_grid;
        RuntimeInfo* m_runtimeInfo;

		sf::Color m_darkColor;
		sf::Color m_lightColor;

        QSFML::Components::KeyPressEvent* m_toggleRuntimeInfoEvent;
};
}
}
