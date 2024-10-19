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
		/**
		 * @brief 
		 * This class is used to animate a texture.
		 * It is used to animate a texture by changing the UV coordinates of the texture.
		 * The specified texture is divided into a grid of images which must be equal in size.
		 * You can define multiple animations with different sequences of images and durations.
		 * Each animation specifies the order of subimage indices and the duration of each subimage.
		 * 
		 * The source texture must follow the following rules:
		 * 1. The texture must be divided into a grid of images.
		 * 2. The sub images must be of equal size.
		 * 3. The indexing of the subimages starts from 0 and goes from left to right and top to bottom.
		 * 
		 * Indexing example:
		 * +----+----+----+----+
		 * | 0  | 1  | 2  | 3  |
		 * +----+----+----+----+
		 * | 4  | 5  | 6  | 7  |
		 * +----+----+----+----+
		 */
		class QSFML_EDITOR_WIDGET_EXPORT TextureAnimation : public Component, public Utilities::Updatable
		{
		public:
			
			/**
			 * @brief 
			 * This struct is used to define a single step in an animation.
			 * It contains the index of the subimage and the duration for the subimage.
			 */
			struct Step
			{
				unsigned int index;
				float duration;
			};

			/**
			 * @brief 
			 * This struct is used to define an animation.
			 * A animation is a sequence of steps which are executed in order.
			 */
			struct Animation
			{
				QSFML::string name;				//<! The name of the animation
				QSFML::vector<Step> steps;      //<! The sequence of steps in the animation

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

			/**
			 * @brief 
			 * Creates a new TextureAnimation component.
			 * @param texture to use for the animations
			 * @param imageCount x and y amount of the subimages in the grid
			 * @param name of the component
			 */
			TextureAnimation(
				const sf::Texture& texture, 
				const sf::Vector2u& imageCount, 
				const std::string& name = "TextureAnimation");

			/**
			 * @brief 
			 * Creates a new TextureAnimation component.
			 * @param textureID string, used to find the texture in the texture manager
			 * @param imageCount x and y amount of the subimages in the grid
			 * @param name of the component
			 */
			TextureAnimation(
				const std::string& textureID, 
				const sf::Vector2u& imageCount, 
				const std::string& name = "TextureAnimation");

			TextureAnimation(const TextureAnimation& other);

			virtual ~TextureAnimation();

			COMPONENT_DECL(TextureAnimation);

			/**
			 * @brief 
			 * Logger for TextureAnimations
			 * @return 
			 */
			static Log::LogObject& logger();

			/**
			 * @brief 
			 * Returns the current UV rectangle of the subimage.
			 * Apply it to the shape or sprite to display the subimage.
			 * mySprite.setTextureRect(myTextureAnimation.getUVRect());
			 * @return Rectangle with the UV coordinates of the subimage
			 */
			const sf::IntRect& getUVRect() const
			{
				return m_currentUVRect;
			}

			/**
			 * @brief 
			 * Returns the texture used for the animations.
			 * @return texture used for the animations
			 */
			const sf::Texture& getTexture() const
			{
				return m_texture;
			}

			/**
			 * @brief 
			 * Adds a new animation to the component.
			 * Each animation must have a unique name.
			 * @warning Do not call this function when a animation is running.
			 * @param animation 
			 */
			void addAnimation(const Animation& animation)
			{
				m_animations[animation.name] = animation;
			}

			/**
			 * @brief 
			 * Adds a new animation to the component using move semantics.
			 * Each animation must have a unique name.
			 * @warning Do not call this function when a animation is running.
			 * @param animation 
			 */
			void addAnimation(const Animation&& animation)
			{
				m_animations.insert({ animation.name, std::move(animation) });
			}

			/**
			 * @brief 
			 * Removes all animations from the component.
			 */
			void clearAnimations()
			{
				setEnabled(false);
				m_animations.clear();
				m_animationSequence.clear();
			}

			/**
			 * @brief 
			 * Sets the animation in loop mode
			 * @param repeating, if set to true, a running animation will repeat after it has finished
			 */
			void setRepeating(bool repeating)
			{
				m_repeating = repeating;
			}

			/**
			 * @brief 
			 * Returns if the animation is in loop mode
			 * @return true if the animation is in loop mode
			 */
			bool isRepeating() const
			{
				return m_repeating;
			}

			/**
			 * @brief
			 * Returns if the animation is running
			 * @return true if the animation is running
			 */
			bool isRunning() const
			{
				return isEnabled();
			}

			/**
			 * @brief 
			 * Sets the current used subimage using the index.
			 * If the index is higher than the amount of subimages, the texture uv rectangle is set 
			 * outside of the texture -> no subimage is displayed.
			 * @param index 
			 */
			void setTextureIndex(unsigned int index);

			unsigned int getTextureIndex() const
			{
				return m_currentTextureIndex;
			}

			/**
			 * @brief 
			 * Starts an animation with the specified name.
			 * @param animationName for the animation
			 */
			void start(const QSFML::string &animationName);

			/**
			 * @brief 
			 * Starts an animation with the specified sequence of animations.
			 * Each animation in the sequence is executed in order.
			 * @param sequence of animations
			 */
			void start(const QSFML::vector<QSFML::string> &sequence);

			/**
			 * @brief 
			 * Starts the last used animation.
			 */
			void start();

			/**
			 * @brief 
			 * Stops the current animation.
			 */
			void stop();

			/**
			 * @brief 
			 * Sets the speed factor of the animation.
			 * Each step time gets divided by the speed.
			 * If the speed is negative, the animation is played in reverse.
			 * If the speed is set to 0, the speed is set to 1*e-6.
			 * @param speed 
			 */
			void setSpeed(float speed);

			/**
			 * @brief 
			 * Returns the speed factor of the animation.
			 * @return speed factor of the animation
			 */
			float getSpeed() const
			{
				return m_speed;
			}

			/**
			 * @brief 
			 * Sets the time domain of the animation.
			 * The time domain specifies how the time of the animation is calculated.
			 * @param domain 
			 */
			void setTimeDomain(TimeDomain domain)
			{
				m_timeDomain = domain;
			}

			/**
			 * @brief 
			 * Gets the time domain of the animation.
			 * @return the current time domain of the animation
			 */
			TimeDomain getTimeDomain() const
			{
				return m_timeDomain;
			}

			/**
			 * @brief 
			 * Returns the progress of the current animation.
			 * The progress is a value between 0 and 1.
			 * 0 means the animation is at the start and 1 means the animation is at the end.
			 * @return current animation progress
			 */
			float getProgress() const;

			/**
			 * @brief 
			 * Returns the current animation time.
			 * The time is the sum of all animation step times of the current animation run.
			 * @return 
			 */
			float getCurrentAnimationTime() const
			{
				return m_currentAnimationTime;
			}

			/**
			 * @brief 
			 * Gets the total time it takes to play the current animation.
			 * It ignores the speed factor.
			 * @return total time of the current animation
			 */
			float getAnimationTime() const
			{
				return m_sumDuration;
			}

			/**
			 * @brief 
			 * Bind the animation to a shape.
			 * Add the shape here, the texture gets applied to the shape.
			 * The texture rect of the shape gets updated by the animation.
			 * @param shape 
			 */
			void bindTo(sf::Shape* shape);

			/**
			 * @brief 
			 * Gets the shape the animation is bound to.
			 * @return current bound shape
			 */
			sf::Shape* getShape() const
			{
				return m_shape;
			}

			/**
			 * @brief 
			 * Bind the animation to a sprite.
			 * Add the sprite here, the texture gets applied to the sprite.
			 * The texture rect of the sprite gets updated by the animation.
			 * @param sprite 
			 */
			void bindTo(sf::Sprite* sprite);

			/**
			 * @brief 
			 * Gets the sprite the animation is bound to.
			 * @return current bound sprite
			 */
			sf::Sprite* getSprite() const
			{
				return m_sprite;
			}


		protected:
			void onEnable() override;
			void onDisable() override;

			/**
			 * @brief 
			 * Updates the animation
			 */
			void update() override;

			
		private:

			

			const sf::Texture& m_texture;
			const sf::Vector2u m_imageDim;

			sf::IntRect m_currentUVRect;			//<! Current UV rectangle of the subimage

			int m_currentAnimationStep;				//<! The current step in the current animation sequence
			int m_currentAnimationSequence;			//<! The current animation sequence
			unsigned int m_currentTextureIndex;		//<! The current subimage index

			float m_elapsedTime;
			bool m_repeating;

			float m_speed;
			float m_1InvSpeed; // 1/Speed of the animation

			int m_animationDirection;

			QSFML::unordered_map<QSFML::string, Animation> m_animations;
			QSFML::vector<Animation*> m_animationSequence;
			float m_sumDuration;
			float m_currentAnimationTime;

			TimeDomain m_timeDomain;

			sf::Shape* m_shape = nullptr;
			sf::Sprite* m_sprite = nullptr;
		};
	}
}