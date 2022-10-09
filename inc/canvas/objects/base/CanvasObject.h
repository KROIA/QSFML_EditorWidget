#pragma once

#include <SFML/Graphics.hpp>
#include <QDebug>
#include <vector>
#include "CanvasForwardDeclaration.h"
#include <easy/profiler.h>

class CanvasObject
{
    public:
        CanvasObject(CanvasObject *parent = nullptr);
        virtual ~CanvasObject();

        void setCanvasParent(QSFML_Canvas *parent);
        QSFML_Canvas *getCanvasParent() const;
        void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i &pixelSpace);
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace);

        void addChild(CanvasObject *child);
        void removeChild(CanvasObject *child);
        void clearChilds();
        bool childExists(CanvasObject *child);
        size_t getChildIndex(CanvasObject *child);

        virtual void onCanvasParentChange(QSFML_Canvas *newParent);
        virtual void onParentChange(CanvasObject *newParent);
       // virtual void sfEvent(const sf::Event &e);

        const static size_t npos = -1;
    protected:
        virtual void internalOnCanvasParentChange(QSFML_Canvas *newParent) = 0;
        virtual void internalOnParentChange(CanvasObject *newParent) = 0;

        QSFML_Canvas *m_canvasParent;
        CanvasObject *m_parent;

        std::vector<CanvasObject*> m_childs;
};

