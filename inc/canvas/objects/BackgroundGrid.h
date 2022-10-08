#pragma once

#include "Drawable.h"

class BackgroundGrid: public Drawable, virtual CanvasObject
{
    public:
        BackgroundGrid(CanvasObject *parent = nullptr):Drawable(parent){};
        virtual ~BackgroundGrid(){};

        void draw(const sf::Vector2f &drawPos) final
        {
            sf::Vector2f start = drawPos;
            sf::Vector2f end(500,0);
            for(int i=0; i<10; ++i)
            {
                end.y = start.y;
                sf::Vertex line[] =
                {
                    sf::Vertex(start),
                    sf::Vertex(end)
                };

                Drawable::draw(line, 2, sf::Lines);
                start.y += 20;
            }
           // qDebug() << "Draw";
        }
};
