#pragma once

#include "QSFML_base.h"
#include "components/base/Component.h"
#include "components/base/Drawable.h"

#include "utilities/Ray.h"
#include "utilities/AABB.h"
#include "utilities/CollisionInfo.h"

#include <qobject.h>
#include <vector>

namespace QSFML
{
namespace Components
{
    class Collider;
    

    class QSFML_EDITOR_WIDGET_EXPORT Collider: public Component
    {
            
        public:
            class QSFML_EDITOR_WIDGET_EXPORT Painter;
            friend Painter;
            explicit Collider(const std::string &name = "Collider");
            Collider(const Collider &other);
            ~Collider();

            COMPONENT_DECL(Collider);

            Collider& operator=(const Collider& other);

            /*
             *  \brief Sets the vertecies of the collider. Relative position to this colliders position.
             *         Vertecies have to be set clockwise
             */
            void setVertecies(const std::vector<sf::Vector2f>& vertecies);
            void addVertex(const sf::Vector2f& vertex);
            size_t getVertexCount() const;
            const std::vector<sf::Vector2f>& getVertecies() const;
            const Utilities::AABB& getBoundingBox() const;
            
            void clear();
            void setPos(const sf::Vector2f& pos);
            void move(const sf::Vector2f& offset);
            const sf::Vector2f& getPos() const;

            void checkCollision(const std::vector<Objects::CanvasObject*>& objs,
                                std::vector<Utilities::Collisioninfo>& collisions,
                                bool onlyFirstCollisionPerObject = true) const;
            void checkCollision(const std::vector<Components::Collider*> &other,
                std::vector<Utilities::Collisioninfo>& collisions,
                bool onlyFirstCollisionPerObject = true) const;

            bool checkCollision(Collider* other, std::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
            bool contains(const sf::Vector2f& point);

            static bool contains(const std::vector<sf::Vector2f>& polygon, 
                                 const sf::Vector2f& point,
                                 const sf::Vector2f& polygonPos = sf::Vector2f(0,0));

            Painter* createPainter();

            void resolveCollision(Collider* other);
            void resolveCollision(const std::vector<Collider*>& other);

            static bool isSeparatingAxis(const std::vector<sf::Vector2f>& polygonA, const std::vector<sf::Vector2f>& polygonB, const sf::Vector2f& axis);
            static sf::Vector2f getMinimumTranslationVector(const std::vector<sf::Vector2f>& polygonA, const std::vector<sf::Vector2f>& polygonB, const sf::Vector2f& axis);
            static sf::Vector2f normalize(const sf::Vector2f& vector);
            static sf::Vector2f resolveCollision(std::vector<sf::Vector2f>& polygonA, std::vector<sf::Vector2f>& polygonB);


            class QSFML_EDITOR_WIDGET_EXPORT Painter : public Components::Drawable
            {
                friend Collider;
                Painter(Collider* collider, const std::string& name = "ColliderPainter");
                Painter(const Painter& other);
                COMPONENT_DECL(Painter);
            public:
                
                ~Painter();
                

                void draw(sf::RenderTarget& target,
                          sf::RenderStates states) const override;

                void setColor(const sf::Color& color);
                const sf::Color& getColor() const;

                void setColorAABB(const sf::Color& color);
                const sf::Color& getColorAABB() const;
                void setColorCollider(const sf::Color& color);
                const sf::Color& getColorCollider() const;

            private:
                void onColliderDelete();
                Collider* m_collider;
                sf::Color m_aabbColor;
                sf::Color m_colliderColor;
            };


        protected:
            
            void calculateBoundingBox();
            void calculateAbsPos();
            void onPainterDeleted(Painter* p);

            
    
            std::vector<sf::Vector2f> m_absoluteVertices;
            std::vector<sf::Vector2f> m_relativeVertices;
            sf::Vector2f m_pos;
            Utilities::AABB m_boundingBox;

            std::vector<Painter*> m_painters;
    };


}
}
