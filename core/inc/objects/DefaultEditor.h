#pragma once

#include "objects/base/CanvasObject.h"
#include "objects/VisibleCamera.h"
#include "objects/BackgroundGrid.h"
#include "objects/RuntimeInfo.h"
#include "QSFML_EditorWidget_base.h"

#include <QObject>

namespace QSFML
{
namespace Objects
{
class QSFML_EDITOR_WIDGET_EXPORT DefaultEditor: public QObject, public CanvasObject
{
    Q_OBJECT
    public:
        DefaultEditor(const std::string &name = "DefaultEditor",
                      const sf::Vector2f &size = sf::Vector2f(800,600));
        DefaultEditor(const DefaultEditor &other);
        ~DefaultEditor();
        OBJECT_DECL(DefaultEditor);

        VisibleCamera *getCamera() const;
        BackgroundGrid *getGrid() const;
        RuntimeInfo* getRuntimeInfo() const;

        void setToggleRuntimeInfoKey(const sf::Keyboard::Key& key);

    public slots:
        void onToggleRuntimeInfo();
        void onEnableRuntimeInfo();
        void onDisableRuntimeInfo();

    protected:

    private:
        
        void setup(const sf::Vector2f& size);
        void onCanvasParentChange(Canvas* oldParent, Canvas* newParent) override;

        VisibleCamera *m_cam;
        BackgroundGrid *m_grid;
        RuntimeInfo* m_runtimeInfo;

        QSFML::Components::KeyPressEvent* m_toggleRuntimeInfoEvent;
};
}
}
