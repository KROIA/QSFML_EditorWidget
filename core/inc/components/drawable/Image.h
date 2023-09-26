#pragma once

#include "QSFML_base.h"
#include "components/base/Drawable.h"

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_EXPORT Image : public Drawable
        {
        public:
            Image(const std::string& name = "Image");
            Image(const sf::Texture* t, const std::string& name = "Image");
            Image(const std::string &imagePath, const std::string& name = "Image");
            Image(const Image& other);
            ~Image();
            COMPONENT_DECL(Image);

            void setTexture(const sf::Texture* t);
            const sf::Texture* getTexture() const;

            bool loadFromImage(const std::string& imagePath);

            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override;


            ////////////////////////////////////////////////////////////
            /// \brief set the position of the object
            ///
            /// This function completely overwrites the previous position.
            /// See the move function to apply an offset based on the previous position instead.
            /// The default position of a transformable object is (0, 0).
            ///
            /// \param x X coordinate of the new position
            /// \param y Y coordinate of the new position
            ///
            /// \see move, getPosition
            ///
            ////////////////////////////////////////////////////////////
            void setPosition(float x, float y);

            ////////////////////////////////////////////////////////////
            /// \brief set the position of the object
            ///
            /// This function completely overwrites the previous position.
            /// See the move function to apply an offset based on the previous position instead.
            /// The default position of a transformable object is (0, 0).
            ///
            /// \param position New position
            ///
            /// \see move, getPosition
            ///
            ////////////////////////////////////////////////////////////
            void setPosition(const sf::Vector2f& position);

            ////////////////////////////////////////////////////////////
            /// \brief set the orientation of the object
            ///
            /// This function completely overwrites the previous rotation.
            /// See the rotate function to add an angle based on the previous rotation instead.
            /// The default rotation of a transformable object is 0.
            ///
            /// \param angle New rotation, in degrees
            ///
            /// \see rotate, getRotation
            ///
            ////////////////////////////////////////////////////////////
            void setRotation(float angle);

            ////////////////////////////////////////////////////////////
            /// \brief set the scale factors of the object
            ///
            /// This function completely overwrites the previous scale.
            /// See the scale function to add a factor based on the previous scale instead.
            /// The default scale of a transformable object is (1, 1).
            ///
            /// \param factorX New horizontal scale factor
            /// \param factorY New vertical scale factor
            ///
            /// \see scale, getScale
            ///
            ////////////////////////////////////////////////////////////
            void setScale(float factorX, float factorY);

            ////////////////////////////////////////////////////////////
            /// \brief set the scale factors of the object
            ///
            /// This function completely overwrites the previous scale.
            /// See the scale function to add a factor based on the previous scale instead.
            /// The default scale of a transformable object is (1, 1).
            ///
            /// \param factors New scale factors
            ///
            /// \see scale, getScale
            ///
            ////////////////////////////////////////////////////////////
            void setScale(const sf::Vector2f& factors);

            ////////////////////////////////////////////////////////////
            /// \brief set the local origin of the object
            ///
            /// The origin of an object defines the center point for
            /// all transformations (position, scale, rotation).
            /// The coordinates of this point must be relative to the
            /// top-left corner of the object, and ignore all
            /// transformations (position, scale, rotation).
            /// The default origin of a transformable object is (0, 0).
            ///
            /// \param x X coordinate of the new origin
            /// \param y Y coordinate of the new origin
            ///
            /// \see getOrigin
            ///
            ////////////////////////////////////////////////////////////
            void setOrigin(float x, float y);

            ////////////////////////////////////////////////////////////
            /// \brief set the local origin of the object
            ///
            /// The origin of an object defines the center point for
            /// all transformations (position, scale, rotation).
            /// The coordinates of this point must be relative to the
            /// top-left corner of the object, and ignore all
            /// transformations (position, scale, rotation).
            /// The default origin of a transformable object is (0, 0).
            ///
            /// \param origin New origin
            ///
            /// \see getOrigin
            ///
            ////////////////////////////////////////////////////////////
            void setOrigin(const sf::Vector2f& origin);

            ////////////////////////////////////////////////////////////
            /// \brief get the position of the object
            ///
            /// \return Current position
            ///
            /// \see setPosition
            ///
            ////////////////////////////////////////////////////////////
            const sf::Vector2f& getPosition() const;

            ////////////////////////////////////////////////////////////
            /// \brief get the orientation of the object
            ///
            /// The rotation is always in the range [0, 360].
            ///
            /// \return Current rotation, in degrees
            ///
            /// \see setRotation
            ///
            ////////////////////////////////////////////////////////////
            float getRotation() const;

            ////////////////////////////////////////////////////////////
            /// \brief get the current scale of the object
            ///
            /// \return Current scale factors
            ///
            /// \see setScale
            ///
            ////////////////////////////////////////////////////////////
            const sf::Vector2f& getScale() const;

            ////////////////////////////////////////////////////////////
            /// \brief get the local origin of the object
            ///
            /// \return Current origin
            ///
            /// \see setOrigin
            ///
            ////////////////////////////////////////////////////////////
            const sf::Vector2f& getOrigin() const;

            ////////////////////////////////////////////////////////////
            /// \brief Move the object by a given offset
            ///
            /// This function adds to the current position of the object,
            /// unlike setPosition which overwrites it.
            /// Thus, it is equivalent to the following code:
            /// \code
            /// sf::Vector2f pos = object.getPosition();
            /// object.setPosition(pos.x + offsetX, pos.y + offsetY);
            /// \endcode
            ///
            /// \param offsetX X offset
            /// \param offsetY Y offset
            ///
            /// \see setPosition
            ///
            ////////////////////////////////////////////////////////////
            void move(float offsetX, float offsetY);

            ////////////////////////////////////////////////////////////
            /// \brief Move the object by a given offset
            ///
            /// This function adds to the current position of the object,
            /// unlike setPosition which overwrites it.
            /// Thus, it is equivalent to the following code:
            /// \code
            /// object.setPosition(object.getPosition() + offset);
            /// \endcode
            ///
            /// \param offset Offset
            ///
            /// \see setPosition
            ///
            ////////////////////////////////////////////////////////////
            void move(const sf::Vector2f& offset);

            ////////////////////////////////////////////////////////////
            /// \brief Rotate the object
            ///
            /// This function adds to the current rotation of the object,
            /// unlike setRotation which overwrites it.
            /// Thus, it is equivalent to the following code:
            /// \code
            /// object.setRotation(object.getRotation() + angle);
            /// \endcode
            ///
            /// \param angle Angle of rotation, in degrees
            ///
            ////////////////////////////////////////////////////////////
            void rotate(float angle);

            ////////////////////////////////////////////////////////////
            /// \brief Scale the object
            ///
            /// This function multiplies the current scale of the object,
            /// unlike setScale which overwrites it.
            /// Thus, it is equivalent to the following code:
            /// \code
            /// sf::Vector2f scale = object.getScale();
            /// object.setScale(scale.x * factorX, scale.y * factorY);
            /// \endcode
            ///
            /// \param factorX Horizontal scale factor
            /// \param factorY Vertical scale factor
            ///
            /// \see setScale
            ///
            ////////////////////////////////////////////////////////////
            void scale(float factorX, float factorY);

            ////////////////////////////////////////////////////////////
            /// \brief Scale the object
            ///
            /// This function multiplies the current scale of the object,
            /// unlike setScale which overwrites it.
            /// Thus, it is equivalent to the following code:
            /// \code
            /// sf::Vector2f scale = object.getScale();
            /// object.setScale(scale.x * factor.x, scale.y * factor.y);
            /// \endcode
            ///
            /// \param factors Scale factors
            ///
            /// \see setScale
            ///
            ////////////////////////////////////////////////////////////
            void scale(const sf::Vector2f& factors);

            ////////////////////////////////////////////////////////////
            /// \brief get the combined transform of the object
            ///
            /// \return Transform combining the position/rotation/scale/origin of the object
            ///
            /// \see getInverseTransform
            ///
            ////////////////////////////////////////////////////////////
            const sf::Transform& getTransform() const;

            ////////////////////////////////////////////////////////////
            /// \brief get the inverse of the combined transform of the object
            ///
            /// \return Inverse of the combined transformations applied to the object
            ///
            /// \see getTransform
            ///
            ////////////////////////////////////////////////////////////
            const sf::Transform& getInverseTransform() const;

        private:
            void createSprite();

            sf::Sprite* m_sprite;
            const sf::Texture* m_texture;
            sf::Texture* m_selfOwnedTexture;
        };
    }
}
