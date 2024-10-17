#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Component.h"
#include "utilities/Updatable.h"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace QSFML
{
	namespace Components
	{
		class QSFML_EDITOR_WIDGET_EXPORT TextureAnimation : public Component, public Utilities::Updatable
		{
		public:
			struct Step
			{
				unsigned int index;
				float duration;
			};

			struct Animation
			{
				QSFML::string name;
				QSFML::vector<Step> steps;

				// Create a constructor for init list
				Animation(const QSFML::string& name = "Animation") 
					: name(name) {}

				Animation(const QSFML::string& name, const QSFML::vector<Step>& steps)
					: name(name), steps(steps) {}

				Animation(const Animation& other)
					: name(other.name), steps(other.steps) {}

				Animation& operator=(const Animation& other)
				{
					name = other.name;
					steps = other.steps;
					return *this;
				}

				Animation& operator=(Animation&& other)
				{
					name = std::move(other.name);
					steps = std::move(other.steps);
					return *this;
				}

				Animation(Animation&& other)
					: name(std::move(other.name)), steps(std::move(other.steps)) {}

				~Animation() = default;

			};

			TextureAnimation(
				const sf::Texture& texture, 
				const sf::Vector2u& imageCount, 
				const std::string& name = "TextureAnimation");

			TextureAnimation(
				const std::string& textureID, 
				const sf::Vector2u& imageCount, 
				const std::string& name = "TextureAnimation");

			virtual ~TextureAnimation();

			COMPONENT_DECL(TextureAnimation);


			const sf::IntRect& getUVRect() const
			{
				return m_currentUVRect;
			}

			const sf::Texture& getTexture() const
			{
				return m_texture;
			}

			void addAnimation(const Animation& animation)
			{
				m_animations[animation.name] = animation;
			}
			void addAnimation(const Animation&& animation)
			{
				m_animations.insert({ animation.name, std::move(animation) });
			}
			void clearAnimations()
			{
				m_animations.clear();
			}

			void setRepeating(bool repeating)
			{
				m_repeating = repeating;
			}
			bool isRepeating() const
			{
				return m_repeating;
			}
			bool isRunning() const
			{
				return isEnabled();
			}

			void start(const QSFML::string &animationName);
			void start(const QSFML::vector<QSFML::string> &sequence);
			void start();
			void stop();

			void setSpeed(float speed)
			{
				if (speed < 0)
					m_animationDirection = -1;
				else
					m_animationDirection = 1;
				m_1InvSpeed = std::abs(1/speed);
			}
			float getSpeed() const
			{
				return m_animationDirection / m_1InvSpeed;
			}

			float getProgress() const;
			float getCurrentAnimationTime() const
			{
				return m_currentAnimationTime;
			}
			float getAnimationTime() const
			{
				return m_sumDuration;
			}

			void bindTo(sf::Shape* shape);
			sf::Shape* getShape() const
			{
				return m_shape;
			}

			void bindTo(sf::Sprite* sprite);
			sf::Sprite* getSprite() const
			{
				return m_sprite;
			}


		protected:
			void onEnable() override;
			void onDisable() override;
			void update() override;

			void setAnimationIndex(unsigned int index);

			static Log::LogObject& logger();
		private:

			

			const sf::Texture& m_texture;
			const sf::Vector2u m_imageDim;

			sf::IntRect m_currentUVRect;
			//unsigned int m_currentImageIndex;
			//unsigned int m_imageCount;
			//QSFML::string m_currentAnimation;
			int m_currentAnimationStep;
			int m_currentAnimationSequenceStep;

			//float m_startTime;
			float m_elapsedTime;
			bool m_repeating;
			float m_1InvSpeed; // 1/Speed of the animation
			int m_animationDirection;

			QSFML::unordered_map<QSFML::string, Animation> m_animations;
			QSFML::vector<Animation*> m_animationSequence;
			float m_sumDuration;
			float m_currentAnimationTime;

			//std::function<void(Animation&)> onFinished;

			sf::Shape* m_shape = nullptr;
			sf::Sprite* m_sprite = nullptr;
		};
	}
}