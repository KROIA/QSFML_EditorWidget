#pragma once

#include "SFML/Graphics.hpp"
#include "QSFML_base.h"

namespace QSFML
{
namespace Utilities
{

//!<\def If this is set, it will check if the rectangle has a negative size and if so it will be corrected so,
//!      that the rectangle stays at the same position and size but the coordinates of the top left corner will have changed.
#define AABB_VALID_RECT_CHECK
class QSFML_EDITOR_WIDGET_EXPORT AABB
{
    public:
        AABB();
        AABB(const AABB &other);
        AABB(const sf::Vector2f &pos, const sf::Vector2f &size);
        AABB(float x, float y, float width, float height);
        ~AABB();
        const AABB &operator=(const AABB &other);
        bool operator==(const AABB &other) const;  //!<\return true if both rects have the same position and size.
        bool operator!=(const AABB &other) const;  //!<\return true if both rects do not have the same position and size.
        bool operator<(const AABB &other) const;   //!<\return true if this area is smaller than the area of ​​other.
        bool operator>(const AABB &other) const;   //!<\return true if this area is larger than the area of ​​other.
        bool operator<=(const AABB &other) const;  //!<\return true if this area is smaller equal than the area of ​​other.
        bool operator>=(const AABB &other) const;  //!<\return true if this area is larger equal than the area of ​​other.

        void setPos(const sf::Vector2f &pos);      //!<\brief Sets the top left corner of this.
        void setCenterPos(const sf::Vector2f &pos);//!<\brief Sets the position of the rectangle, so that the center of the rectangle is on the parameter pos.
        void setX(float x);                        //!<\brief Sets the top left corner's x position.
        void setY(float y);                        //!<\brief Sets the top left corner's y position.
        void setSize(const sf::Vector2f &size);    //!<\brief Sets size of this.
        void setWidth(float width);                //!<\brief Sets the width of this.
        void setHeight(float height);              //!<\brief Sets the height of this.
        void move(const sf::Vector2f &delta);      //!<\brief Translates this along the given vector.

        const sf::Vector2f &getPos() const;        //!<\return the position of the top left corner.
        const sf::Vector2f &TL() const;            //!<\return the position of the top left corner.
        sf::Vector2f TR() const;                   //!<\return the position of the top right corner.
        sf::Vector2f BR() const;                   //!<\return the position of the bottom right corner.
        sf::Vector2f BL() const;                   //!<\return the position of the bottom left corner.
        const sf::Vector2f &getSize() const;       //!<\return the size of this.

        sf::Vector2f getLeft() const;              //!<\return the left edges vector.
        sf::Vector2f getTop() const;               //!<\return the top edges vector.

        bool contains(const sf::Vector2f &point) const;  //!<\return true if the point is inside the rectangle.
        bool intersects(const AABB &b) const;            //!<\return true if there is any intersection of this and b.
        bool intersectsInverseOf(const AABB &b) const;   //!<\return true if b is not fully contained in this.
        bool contains(const AABB &b) const;              //!<\return true if b is fully contained in this.
        bool isLeftOf(const AABB &b) const;              //!<\return true if this is on the left side of b.
        bool isRightOf(const AABB &b) const;             //!<\return true if this is on the right side of b.
        bool isOnTopOf(const AABB &b) const;             //!<\return true if this is on top of b.
        bool isBelowOf(const AABB &b) const;             //!<\return true if this is below b.
        bool intersectsTopOf(const AABB &b) const;       //!<\return true if this intersects the top edge of b. This will be true even if both may not intersect.
        bool intersectsLeftOf(const AABB &b) const;      //!<\return true if this intersects the left edge of b. This will be true even if both may not intersect.
        bool intersectsBottomOf(const AABB &b) const;    //!<\return true if this intersects the bottom edge of b. This will be true even if both may not intersect.
        bool intersectsRightOf(const AABB &b) const;     //!<\return true if this intersects the right edge of b. This will be true even if both may not intersect.

        static bool contains(const AABB &a, const sf::Vector2f &point);    //!<\return true if the point is inside the rectangle.
        static bool intersects(const AABB &a, const AABB &b);              //!<\return true if there is any intersection of a and b.
        static bool intersectsInverseOf(const AABB &a, const AABB &b);     //!<\return true if b is not fully contained in a.
        static bool contains(const AABB &a, const AABB &b);                //!<\return true if b is fully contained in a.
        static bool isLeftOf(const AABB &a, const AABB &b);                //!<\return true if a is on the left side of b.
        static bool isRightOf(const AABB &a, const AABB &b);               //!<\return true if a is on the right side of b.
        static bool isOnTopOf(const AABB &a, const AABB &b);               //!<\return true if a is on top of b.
        static bool isBelowOf(const AABB &a, const AABB &b);               //!<\return true if a is below b.
        static bool intersectsTopOf(const AABB &a, const AABB &b);         //!<\return true if a intersects the top edge of b. a will be true even if both may not intersect.
        static bool intersectsLeftOf(const AABB &a, const AABB &b);        //!<\return true if a intersects the left edge of b. a will be true even if both may not intersect.
        static bool intersectsBottomOf(const AABB &a, const AABB &b);      //!<\return true if a intersects the bottom edge of b. a will be true even if both may not intersect.
        static bool intersectsRightOf(const AABB &a, const AABB &b);       //!<\return true if a intersects the right edge of b. a will be true even if both may not intersect.

        static AABB getFrame(const std::vector<AABB> &list);               //!<\return a AABB with the position and size, so that all AABB's in the list would fit in it.
                                                                           //!<        Its a frame around all rects from the list.
        static AABB getFrame(const std::vector<sf::Vector2f>& points);     //!<\return a AABB with the position and size, so that all points in the list would fit in it.
                                                                           //!<        Its a frame around all points from the list.
    private:
        sf::Vector2f m_pos;
        sf::Vector2f m_size;
};
}
}
