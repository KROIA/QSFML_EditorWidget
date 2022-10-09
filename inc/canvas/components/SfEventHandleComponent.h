#pragma once

#include <SFML/Graphics.hpp>
#include "Component.h"

class SfEventHandleComponent :   public Component
{
    public:
        SfEventHandleComponent(const std::string &name = "SfEventHandleComponent")
            : Component(name){};
        virtual ~SfEventHandleComponent(){};

        virtual void sfEvent(const sf::Event &e) = 0;
    protected:

};
