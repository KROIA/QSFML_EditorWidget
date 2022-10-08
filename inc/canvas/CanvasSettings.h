#pragma once

#include <SFML/Window.hpp>


struct CanvasSettings
{
    /**
     * \brief The layout of the Canvas, how it will behave on the screen
     */
    struct Layout
    {
        bool autoAjustSize;         /*!< If set to true, the canvas will automatically
                                         resizes based on its parent widget. */
        sf::Vector2u fixedSize;     //!< If autoAjustSize is false, this size will be applyed to the canvas widget

        /**
         * \brief The margin of the canvas
         * \details This will define how many pixels in each direction the
         *          canvas will be spaced inside of the parent widget.
         */
        struct Margin
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
            fixedSize = sf::Vector2u(100,100);

        }
    };
    Layout layout;

    /**
     * \brief The time related settings
     */
    struct Timing
    {
        unsigned int frameTime;  //!< Time between each frame

        Timing()
        {
            frameTime = 16; // 62.5 FPS
        }
    };
    Timing timing;
    sf::ContextSettings contextSettings;

};
