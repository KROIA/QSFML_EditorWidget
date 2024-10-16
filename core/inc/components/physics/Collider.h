#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "components/base/Drawable.h"

#include "utilities/Ray.h"
#include "utilities/AABB.h"
#include "utilities/Collisioninfo.h"

#include <qobject.h>
#include <vector>
#include <vector>

namespace QSFML
{
namespace Components
{
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
            void setVertecies(const QSFML::vector<sf::Vector2f>& vertecies);
            void addVertex(const sf::Vector2f& vertex);
            size_t getVertexCount() const;
            const QSFML::vector<sf::Vector2f>& getVertecies() const;
            const Utilities::AABB& getBoundingBox() const;
            
            void clear();
            void setPos(const sf::Vector2f& pos);
            void move(const sf::Vector2f& offset);
            const sf::Vector2f& getPos() const;

            bool checkCollision(const QSFML::vector<Objects::GameObjectPtr>& objs,
                                QSFML::vector<Utilities::Collisioninfo>& collisions,
                                bool onlyFirstCollisionPerObject = true) const;
            bool checkCollision(const QSFML::vector<Components::Collider*> &other,
                QSFML::vector<Utilities::Collisioninfo>& collisions,
                bool onlyFirstCollisionPerObject = true) const;

            bool checkCollision(Collider* other, QSFML::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
            void checkCollision_noAABB(const QSFML::vector<Components::Collider*>& other, QSFML::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
            bool checkCollision_noAABB(Collider* other, QSFML::vector<Utilities::Collisioninfo>& collisions, bool onlyFirstCollision = true) const;
            bool contains(const sf::Vector2f& point);

            static bool contains(const QSFML::vector<sf::Vector2f>& polygon, 
                                 const sf::Vector2f& point);

            Painter* createPainter();

			bool isDirty() const { return m_dirty; }
            void updateColliderData() const;

            class QSFML_EDITOR_WIDGET_EXPORT Painter : public Components::Drawable
            {
                friend Collider;
                Painter(Collider* collider, const std::string& name = "ColliderPainter");
                Painter(const Painter& other);
                COMPONENT_DECL(Painter);
            public:
                
                ~Painter();
                

                void drawComponent(sf::RenderTarget& target,
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
            static inline bool doLineSegmentsIntersect(const sf::Vector2f &p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, float &scalar1, float &scalar2);



            void calculateBoundingBox()const;
            void calculateAbsPos() const;
            void onPainterDeleted(Painter* p);
			void markDirty() const { m_dirty = true; }
			void markUndirty() const { m_dirty = false; }


            
    
            mutable QSFML::vector<sf::Vector2f> m_absoluteVertices;
            QSFML::vector<sf::Vector2f> m_relativeVertices;
            sf::Vector2f m_pos;
            mutable Utilities::AABB m_boundingBox;
			mutable bool m_dirty = true;

            QSFML::vector<Painter*> m_painters;
    };


}
}
