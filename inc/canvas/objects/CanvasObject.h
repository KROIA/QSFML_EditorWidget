#pragma once

#include <SFML/Graphics.hpp>
#include <QDebug>
#include <vector>
#include "CanvasForwardDeclaration.h"
#include "Component.h"
#include <easy/profiler.h>



class CanvasObject
{
    public:
        CanvasObject(const std::string &name = "",
                     CanvasObject *parent = nullptr);
        virtual ~CanvasObject();

        // Canvas Object Internal functions
        void setCanvasParent(QSFML_Canvas *parent);
        QSFML_Canvas *getCanvasParent() const;







        // Canvas Object external functions
        void setParent(CanvasObject *parent);
        CanvasObject *getParent() const;

        void setName(const std::string &name);
        const std::string getName() const;

        sf::Vector2i getMousePosition() const;
        sf::Vector2f getMouseWorldPosition() const;
        sf::Vector2f getInWorldSpace(const sf::Vector2i &pixelSpace) const;
        sf::Vector2i getInScreenSpace(const sf::Vector2f &worldSpace) const;

        void addChild(CanvasObject *child);
        void removeChild(CanvasObject *child);
        void clearChilds();
        bool childExists(CanvasObject *child) const;
        size_t getChildIndex(CanvasObject *child) const;
        const std::vector<CanvasObject*> &getChilds() const;
        size_t getChildCount() const;

        void addComponent(Component *comp);
        void removeComponent(Component *comp);
        void clearComponents();
        bool componentExists(Component *comp) const;
        size_t getComponentIndex(Component *comp) const;
        const std::vector<Component*> &getComponents()  const;
        template<typename T>
        std::vector<T*> getComponents() const;
        size_t getComponentCount() const;
        template<typename T>
        size_t getComponentCount() const;



        void sfEvent(const std::vector<sf::Event> &events);
        void draw(sf::RenderWindow &window) const;

        std::string toString() const;
        const static size_t npos = -1;
    protected:
        std::vector<std::string> toStringInternal(const std::string &preStr) const;


        virtual void onCanvasParentChange(QSFML_Canvas *newParent);
        virtual void onParentChange(CanvasObject *newParent);

        virtual void internalOnCanvasParentChange(QSFML_Canvas *newParent) {}
        virtual void internalOnParentChange(CanvasObject *newParent) {}

        static size_t m_objNameCounter;
        std::string m_name;
        QSFML_Canvas *m_canvasParent;
        CanvasObject *m_parent;


        std::vector<CanvasObject*> m_childs;
        std::vector<Component*> m_components;


};

