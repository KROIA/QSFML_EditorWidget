#pragma once

#include <vector>
#include "CanvasObject.h"
#include "Drawable.h"
#include "CameraController.h"

namespace QSFML
{
class CanvasObjectContainer
{
    public:
        CanvasObjectContainer(Canvas *parent);
        ~CanvasObjectContainer();

        void addObject(Objects::CanvasObject *obj);
        void addObject(const std::vector<Objects::CanvasObject*> &objs);

        void removeObject(Objects::CanvasObject *obj);
        void removeObject(const std::vector<Objects::CanvasObject*> &objs);
        void clearObjects();

        void reserveObjectsCount(size_t size);
        size_t getObjectsCount() const;
        template<typename T>
        size_t getObjectsCount() const;
        const std::vector<Objects::CanvasObject*> &getObjects() const;
        template<typename T>
        std::vector<T*> getObjects() const;
        template<typename T>
        T* getFirstObject() const;

        bool objectExists(Objects::CanvasObject *obj);
        size_t getObjectIndex(Objects::CanvasObject *obj);


        void sfEvent(const std::vector<sf::Event> &events);
        void draw(sf::RenderWindow &window);

        const static size_t npos = -1;
    private:
        template<typename T>
        bool objectExists(T *obj,const std::vector<T*> &list);

        template<typename T>
        size_t getObjectIndex(T *obj,const std::vector<T*> &list);

        template<typename T>
        void addObject(Objects::CanvasObject *obj,std::vector<T*> &list);

        template<typename T>
        void removeObject(Objects::CanvasObject *obj,std::vector<T*> &list);

        // All objects will be contained in this list
        std::vector<Objects::CanvasObject*> m_container;

        Canvas *m_parent;
};

}
