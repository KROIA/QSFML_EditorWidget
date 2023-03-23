#pragma once

#include "objects/CanvasObject.h"
#include "objects/VisibleCamera.h"
#include "objects/BackgroundGrid.h"

namespace QSFML
{
namespace Objects
{
class DefaultEditor: public CanvasObject
{
    public:
    DefaultEditor(const std::string &name = "DefaultEditor",
                  const sf::Vector2f &size = sf::Vector2f(800,600));
    DefaultEditor(const DefaultEditor &other);
    ~DefaultEditor();
    CLONE_FUNC(DefaultEditor)

    //void update() override;

    VisibleCamera *getCamera() const;
    BackgroundGrid *getGrid() const;

    protected:

    private:
    VisibleCamera *m_cam;
    BackgroundGrid *m_grid;

};
}
}
