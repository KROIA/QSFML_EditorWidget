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
			m_animationDirection = 1;

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
			m_animationDirection = 1;

			//m_imageCount = imageCount.x * imageCount.y;
			//m_currentImageIndex = 0;
			setEnabled(false);
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
#ifdef QSFML_DEBUG
			const auto it = m_animations.find(animationName);
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
			setEnabled(true);
		}
		void TextureAnimation::stop()
		{
			setEnabled(false);
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
				setAnimationIndex(m_imageDim.x * m_imageDim.y);
			}
		}
		void TextureAnimation::bindTo(sf::Sprite* sprite)
		{
			m_sprite = sprite;
			if (m_sprite)
			{
				m_sprite->setTexture(m_texture);
				setAnimationIndex(m_imageDim.x * m_imageDim.y);
			}
		}

		void TextureAnimation::onEnable()
		{
			m_currentAnimationStep = 0;
			m_currentAnimationSequenceStep = 0;
			m_elapsedTime = 0;
			m_currentAnimationTime = 0;

			// Calculate the sum duration
			m_sumDuration = 0;
			for (const auto& animation : m_animationSequence)
			{
				for (const auto& step : animation->steps)
				{
					m_sumDuration += step.duration;
				}
			}
			setAnimationIndex(m_animationSequence[m_currentAnimationSequenceStep]->steps[m_currentAnimationStep].index);
		}
		void TextureAnimation::onDisable()
		{
			m_currentAnimationStep = 0;
			m_currentAnimationSequenceStep = 0;
			m_elapsedTime = 0;


			// Select texture element outside of the image to make it invisible
			setAnimationIndex(m_imageDim.x * m_imageDim.y);
		}

		void TextureAnimation::update()
		{
			float deltaT = getDeltaT();
			Animation& animation = *m_animationSequence[m_currentAnimationSequenceStep];

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
					m_currentAnimationSequenceStep += m_animationDirection;

					int animSeqSize = m_animationSequence.size();
					if (m_currentAnimationSequenceStep >= animSeqSize || m_currentAnimationSequenceStep < 0)
					{
						m_currentAnimationSequenceStep = (animSeqSize+m_currentAnimationSequenceStep) % animSeqSize;
						if (!m_repeating)
						{
							setEnabled(false);
							return;
						}
						m_currentAnimationTime = 0;
					}
					//animation = *m_animationSequence[m_currentAnimationSequenceStep];
				}
				setAnimationIndex(m_animationSequence[m_currentAnimationSequenceStep]->steps[m_currentAnimationStep].index);
			}
		}

		void TextureAnimation::setAnimationIndex(unsigned int index)
		{
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