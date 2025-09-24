#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "utilities/Transformable.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>
#include <array>  
#include <cmath>

namespace QSFML
{
namespace Components
{
/// @class Drawable
/// @brief
/// Drawable component to draw stuff to the screen 
/// 
/// @details
///     This is one of the predefined Components.<br>
///     The Drawable overrides the draw(...)
///     function from the sf::Drawable<br>
///     The draw(...) function will be called on every frame.
/// 
///     You must inherit from this class and then override the drawComponent(...) function<br>
/// 
/// @code
/// class MyDrawable : public QSFML::Components::Drawable
/// {
///     public:
///     MyDrawable(const std::string &name = "MyDrawable")
///         : Drawable(name)
///     {
/// 
///     }
///     MyDrawable(const MyDrawable &other)
///          : Drawable(other)
///     {
/// 
///     }
/// 
///     void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override
///     {
///          // Draw your stuff here
///          // target.draw(...) 
/// 
///          // If you want to draw using the relative position of the parent object, 
///          // pass the states to the target.draw(...) function
///          // target.draw(..., states) 
///     }
/// };
/// @endcode
class QSFML_EDITOR_WIDGET_API Drawable : public Component, public sf::Drawable, public sf::Transformable
{
    public:
        Drawable(const std::string& name = "Drawable");
        Drawable(const Drawable &other);
        virtual ~Drawable();

		/// @brief
		/// Draw the object to the target
		/// @details
		/// This function will be called on every frame
		/// You must override the drawComponent(...) function and draw your stuff there
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

		/// @brief Gets the global position of the object
		/// @return the global position of the object
        sf::Vector2f getGlobalPosition() const;

		/// @brief Gets the global rotation of the object
		/// @return the global rotation of the object
        float getGlobalRotation() const;

        sf::Transform getGlobalTransform() const;

		/// @brief Enables or disables the transform of the drawable
        /// @param ignore 
        void ignoreTransform(bool ignore) { m_ignoreTransform = ignore; }

		/// @brief Returns if the transform is ignored
		/// @return true if the transform is ignored for this drawable
        bool ignoresTransform() const { return m_ignoreTransform; }

        /**
         * @brief
         * Gets the default camera of the scene
         * @return default camera or nullptr if object is not in a scene
         */
        Objects::CameraWindow* getDefaultCamera() const;

        /**
         * @brief
         * Gets the camera that is currently used for rendering
         * @return current render camera, or nullptr if object is not in rendering step
         */
        Objects::CameraWindow* getCurrentRenderCamera() const;


		/// @brief
		/// Check if the object is visible in the target
		/// @details
		/// This function will check if the object is visible in the target
		/// @param position
		/// A position that shuld be checked
		/// @param target
		/// The target where the object should be visible
		/// @return true if the object is visible in the target
        /// 
		/// @code
        /// void MyDrawable::drawComponent(sf::RenderTarget& target, sf::RenderStates states)
        /// {
		///     sf::Vector2f pos = sf::Vector2f(100, 100);
		///     bool visible = isVisible(pos, target);
		///     if(visible)
		///     {
		///         // Draw the point
        ///     }
        ///     else
        ///     {
		///         // Do not draw the point, it would be outside the view
        ///     }
        /// }
		/// @endcode
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

        /**
         * @brief 
		 * Check if the Rectangle is visible in the target
         * @param rect 
         * @param target 
		 * @return true if the rectangle is visible in the target
         */
        bool isVisible(const sf::FloatRect& rect, const sf::RenderTarget& target) const
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
            return viewBounds.intersects(rect);
        }


        /// @brief 
        /// Draw a circle shape using OpenGL
        /// @tparam segments 
        /// The number of segments of the circle
        /// @details
        /// The template function allows you to specify the number of segments and thus the quality of the circle
        /// On the first call, the function will create a cache of the shape
        /// This makes it a lot faster to draw the circle 
        /// @param pos 
        /// Position of the center of the circle in world coordinates
        /// @param radius 
        /// Radius of the circle in world coordinates
        /// 
        /// @code
        /// void MyDrawable::drawComponent(sf::RenderTarget& target, sf::RenderStates states)
        /// {
        ///     sf::Color color = sf::Color::Red;
        ///     sf::Vector2f pos = sf::Vector2f(100, 100);
        ///     float radius = 50;
        ///     glColor4ub(color.r, color.g, color.b, color.a);
        ///     drawGlCircleShape<30>(pos, radius);
        /// }
        /// @endcode
        template <size_t segments>
        static void drawGlCircleShape(const sf::Vector2f& pos, float radius)
        {
            static bool initialized = false;
            static sf::Vector2f shape[segments];
            if (!initialized)
            {
                // Create shape cache
                float dAngle = 2 * M_PI / segments;
                for (size_t i = 0; i < segments; i++)
                {
                    shape[i].x = std::cos(i * dAngle);
                    shape[i].y = std::sin(i * dAngle);
                }
                initialized = true;
            }
            glBegin(GL_TRIANGLE_FAN);
            for (size_t i = 0; i < segments; i++)
            {
                glVertex2f(pos.x + shape[i].x * radius, pos.y + shape[i].y * radius);
            }
            glEnd();
        }

    protected:
       
        /// @brief 
        /// User defined draw function
		/// @details
		/// This function will be called on every frame
		/// You must override this function and draw your stuff here
		/// The states transform will already be moved to the correct relative position uf the object
        /// @param target 
        /// @param states 
        virtual void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const = 0;


		

        private:
			bool m_ignoreTransform = false; ///< Ignore the transform of the drawable if it is set to true
};
}
}
