#pragma once

namespace QSFML
{
    class Canvas;
    class CanvasObjectContainer;
    class CanvasObjectGroup;
    class CanvasThreadWorker;
    struct CanvasSettings;
    class CanvasThreadWorker;

    class Stats;
    class StatsManager;
    
    namespace Objects
    {
        class CanvasObject;
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
        class DrawableVector;
        class KeyPressEvent;
        class Line;
        class MouseFollower;
        class MousePressEvent;
        class PointPainter;
        class SfEventHandle;
        class Text;
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
        struct CollisionInfo;
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
