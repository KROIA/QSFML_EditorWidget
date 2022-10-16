#pragma once

#include <QObject>
#include "AABB.h"
#include "MousePressEvent.h"

namespace QSFML
{
namespace Components
{
class Button : public MousePressEvent, public Utilities::AABB
{
    public:
        explicit Button(const std::string &name = "BoxCollider");
        explicit Button(const std::string &name, const Utilities::AABB &box);
        ~Button();

    protected:
        bool getCurrentValue() override;
};
}
}
