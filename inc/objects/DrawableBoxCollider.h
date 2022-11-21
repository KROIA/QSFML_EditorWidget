/*#pragma once

#include "CanvasObject.h"
#include "components/Drawable.h"
#include "components/BoxCollider.h"
#include "components/DrawableVector.h"


namespace QSFML
{
namespace Objects
{
class DrawableBoxCollider: public CanvasObject
{
    public:
    DrawableBoxCollider(const std::string &name = "DrawableBoxCollider");
    DrawableBoxCollider(const std::string &name, const Utilities::AABB &box);
    DrawableBoxCollider(const DrawableBoxCollider &other);
    ~DrawableBoxCollider();
    CLONE_FUNC(DrawableBoxCollider)

    void setColor(const sf::Color &color);
    const sf::Color &getColor() const;

    Components::BoxCollider *getBox() const;
    void update() override;

    protected:

    private:
    void setup(const Utilities::AABB &box);
    sf::Color m_color;
    Components::DrawableVector *m_pos, *m_top, *m_left, *m_right, *m_bottom;
    Components::BoxCollider *m_box;
};
}
}
*/
