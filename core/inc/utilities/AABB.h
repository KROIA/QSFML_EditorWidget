#pragma once

#include "SFML/Graphics.hpp"
#include "QSFML_EditorWidget_base.h"

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
        /*AABB();
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

        float getLeft() const;                     //!<\return the left edges X coordinate.
        float getRight() const;                    //!<\return the right edges X coordinate.
        float getTop() const;                      //!<\return the top edges Y coordinate.
        float getBottom() const;                   //!<\return the bottom edges Y coordinate.

        void setLeft(float x);                     //!<\brief Sets the left edges X coordinate.
        void setRight(float x);                    //!<\brief Sets the right edges X coordinate.
        void setTop(float y);                      //!<\brief Sets the top edges Y coordinate.
        void setBottom(float y);                   //!<\brief Sets the bottom edges Y coordinate.

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
        */
        AABB()
        {
            setPos({ 0,0 });
            setSize({ 0,0 });
        }
        AABB(const AABB& other)
        {
            this->m_pos = other.m_pos;
            this->m_size = other.m_size;
        }
        AABB(const sf::Vector2f& pos, const sf::Vector2f& size)
        {
            setPos(pos);
            setSize(size);
        }
        AABB(float x, float y, float width, float height)
        {
            setPos({ x,y });
            setSize({ width,height });
        }
        ~AABB()
        {

        }
        const AABB& operator=(const AABB& other)
        {
            this->m_pos = other.m_pos;
            this->m_size = other.m_size;
            return *this;
        }
        bool operator==(const AABB& other) const
        {
            return this->m_size == other.m_size &&
                this->m_pos == other.m_pos;
        }
        bool operator!=(const AABB& other) const
        {
            return this->m_size != other.m_size ||
                this->m_pos != other.m_pos;
        }
        bool operator<(const AABB& other) const
        {
            return m_size.x * m_size.y < other.m_size.x * other.m_size.y;
        }
        bool operator>(const AABB& other) const
        {
            return m_size.x * m_size.y > other.m_size.x * other.m_size.y;
        }
        bool operator<=(const AABB& other) const
        {
            return m_size.x * m_size.y <= other.m_size.x * other.m_size.y;
        }
        bool operator>=(const AABB& other) const
        {
            return m_size.x * m_size.y >= other.m_size.x * other.m_size.y;
        }

        void setPos(const sf::Vector2f& pos)
        {
            m_pos = pos;
        }
        void setCenterPos(const sf::Vector2f& pos)
        {
            setPos(pos - m_size / 2.f);
        }
        void setX(float x)
        {
            m_pos.x = x;
        }
        void setY(float y)
        {
            m_pos.y = y;
        }
        void setSize(const sf::Vector2f& size)
        {
            m_size = size;
#ifdef AABB_VALID_RECT_CHECK
            if (m_size.x < 0)
            {
                m_pos.x += m_size.x;
                m_size.x = -m_size.x;
            }
            if (m_size.y < 0)
            {
                m_pos.y += m_size.y;
                m_size.y = -m_size.y;
            }
#endif
        }
        void setWidth(float width)
        {
            m_size.x = width;
#ifdef AABB_VALID_RECT_CHECK
            if (m_size.x < 0)
            {
                m_pos.x += m_size.x;
                m_size.x = -m_size.x;
            }
#endif
        }
        void setHeight(float height)
        {
            m_size.y = height;
#ifdef AABB_VALID_RECT_CHECK
            if (m_size.y < 0)
            {
                m_pos.y += m_size.y;
                m_size.y = -m_size.y;
            }
#endif
        }
        void move(const sf::Vector2f& delta)
        {
            m_pos += delta;
        }
        const sf::Vector2f& getPos() const
        {
            return m_pos;
        }
        const sf::Vector2f& TL() const
        {
            return m_pos;
        }
        sf::Vector2f TR() const
        {
            sf::Vector2f p = m_pos;
            p.x += m_size.x;
            return p;
        }
        sf::Vector2f BR() const
        {
            return m_pos + m_size;
        }
        sf::Vector2f BL() const
        {
            sf::Vector2f p = m_pos;
            p.y += m_size.y;
            return p;
        }
        const sf::Vector2f& getSize() const
        {
            return m_size;
        }
        float getLeft() const
        {
            return m_pos.x;
        }
        float getRight() const
        {
            return m_pos.x + m_size.x;
        }
        float getTop() const
        {
            return m_pos.y;
        }
        float getBottom() const
        {
            return m_pos.y + m_size.y;
        }

        void setLeft(float x)
        {
            float right = m_pos.x + m_size.x;
            m_pos.x = x;
            m_size.x = right - x;
        }
        void setRight(float x)
        {
            m_size.x = x - m_pos.x;
        }
        void setTop(float y)
        {
            float bottom = m_pos.y + m_size.y;
            m_pos.y = y;
            m_size.y = bottom - y;
        }
        void setBottom(float y)
        {
            m_pos.y = y - m_size.y;
        }

        bool contains(const sf::Vector2f& point) const
        {
            return (m_pos.x <= point.x && m_pos.x + m_size.x >= point.x) &&
                (m_pos.y <= point.y && m_pos.y + m_size.y >= point.y);
        }
        bool intersects(const AABB& b) const
        {
            return (m_pos.x <= b.m_pos.x + b.m_size.x && m_pos.x + m_size.x >= b.m_pos.x) &&
                (m_pos.y <= b.m_pos.y + b.m_size.y && m_pos.y + m_size.y >= b.m_pos.y);
        }
        bool intersectsInverseOf(const AABB& b) const
        {
            return !(m_pos.x < b.m_pos.x && m_pos.x + m_size.x > b.m_pos.x + b.m_size.x &&
                m_pos.y < b.m_pos.y && m_pos.y + m_size.y > b.m_pos.y + b.m_size.y);
        }
        bool contains(const AABB& b) const
        {
            return (m_pos.x < b.m_pos.x && m_pos.x + m_size.x > b.m_pos.x + b.m_size.x &&
                m_pos.y < b.m_pos.y && m_pos.y + m_size.y > b.m_pos.y + b.m_size.y);
        }
        bool isLeftOf(const AABB& b) const
        {
            return b.m_pos.x > m_pos.x + m_size.x;
        }
        bool isRightOf(const AABB& b) const
        {
            return m_pos.x > b.m_pos.x + b.m_size.x;
        }
        bool isOnTopOf(const AABB& b) const
        {
            return b.m_pos.y > m_pos.y + m_size.y;
        }
        bool isBelowOf(const AABB& b) const
        {
            return m_pos.y > b.m_pos.y + b.m_size.y;
        }
        bool intersectsTopOf(const AABB& b) const
        {
            return m_pos.y < b.m_pos.y && m_pos.y + m_size.y > b.m_pos.y;
        }
        bool intersectsLeftOf(const AABB& b) const
        {
            return m_pos.x < b.m_pos.x && m_pos.x + m_size.x > b.m_pos.x;
        }
        bool intersectsBottomOf(const AABB& b) const
        {
            return m_pos.y < b.m_pos.y + b.m_size.y && m_pos.y + m_size.y > b.m_pos.y + b.m_size.y;
        }
        bool intersectsRightOf(const AABB& b) const
        {
            return m_pos.x < b.m_pos.x + b.m_size.x && m_pos.x + m_size.x > b.m_pos.x + b.m_size.x;
        }

        static bool contains(const AABB& a, const sf::Vector2f& point)
        {
            return (a.m_pos.x <= point.x && a.m_pos.x + a.m_size.x >= point.x) &&
                (a.m_pos.y <= point.y && a.m_pos.y + a.m_size.y >= point.y);
        }
        static bool intersects(const AABB& a, const AABB& b)
        {
            return (a.m_pos.x <= b.m_pos.x + b.m_size.x && a.m_pos.x + a.m_size.x >= b.m_pos.x) &&
                (a.m_pos.y <= b.m_pos.y + b.m_size.y && a.m_pos.y + a.m_size.y >= b.m_pos.y);
        }
        static bool intersectsInverseOf(const AABB& a, const AABB& b)
        {
            return !(a.m_pos.x < b.m_pos.x && a.m_pos.x + a.m_size.x > b.m_pos.x + b.m_size.x &&
                a.m_pos.y < b.m_pos.y && a.m_pos.y + a.m_size.y > b.m_pos.y + b.m_size.y);
        }
        static bool contains(const AABB& a, const AABB& b)
        {
            return (a.m_pos.x < b.m_pos.x && a.m_pos.x + a.m_size.x > b.m_pos.x + b.m_size.x &&
                a.m_pos.y < b.m_pos.y && a.m_pos.y + a.m_size.y > b.m_pos.y + b.m_size.y);

        }
        static bool isLeftOf(const AABB& a, const AABB& b)
        {
            return b.m_pos.x > a.m_pos.x + a.m_size.x;
        }
        static bool isRightOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.x > b.m_pos.x + b.m_size.x;
        }
        static bool isOnTopOf(const AABB& a, const AABB& b)
        {
            return b.m_pos.y > a.m_pos.y + a.m_size.y;
        }
        static bool isBelowOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.y > b.m_pos.y + b.m_size.y;
        }
        static bool intersectsTopOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.y < b.m_pos.y && a.m_pos.y + a.m_size.y > b.m_pos.y;
        }
        static bool intersectsLeftOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.x < b.m_pos.x && a.m_pos.x + a.m_size.x > b.m_pos.x;
        }
        static bool intersectsBottomOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.y < b.m_pos.y + b.m_size.y && a.m_pos.y + a.m_size.y > b.m_pos.y + b.m_size.y;
        }
        static bool intersectsRightOf(const AABB& a, const AABB& b)
        {
            return a.m_pos.x < b.m_pos.x + b.m_size.x && a.m_pos.x + a.m_size.x > b.m_pos.x + b.m_size.x;
        }



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
