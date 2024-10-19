#pragma once

#include "QSFML_EditorWidget_base.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>


namespace QSFML
{
struct QSFML_EDITOR_WIDGET_EXPORT SceneSettings
{
    /**
     * \brief The layout of the Scene, how it will behave on the screen
     */
    struct QSFML_EDITOR_WIDGET_EXPORT Layout
    {
        bool autoAjustSize;         /*!< If set to true, the Scene will automatically
                                         resizes based on its parent widget. */
        sf::Vector2u fixedSize;     //!< If autoAjustSize is false, this size will be applyed to the Scene widget

        /**
         * \brief The margin of the Scene
         * \details This will define how many pixels in each direction the
         *          Scene will be spaced inside of the parent widget.
         */
        struct QSFML_EDITOR_WIDGET_EXPORT Margin
        {
            unsigned int left;
            unsigned int top;
            unsigned int right;
            unsigned int bottom;

            Margin()
            {
                left = 0;
                top = 0;
                right = 0;
                bottom = 0;
            }
        };
        Margin margin;

        Layout()
        {
            autoAjustSize = true;
            fixedSize = sf::Vector2u(1000,700);

        }
    };
    Layout layout;

    /**
     * \brief The Colors of different parts on the Scene
     */
    struct QSFML_EDITOR_WIDGET_EXPORT Colors
    {
        sf::Color defaultBackground; //!< Background color of parts on which nothing was painted

        Colors()
        {
            defaultBackground = sf::Color(80,80,80);
        }
    };
    Colors colors;

    /**
     * \brief The time related settings
     */
    struct QSFML_EDITOR_WIDGET_EXPORT Timing
    {
        double frameTime;  //!< Time between each frame [s]

        //bool physicsUseFixedTimeInterval; //<! If enabled, deltaT will be set to a fixed value, otherwise deltaT = frameTime*timeScale
        double physicsDeltaTScale;  // seconds multiplyer
        double physicsFixedDeltaT;  // [s]
        Timing()
        {
            frameTime = 0.016; // 62.5 FPS
            //physicsUseFixedTimeInterval = true;
            physicsDeltaTScale = 1.f;
            physicsFixedDeltaT = 0.01;
        }
    };
    Timing timing;
    sf::ContextSettings contextSettings;

    /**
     * \brief Text settings
     */
    //std::string fontPath;

    struct QSFML_EDITOR_WIDGET_EXPORT UpdateControlls
    {
        bool enableEventLoop;
        bool enableUpdateLoop;
        bool enablePaintLoop;
        bool enableMultithreading;
        struct ThreadSettings
        {
            size_t threadCount;
            size_t objectGroups;

            ThreadSettings()
            {
                threadCount = 4;
                objectGroups = 8;
            }
        };
        ThreadSettings threadSettings;

        UpdateControlls()
        {
            enableEventLoop = true;
            enableUpdateLoop = true;
            enablePaintLoop = true;
            enableMultithreading = false;
        }
    };
    UpdateControlls updateControlls;

    SceneSettings()
    {
       // fontPath = "C:\\Windows\\Fonts\\Arial.ttf";
    }
};
}
