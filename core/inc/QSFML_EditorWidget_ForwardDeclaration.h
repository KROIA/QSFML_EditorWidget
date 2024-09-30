#pragma once

namespace QSFML
{
    class Scene;
    class GameObjectContainer;
    class GameObjectGroup;
    class GameObjectLayerGroup;
    class SceneThreadWorker;
    struct SceneSettings;
    class SceneThreadWorker;

    
    
    namespace Objects
    {
        class GameObject;
        class BackgroundGrid;
        class CameraController;
        class DefaultEditor;
        class LineChart;
        class VisibleCamera;
    }
    namespace Components
    {
        class Component;
        class Drawable;
        class Collider;
        class AbstractPressEvent;
        class Button;
        class VectorPainter;
        class KeyPressEvent;
        class LinePainter;
        class MouseFollower;
        class MousePressEvent;
        class PointPainter;
        class SfEventHandle;
        class Text;
        class Image;
    }
    namespace Utilities
    {
        class Updatable;
        class AABB;
        class RandomEngine;
        class Ray;
        class Singelton;
        class Updatable;
        class VectorOperations;
        class ObjectQuadTree;
        class Timer;
        struct Collisioninfo;

        class Stats;
        class StatsManager;
        class Transformable;
    }
    

#define CLONE_FUNC_DEC(className) \
    className *clone() const

#define CLONE_FUNC_IMPL(className) \
    className* className::clone() const \
    { \
        return new className(*this); \
    }


#define CLONE_FUNC_TEMPLATE_IMPL(className) \
    template<typename templateType> \
    className<templateType>* className<templateType>::clone() const \
    { \
        return new className<templateType>(*this); \
    }

}
