#pragma once

#include "QSFML_EditorWidget.h"

class SandboxObject: public QSFML::Objects::CanvasObject
{
    public:
    SandboxObject(const std::string &name = "SandboxObject");
    SandboxObject(const SandboxObject &other);
    ~SandboxObject();
    CLONE_FUNC(SandboxObject)

    void update() override;


    protected:

    private:
        QSFML::Components::DrawableVector *m_vec;

};
