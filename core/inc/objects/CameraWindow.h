#pragma once

#include "QSFML_EditorWidget_base.h"
#include "objects/base/GameObject.h"
#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <string>

namespace QSFML
{
	namespace Objects
	{
		class QSFML_EDITOR_WIDGET_EXPORT CameraWindow : public QWidget, public GameObject
		{
			Q_OBJECT
				void setup();
			friend class Scene;
			public:
			CameraWindow(const sf::ContextSettings &settings, const std::string& name = "CameraWindow",
						 QWidget* parent = nullptr);
			CameraWindow(const CameraWindow& other);
			~CameraWindow();

			OBJECT_DECL(CameraWindow);

			
			void setThisCameraView(const sf::View& view);
			const sf::View& getThisCameraView() const;
			const sf::View& getThisCameraDefaultView() const;
			Utilities::AABB getThisCameraViewRect() const;
			sf::Vector2f getThisCameraViewCenterPosition() const;
			sf::Vector2i getThisCameraMousePosition() const;
			sf::Vector2f getThisCameraMouseWorldPosition() const;
			sf::Vector2f getInThisCameraWorldSpace(sf::Vector2i pixelSpace) const;
			sf::Vector2i getInThisCameraScreenSpace(const sf::Vector2f& worldSpace) const;
			sf::Image captureThisCameraScreen() const;
			void captureThisCameraScreen(sf::Image& image) const;

			sf::Vector2u getThisCameraSize() const;
			const sf::Vector2u &getThisCameraOldSize() const { return m_oldViewSize; }
			const sf::Vector2f &getThisCameraDpiScale() const { return m_dpiScale; }

			bool isOpen() const {
				if (m_window)
					return m_window->isOpen();
				return false;
			}

			void enableFrameTimer(bool enable);

			/*
			void setPosition(float x, float y) override;
			void setPosition(const sf::Vector2f& position) override;
			void setRotation(float angle) override;
			void setScale(float factorX, float factorY) override;
			void setScale(const sf::Vector2f& factors) override;
			void setOrigin(float x, float y) override;
			void setOrigin(const sf::Vector2f& origin) override;
			void move(float offsetX, float offsetY) override;
			void move(const sf::Vector2f& offset) override;
			void rotate(float angle) override;
			void scale(float factorX, float factorY) override;
			void scale(const sf::Vector2f& factor) override;
			*/

			private slots:
			void onFrame();

			void pollEvents();
			const QSFML::vector<sf::Event>& getThisCameraEvents() const { return m_events; }
			
			
			


			protected:

			void update() override;
			//void updateView();
			QPaintEngine* paintEngine() const override;
			void showEvent(QShowEvent*) override;
			void closeEvent(QCloseEvent*) override;
			void hideEvent(QHideEvent*) override;

			void resizeEvent(QResizeEvent* event) override;

			sf::RenderWindow* getRenderWindow() { return m_window; }
			private:

			void resizeEvent(const sf::Vector2u& oldSize, const sf::Vector2u& newSize);


			sf::RenderWindow* m_window = nullptr;
			sf::ContextSettings m_settings;
			//sf::View m_view;
			sf::Vector2f m_dpiScale;
			sf::Vector2u m_oldViewSize;
			sf::Vector2u m_currentViewSize;
			QTimer m_frameTimer;
			bool m_enableFrameTimer = true;
			QSFML::vector<sf::Event> m_events;
			sf::Transform m_lastTransform;

			mutable sf::Texture* m_screenCaptureTexture = nullptr;

			//Scene* m_scene = nullptr;
		};
	}
}