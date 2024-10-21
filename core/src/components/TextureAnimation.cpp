#include "components/TextureAnimation.h"
#include "assets/TextureManager.h"
#include "Scene/Scene.h"

namespace QSFML
{
	namespace Components
	{
		TextureAnimation::TextureAnimation(
			const sf::Texture& texture, 
			const sf::Vector2u& imageCount,
			const std::string& name)
			: Component(name)
			, Updatable()
			, m_texture(texture)
			, m_imageDim(imageCount)
		{
			m_currentUVRect.width = m_texture.getSize().x / m_imageDim.x;
			m_currentUVRect.height = m_texture.getSize().y / m_imageDim.y;
			m_repeating = false;
			m_1InvSpeed = 1.0f;
			m_speed = 1.0f;
			m_animationDirection = 1;
			m_currentTextureIndex = 0;
			m_elapsedTime = 0;
			m_timeDomain = TimeDomain::realSimulationTime;

			//m_imageCount = imageCount.x * imageCount.y;
			//m_currentImageIndex = 0;
			setEnabled(false);
		}
		TextureAnimation::TextureAnimation(
			const std::string& textureID, 
			const sf::Vector2u& imageCount,
			const std::string& name)
			: Component(name)
			, Updatable()
			, m_texture(Assets::TextureManager::getTexture(textureID))
			, m_imageDim(imageCount)
		{
			m_currentUVRect.width = m_texture.getSize().x / m_imageDim.x;
			m_currentUVRect.height = m_texture.getSize().y / m_imageDim.y;
			m_repeating = false;
			m_1InvSpeed = 1.0f;
			m_speed = 1.0f;
			m_animationDirection = 1;
			m_currentTextureIndex = 0;
			m_elapsedTime = 0;
			m_timeDomain = TimeDomain::realSimulationTime;

			//m_imageCount = imageCount.x * imageCount.y;
			//m_currentImageIndex = 0;
			setEnabled(false);
		}

		TextureAnimation::TextureAnimation(const TextureAnimation& other)
			: Component(other)
			, Updatable()
			, m_texture(other.m_texture)
			, m_imageDim(other.m_imageDim)
			, m_currentUVRect(other.m_currentUVRect)
			, m_currentAnimationStep(other.m_currentAnimationStep)
			, m_currentAnimationSequence(other.m_currentAnimationSequence)
			, m_currentTextureIndex(other.m_currentTextureIndex)
			, m_elapsedTime(other.m_elapsedTime)
			, m_timeDomain(other.m_timeDomain)
			, m_repeating(other.m_repeating)
			, m_speed(other.m_speed)
			, m_1InvSpeed(other.m_1InvSpeed)
			, m_animationDirection(other.m_animationDirection)
			, m_animations(other.m_animations)
			, m_sumDuration(other.m_sumDuration)
			, m_currentAnimationTime(other.m_currentAnimationTime)
			, m_shape(other.m_shape)
			, m_sprite(other.m_sprite)
		{
			m_animationSequence.reserve(other.m_animationSequence.size());
			for (size_t i = 0; i < other.m_animationSequence.size(); i++)
			{
				m_animationSequence.push_back(&m_animations[other.m_animationSequence[i]->name]);
			}
		}
		TextureAnimation::~TextureAnimation()
		{

		}
		COMPONENT_IMPL(TextureAnimation);

		Log::LogObject& TextureAnimation::logger()
		{
			static Log::LogObject logger("TextureAnimation");
			return logger;
		}

		void TextureAnimation::start(const QSFML::string& animationName)
		{
			m_animationSequence.clear();
			const auto &it = m_animations.find(animationName);
#ifdef QSFML_DEBUG
			if (it == m_animations.end())
			{
				logger().logError("Invalid animation name: \""+std::string(animationName.c_str())+"\"");
				QSFML_DEBUG_BREAK();
				return;
			}
#endif
			m_animationSequence.push_back(&it->second);
			setEnabled(true);
		}
		void TextureAnimation::start(const QSFML::vector<QSFML::string>& sequence)
		{
			if (sequence.size() == 0)
			{
				logger().logWarning("Animation sequence is empty, nothing to animate");
				return;
			}
			m_animationSequence.clear();
			m_animationSequence.reserve(sequence.size());
			for (size_t i = 0; i < sequence.size(); i++)
			{
				const auto it = m_animations.find(sequence[i]);
				if (it == m_animations.end())
				{
#ifdef QSFML_DEBUG
					logger().logError("Invalid animation name: \"" + std::string(sequence[i].c_str()) + "\"");
					QSFML_DEBUG_BREAK();
#endif
					return;
				}
				else
				{
					m_animationSequence.push_back(&it->second);
				}
			}
			setEnabled(true);
		}
		void TextureAnimation::start()
		{
			if (m_animationSequence.size() == 0)
			{
				logger().logWarning("Animation sequence is empty, nothing to animate");
				return;
			}
			setEnabled(true);
		}
		void TextureAnimation::stop()
		{
			setEnabled(false);
		}

