#pragma once

#include <string>
#include "CanvasForwardDeclaration.h"

class Component
{
    public:
        Component(const std::string &name = "");
        virtual ~Component();

        virtual void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;

        void setName(const std::string &name);
        const std::string getName() const;

    protected:

    private:
        std::string m_name;
        CanvasObject *m_parent;
};
