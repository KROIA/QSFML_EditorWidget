#pragma once
// #include "utilities/AABB.h"
#include <qobject.h>
#include "components/Component.h"
#include "components/Drawable.h"
#include "canvas/CanvasForwardDeclaration.h"
#include <vector>
#include "utilities/Ray.h"
#include "utilities/AABB.h"


namespace QSFML
{
namespace Components
{
    class Collider;
    struct QSFML_EDITOR_WIDGET_EXPORT Collisioninfo
    {
        const Collider *collider1;
        const Collider *collider2;
        struct QSFML_EDITOR_WIDGET_EXPORT VertexIndex
        {
            size_t index1;
            size_t index2;
        };
        VertexIndex vertexIndex; // Vector from this vertex to the next had a collision 
    };

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
            
            void clear();
            void setPos(const sf::Vector2f& pos);
            void move(const sf::Vector2f& offset);
            const sf::Vector2f& getPos() const;

            void checkCollision(const std::vector<Objects::CanvasObject*>& objs,
                                std::vector<Collisioninfo>& collisions,
                                bool onlyFirstCollisionPerObject = true) const;
            void checkCollision(const Objects::CanvasObject* obj,
                std::vector<Collisioninfo>& collisions,
                bool onlyFirstCollisionPerObject = true)const;

            bool checkCollision(Collider* other, std::vector<Collisioninfo>& collisions, bool onlyFirstCollision = true) const;

            Painter* createPainter();


            class QSFML_EDITOR_WIDGET_EXPORT Painter : /*public QObject, */public Components::Drawable
            {
                //Q_OBJECT
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
            //signals:
            //    void colliderDeleted(const Collider* collider);

            private:
                void onColliderDelete();
                Collider* m_collider;
                sf::Color m_aabbColor;
                sf::Color m_colliderColor;
            };


        protected:
            
            void calculateBoundingBox();
            void onPainterDeleted(Painter* p);

            
    
            std::vector<sf::Vector2f> m_vertices;
            sf::Vector2f m_pos;
            Utilities::AABB m_boundingBox;

            std::vector<Painter*> m_painters;
    };


}
}
