#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>

namespace QSFML
{
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
     * \brief The Colors of different parts on the canvas
     */
    struct Colors
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

    /**
     * \brief Text settings
     */
    std::string fontPath;

    struct UpdateControlls
    {
        bool enableEventLoop;
        bool enableUpdateLoop;
        bool enablePaintLoop;

        UpdateControlls()
        {
            enableEventLoop = true;
            enableUpdateLoop = true;
            enablePaintLoop = true;
        }
    };
    UpdateControlls updateControlls;

    CanvasSettings()
    {
        fontPath = "C:\\Windows\\Fonts\\Arial.ttf";
    }
};
}