		void TextureAnimation::setSpeed(float speed)
		{
			m_speed = speed;
			std::signbit(speed) ? m_animationDirection = -1 : m_animationDirection = 1;
			if (speed == 0)
			{
				m_1InvSpeed = 999999;
				return;
			}
			
			m_1InvSpeed = std::abs(1 / speed);
		}

		float TextureAnimation::getProgress() const
		{
			return m_currentAnimationTime / m_sumDuration;
		}

		void TextureAnimation::bindTo(sf::Shape* shape)
		{
			m_shape = shape;
			if (m_shape)
			{
				m_shape->setTexture(&m_texture);
				setTextureIndex(m_imageDim.x * m_imageDim.y);
			}
		}
		void TextureAnimation::bindTo(sf::Sprite* sprite)
		{
			m_sprite = sprite;
			if (m_sprite)
			{
				m_sprite->setTexture(m_texture);
				setTextureIndex(m_imageDim.x * m_imageDim.y);
			}
		}

		void TextureAnimation::onEnable()
		{
			m_currentAnimationStep = 0;
			m_currentAnimationSequence = 0;
			m_elapsedTime = 0;
			m_currentAnimationTime = 0;
			m_currentTextureIndex = 0;

			if (m_speed == 0)
			{
				int currentIndex = m_currentTextureIndex;
				setEnabled(false);
				setTextureIndex(currentIndex);
				return;
			}

			// Calculate the sum duration
			m_sumDuration = 0;
			for (const auto& animation : m_animationSequence)
			{
				for (const auto& step : animation->steps)
				{
					m_sumDuration += step.duration;
				}
			}
			setTextureIndex(m_animationSequence[m_currentAnimationSequence]->steps[m_currentAnimationStep].index);
		}
		void TextureAnimation::onDisable()
		{
			m_currentAnimationStep = 0;
			m_currentAnimationSequence = 0;
			m_elapsedTime = 0;


			// Select texture element outside of the image to make it invisible
			setTextureIndex(m_imageDim.x * m_imageDim.y);
		}

		void TextureAnimation::update()
		{
			float deltaT = 0;
			switch (m_timeDomain)
			{
				case TimeDomain::realSimulationTime:
					deltaT = getDeltaT();
					break;
				case TimeDomain::fixedSimulationTime:
					deltaT = getFixedDeltaT();
					break;
			} 
			Animation& animation = *m_animationSequence[m_currentAnimationSequence];

			m_elapsedTime += deltaT;
			if (m_elapsedTime >= animation.steps[m_currentAnimationStep].duration * m_1InvSpeed)
			{
				m_currentAnimationTime += animation.steps[m_currentAnimationStep].duration;
				m_elapsedTime = 0;
				m_currentAnimationStep += m_animationDirection;
				int animStepSize = animation.steps.size();
				if (m_currentAnimationStep >= animStepSize || m_currentAnimationStep < 0 )
				{
					m_currentAnimationStep = (animStepSize+m_currentAnimationStep) % animStepSize;
					m_currentAnimationSequence += m_animationDirection;

					int animSeqSize = m_animationSequence.size();
					if (m_currentAnimationSequence >= animSeqSize || m_currentAnimationSequence < 0)
					{
						m_currentAnimationSequence = (animSeqSize+m_currentAnimationSequence) % animSeqSize;
						if (!m_repeating)
						{
							setEnabled(false);
							return;
						}
						m_currentAnimationTime = 0;
					}
				}
				setTextureIndex(m_animationSequence[m_currentAnimationSequence]->steps[m_currentAnimationStep].index);
			}
		}

		void TextureAnimation::setTextureIndex(unsigned int index)
		{
			m_currentTextureIndex = index;
			m_currentUVRect.left = (index % m_imageDim.x) * m_currentUVRect.width;
			m_currentUVRect.top = (index / m_imageDim.x) * m_currentUVRect.height;
			if (m_shape)
			{
				m_shape->setTextureRect(m_currentUVRect);
			}
			if (m_sprite)
			{
				m_sprite->setTextureRect(m_currentUVRect);
			}
		}
	}
}