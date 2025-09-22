#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics.hpp>
#include "components/base/Component.h"
#ifdef QSFML_USE_GL_DRAW
#include <SFML/OpenGL.hpp>
#endif

namespace QSFML
{
	namespace Components
	{
		class QSFML_EDITOR_WIDGET_EXPORT Transform : public Component
		{
		public:
        COMPONENT_HEADER_IMPL(Transform);

        //auto* clone() const override
        //{ 
        //    return new Transform(*this);
        //}

		static void setGizmoAxisColor(const sf::Color& xAxisColor, const sf::Color& yAxisColor)
		{
			s_xAxisColor = xAxisColor;
			s_yAxisColor = yAxisColor;
		}
		static const sf::Color& getGizmoXAxisColor()
		{
			return s_xAxisColor;
		}
		static const sf::Color& getGizmoYAxisColor()
		{
			return s_yAxisColor;
		}
        static void setGizmoAxisLength(float length)
        {
			s_axisLength = length;
        }
		static float getGizmoAxisLength()
		{
			return s_axisLength;
		}

        Transform()
			: Component("Transform")
			, m_needsTransformUpdate(true)
        {

        }
        Transform(const sf::Vector2f& pos, float rot)
            : Component("Transform")
        {
			setRotation(rot);
			setPosition(pos);
        }
        Transform(const Transform& other)
            : m_transformable(other.m_transformable)
            , m_needsTransformUpdate(other.m_needsTransformUpdate)
        {
            m_globalTransform = other.m_globalTransform;
        }

        bool operator==(const Transform& other) const
        {
            return m_transformable.getTransform() == other.m_transformable.getTransform();
        }
		bool operator!=(const Transform& other) const
		{
			return m_transformable.getTransform() != other.m_transformable.getTransform();
		}

        void setPosition(float x, float y)
        {
			m_transformable.setPosition(x, y);
            markDirty();
        }
        void setPosition(const sf::Vector2f& position)
        {
			m_transformable.setPosition(position);
            markDirty();
        }
        void setRotation(float angle)
        {
			m_transformable.setRotation(angle);
            markDirty();
        }
        void setScale(float factorX, float factorY)
        {
			m_transformable.setScale(factorX, factorY);
            markDirty();
        }
        void setScale(const sf::Vector2f& factors)
        {
			m_transformable.setScale(factors);
            markDirty();
        }
        void setOrigin(float x, float y)
        {
			m_transformable.setOrigin(x, y);
            markDirty();
        }
        void setOrigin(const sf::Vector2f& origin)
        {
			m_transformable.setOrigin(origin);
            markDirty();
        }
        const sf::Vector2f& getPosition() const
        {
			return m_transformable.getPosition();
        }
        float getRotation() const
        {
			return m_transformable.getRotation();
        }
        const sf::Vector2f& getScale() const
        {
			return m_transformable.getScale();
        }
        const sf::Vector2f& getOrigin() const
        {
			return m_transformable.getOrigin();
        }
        void move(float offsetX, float offsetY)
        {
			m_transformable.move(offsetX, offsetY);
            markDirty();
        }
        void move(const sf::Vector2f& offset)
        {
			m_transformable.move(offset);
            markDirty();
        }
        void rotate(float angle)
        {
			m_transformable.rotate(angle);
            markDirty();
        }
        void scale(float factorX, float factorY)
        {
			m_transformable.scale(factorX, factorY);
            markDirty();
        }
        void scale(const sf::Vector2f& factor)
        {
			m_transformable.scale(factor);
            markDirty();
        }
        const sf::Transform& getTransform() const
        {
			return m_transformable.getTransform();
        }
        const sf::Transform& getInverseTransform() const
        {
			return m_transformable.getInverseTransform();
        }
        const sf::Transform& getGlobalTransform() const
        {
            return m_globalTransform;
        }

		bool isDirty() const
		{
			return m_needsTransformUpdate;
		}
        void markDirty()
        {
            m_needsTransformUpdate = true;
        }
        void markUndirty() const
        {
            m_needsTransformUpdate = false;
        }

		void setParentTransform(const sf::Transform& parentTransform)
		{
            m_globalTransform = parentTransform * getTransform();
			markUndirty();
		}

        
        //const sf::Transform& getGlobalTransform() const
        //{
        //    // <! ToDo only update if needed
        //    updateGlobalTransform();
        //    return m_globalTransform;
        //}
		void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const override
		{
#ifdef QSFML_USE_GL_DRAW
            QSFML_UNUSED(target);
            QSFML_UNUSED(states);
            glLoadMatrixf(states.transform.getMatrix());

            // Enable blending to match SFML's alpha blending
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Iterate through the lines and draw each one using OpenGL

			

            glBegin(GL_LINES);
			glColor4ub(s_xAxisColor.r, s_xAxisColor.g, s_xAxisColor.b, s_xAxisColor.a);
			glVertex2f(0, 0);
			glVertex2f(s_axisLength, 0);

            glColor4ub(s_yAxisColor.r, s_yAxisColor.g, s_yAxisColor.b, s_yAxisColor.a);
			glVertex2f(0, 0);
			glVertex2f(0, s_axisLength);
            glEnd();

#else    
			sf::Vertex xAxis[] =
			{
				sf::Vertex(sf::Vector2f(0, 0), s_xAxisColor),
				sf::Vertex(sf::Vector2f(s_axisLength, 0), s_xAxisColor)
		    };
			sf::Vertex yAxis[] =
			{
				sf::Vertex(sf::Vector2f(0, 0), s_yAxisColor),
				sf::Vertex(sf::Vector2f(0, s_axisLength), s_yAxisColor)
			};
			target.draw(xAxis, 2, sf::Lines, states);
			target.draw(yAxis, 2, sf::Lines, states);
            
#endif
		}
			
		protected:
        
        void updateGlobalTransform() const
        {
           /* Transformable* parent = m_transformParent;
			QSFML::vector<const Transformable*> transformTree;
            transformTree.reserve(20);
			while (parent)
			{
				transformTree.push_back(parent);
				parent = parent->m_transformParent;
			}

			sf::Transform globalTransform;
            for (auto it = transformTree.rbegin(); it != transformTree.rend(); ++it)
            {
                globalTransform.combine((*it)->m_transformable.getTransform());
            }
			globalTransform.combine(m_transformable.getTransform());
			m_globalTransform = globalTransform;
			m_needsTransformUpdate = false;*/
        }

		private:


			//Transformable* m_transformParent = nullptr;
			//QSFML::vector<Transformable*> m_children;

			sf::Transformable m_transformable;

			mutable bool m_needsTransformUpdate;
			mutable sf::Transform m_globalTransform;

            static sf::Color s_xAxisColor;
            static sf::Color s_yAxisColor;
			static float s_axisLength;
		};

        
	}
}