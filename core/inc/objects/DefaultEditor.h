#pragma once

#include "objects/base/CanvasObject.h"
#include "objects/VisibleCamera.h"
#include "objects/BackgroundGrid.h"
#include "QSFML_base.h"

namespace QSFML
{
namespace Objects
{
class QSFML_EDITOR_WIDGET_EXPORT DefaultEditor: public CanvasObject
{
    public:
    DefaultEditor(const std::string &name = "DefaultEditor",
                  const sf::Vector2f &size = sf::Vector2f(800,600));
    DefaultEditor(const DefaultEditor &other);
    ~DefaultEditor();
    OBJECT_DECL(DefaultEditor);

    VisibleCamera *getCamera() const;
    BackgroundGrid *getGrid() const;

    protected:

    private:
    void setup(const sf::Vector2f& size);
    VisibleCamera *m_cam;
    BackgroundGrid *m_grid;

};
}
}
