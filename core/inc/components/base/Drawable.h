#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "utilities/Transformable.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>
#include <array>  
#include <cmath>
#include <numbers>  

namespace QSFML
{
namespace Components
{
/// <summary>
/// Drawable component to draw stuff to the screen
/// </summary>
/// <description>
///     This is one of the predefined Components.<br>
///     The Drawable overrides the draw(...)
///     function from the sf::Drawable<br>
///     The draw(...) function will be called on every frame.
/// 
///     You must inherit from this class and then override the applyTransform(...) function<br>
/// <code>
///     class MyDrawable : public QSFML::Components::Drawable
///     {
///         public:
///         MyDrawable(const std::string &name = "MyDrawable")
///             : Drawable(name)
///         {
///     
///         }
///         MyDrawable(const MyDrawable &other)
///              : Drawable(other)
///         {
///     
///         }
///     
///         void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override
///         {
///              // Draw your stuff here
///              // target.draw(...) 
///     
///              // If you want to draw using the relative position of the parent object, 
///              // pass the states to the target.draw(...) function
///              // target.draw(..., states) 
///         }
///     };
/// </code>
/// </description>
class QSFML_EDITOR_WIDGET_EXPORT Drawable : public Component, /*public Utilities::Transformable,*/ public sf::Drawable, public sf::Transformable
{
    public:
        /// <summary>
        /// Drawable constructor
        /// </summary>
        /// <param name="name">Name of the component</param>
        Drawable(const std::string& name = "Drawable");
        Drawable(const Drawable &other);
        virtual ~Drawable();

        /// <summary>
        /// draw
        /// </summary>
        /// <description>
        /// This function will be automaticlly called from the
        /// Scene once per frame if the Component is enabled
        /// </description>
        /// <param name="target"></param>
        /// <param name="states"></param>
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final override
        {
           // applyTransform(states);
            QSFMLP_COMPONENT_BLOCK("Component draw", QSFML_COLOR_STAGE_1);
            QSFMLP_OBJECT_TEXT("Name", getName());
            if (m_ignoreTransform)
            {
                states.transform = sf::Transform(); 
            }
            else
			{
                states.transform *= getTransform();
			}
            drawComponent(target, states);
        }

        /// <summary>
        /// Gets the global world position of the Drawable
        /// </summary>
        /// <returns>global position</returns>
        sf::Vector2f getGlobalPosition() const;

        float getGlobalRotation() const;

        void ignoreTransform(bool ignore) { m_ignoreTransform = ignore; }
        bool ignoresTransform() const { return m_ignoreTransform; }

        bool isVisible(const sf::Vector2f& position, const sf::RenderTarget& target) const
        {
            const sf::View& view = target.getView();
            // Get the view bounds in world coordinates
            sf::FloatRect viewBounds(
                view.getCenter().x - view.getSize().x * 0.5f,
                view.getCenter().y - view.getSize().y * 0.5f,
                view.getSize().x,
                view.getSize().y
            );

            // Check if the position is within the view bounds
            return viewBounds.contains(position);
        }

    protected:
        /// <summary>
        /// Moves the states transform by the relative position of the Drawable
        /// </summary>
        /// <param name="states"></param>
        //void applyTransform(sf::RenderStates& states) const
        //{
        //    states.transform.translate(getPosition());
        //}

        /// <summary>
        /// User defined draw function
        /// </summary>
        /// <description>
        /// This function will be called on every frame
        /// You must override this function and draw your stuff here
        /// The states transform will already be moved to the correct relative position uf the object
        /// </description>
        /// <param name="target"></param>
        /// <param name="states"></param>
        virtual void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const = 0;



		template <size_t segments>
        static void drawGlCircleShape(const sf::Vector2f& pos, float radius)
        {
            static constexpr auto shape = generateCircleVertices<segments>();

           // glColor4ub(m_nodeColor.r, m_nodeColor.g, m_nodeColor.b, m_nodeColor.a);
            glBegin(GL_TRIANGLE_FAN);
            for (size_t i = 0; i < segments; i++)
            {
                glVertex2f(pos.x + shape.at(i).at(0) * radius, pos.y + shape.at(i).at(1) * radius);
            }
            glEnd();
        }

   // private:


        
        // Generate the shape array at compile-time
        template <size_t segments>
        static constexpr auto generateCircleVertices()
        {
            std::array<std::array<float, 2>, segments> shape{};
            constexpr float PI = M_PI;  // Use constexpr PI from <numbers>

            for (size_t i = 0; i < segments; ++i)
            {
                float angle = 2.0f * PI * i / static_cast<float>(segments) - PI;
                shape.at(i).at(0) = cosine(angle);
                shape.at(i).at(1) = sine(angle);
            }
            return shape;
        }


        // A constexpr approximation for sine using more terms of a Taylor series
        static constexpr float sine(float x)
        {
            float result = x;  // First term
            float term = x;    // Current term
            const float x_squared = x * x;

            term *= -x_squared / (2.0f * 3.0f);  // Second term
            result += term;

            term *= -x_squared / (4.0f * 5.0f);  // Third term
            result += term;

            term *= -x_squared / (6.0f * 7.0f);  // Fourth term
            result += term;

            term *= -x_squared / (8.0f * 9.0f);  // Fifth term
            result += term;

            term *= -x_squared / (10.0f * 11.0f);  // Sixth term
            result += term;

            term *= -x_squared / (12.0f * 13.0f);  // Seventh term
            result += term;

            return result;
        }

        // A constexpr approximation for cosine using more terms of a Taylor series
        static constexpr float cosine(float x)
        {
            float result = 1.0f;  // First term
            float term = 1.0f;    // Current term
            const float x_squared = x * x;

            term *= -x_squared / (1.0f * 2.0f);  // Second term
            result += term;

            term *= -x_squared / (3.0f * 4.0f);  // Third term
            result += term;

            term *= -x_squared / (5.0f * 6.0f);  // Fourth term
            result += term;

            term *= -x_squared / (7.0f * 8.0f);  // Fifth term
            result += term;

            term *= -x_squared / (9.0f * 10.0f);  // Sixth term
            result += term;

            term *= -x_squared / (11.0f * 12.0f);  // Seventh term
            result += term;

            return result;
        }

        bool m_ignoreTransform = false;
};
}
}
