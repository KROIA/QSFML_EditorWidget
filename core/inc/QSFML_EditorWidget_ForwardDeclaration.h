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
        class CameraWindow;
        class BackgroundGrid;
        class CameraController;
        class DefaultEditor;
        class LineChart;
        class VisibleCamera;

        typedef GameObject* GameObjectPtr;

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
        class Transform;

        typedef Component* ComponentPtr;
    }
    namespace Utilities
    {
        class Updatable;
        class AABB;
        class RandomEngine;
        class Ray;
        class Singelton;
        class Updatable;
        class ObjectQuadTree;
        class Timer;
        struct Collisioninfo;

        
        class Transformable;

        
        class Stats;
        class StatsManager;
    }

    namespace Internal
    {
        class SceneLogger;
        class LifetimeChecker;
    }
    
    enum class TimeDomain
    {
        /**
         * @brief 
         * Real mesured time, multiplied with the "physicsDeltaTScale" of the SceneSettings struct
         */
        realSimulationTime,

        /**
         * @brief 
		 * Fixed time, using the "physicsFixedDeltaT" of the SceneSettings struct
         */
        fixedSimulationTime
    };

#define CLONE_FUNC_DECL(baseClass, className) \
    baseClass* clone() const override;

#define CLONE_FUNC_IMPL(baseClass,className) \
    baseClass* className::clone() const \
    { \
        return new className(*this); \
    }

#define CLONE_FUNC_HEADER_IMPL(baseClass,className) \
    baseClass* clone() const \
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
