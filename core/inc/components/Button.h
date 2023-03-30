#pragma once

#include <QObject>
#include "utilities/AABB.h"
#include "components/MousePressEvent.h"
#include "QSFML_global.h"

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_EXPORT Button : public MousePressEvent, public Utilities::AABB
{
    public:
        explicit Button(const std::string &name = "BoxCollider");
        explicit Button(const std::string &name, const Utilities::AABB &box);
        Button(const Button &other);
        ~Button();

        COMPONENT_DECL(Button)

    protected:
        bool getCurrentValue() override;
        bool m_wasInside;
        bool m_wasPressed;
};
}
}
