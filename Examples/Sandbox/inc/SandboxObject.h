#pragma once

#include "QSFML_EditorWidget.h"

class SandboxObject: public QObject, public QSFML::Objects::GameObject
{
        Q_OBJECT
    public:
    SandboxObject(const std::string &name = "SandboxObject");
    SandboxObject(const SandboxObject &other);
    ~SandboxObject();
    OBJECT_DECL(SandboxObject)

    void update() override;

    private slots:
    void onButtonPress();

    protected:

    private:
        QSFML::Components::VectorPainter *m_vec;

};
