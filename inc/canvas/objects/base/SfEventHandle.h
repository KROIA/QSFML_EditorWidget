#pragma once

#include <SFML/Graphics.hpp>

class SfEventHandle
{
    public:
        SfEventHandle(){};
        virtual ~SfEventHandle(){};

        virtual void sfEvent(const sf::Event &e) = 0;
    protected:

};
