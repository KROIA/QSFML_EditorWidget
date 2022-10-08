#pragma once

#include <vector>
#include "CanvasObject.h"
#include "Drawable.h"
#include "CameraController.h"

class CanvasObjectContainer
{
    public:
        CanvasObjectContainer(QSFML_Canvas *parent);
        ~CanvasObjectContainer();

        void addObject(CanvasObject *obj);
        void addObject(const std::vector<CanvasObject*> &objs);

        void removeObject(CanvasObject *obj);
        void removeObject(const std::vector<CanvasObject*> &objs);
        void clearObjects();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        const std::vector<CanvasObject*> &getObjects() const;

        bool objectExists(CanvasObject *obj);
        size_t getObjectIndex(CanvasObject *obj);

        void sfEvent(const sf::Event &e);
        void draw();

        const static size_t npos = -1;
    private:
        template<typename T>
        bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        void addObject(CanvasObject *obj,std::vector<T*> &list);

        template<typename T>
        void removeObject(CanvasObject *obj,std::vector<T*> &list);

        std::vector<CanvasObject*> m_container;

        std::vector<Drawable*> m_drawables;
        std::vector<CameraController*> m_cameras;

        QSFML_Canvas *m_parent;
};


