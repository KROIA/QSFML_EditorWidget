#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/MousePressEvent.h"
#include "utilities/AABB.h"

#include <QObject>

namespace QSFML
{
namespace Components
{
class QSFML_EDITOR_WIDGET_API Button : public MousePressEvent, public Utilities::AABB
{
    Q_OBJECT
    public:
        Button(const std::string &name = "Button");
        Button(const std::string &name, const Utilities::AABB &box);
        Button(const Button &other);
        ~Button();

        COMPONENT_DECL(Button);

    protected:
        bool getCurrentValue() override;
        bool m_wasInside;
        bool m_wasPressed;
};
}
}
